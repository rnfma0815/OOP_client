#include "Blackjack.h"
#include "InputHelper.h"
#include <conio.h>
#include <windows.h>
#include <vector>

Blackjack::Blackjack() : currentRound(1) {}

void Blackjack::initializeGame() {
    currentRound = 1;

    player.resetWinPoint();
    dealer.resetWinPoint();

    player.clearRound();
    dealer.clearRound();
    player.clearHiddenCards();

    deck.reset();
    deck.shuffle();

    dealCards();
}

void Blackjack::dealCards() {
    std::vector<Card> dealerPile;
    std::vector<Card> playerPile;
    std::vector<Card> hidden;

    for (int i = 0; i < 25; i++) {
        dealerPile.push_back(deck.draw());
    }

    for (int i = 0; i < 25; i++) {
        playerPile.push_back(deck.draw());
    }

    for (int i = 0; i < 2; i++) {
        hidden.push_back(deck.draw());
    }

    dealer.setPileCards(dealerPile);
    player.setPileCards(playerPile);
    player.setHiddenCards(hidden);
}

int Blackjack::selectHiddenCard() {
    if (player.getHiddenCount() == 0) return -1;
    if (player.getHiddenCount() == 1) return 0;

    int selected = 0;

    while (true) {
        view.drawHiddenSelectScreen(player, selected);

        InputHelper::Key key = InputHelper::readKey();

        if (key == InputHelper::Key::LEFT) {
            selected = (selected - 1 + player.getHiddenCount()) % player.getHiddenCount();
        }
        else if (key == InputHelper::Key::RIGHT) {
            selected = (selected + 1) % player.getHiddenCount();
        }
        else if (key == InputHelper::Key::ENTER) {
            return selected;
        }
        else if (key == InputHelper::Key::ESC) {
            return -1;
        }
    }
}

State::PauseResult Blackjack::playerTurn() {
    int selectedMenu = 0;
    bool stop = false;

    while (!stop) {
        view.drawGameScreen(player, dealer, currentRound, selectedMenu);

        InputHelper::Key key = InputHelper::readKey();

        if (key == InputHelper::Key::ESC) {
            State::PauseResult result = showPauseMenu();

            if (result == State::PauseResult::RESUME) {
                continue;
            }
            else if (result == State::PauseResult::RESTART) {
                return State::PauseResult::RESTART;
            }
            else if (result == State::PauseResult::EXIT_TO_MENU) {
                return State::PauseResult::EXIT_TO_MENU;
            }
        }

        if (key == InputHelper::Key::LEFT) {
            selectedMenu = (selectedMenu - 1 + 3) % 3;
        }
        else if (key == InputHelper::Key::RIGHT) {
            selectedMenu = (selectedMenu + 1) % 3;
        }
        else if (key == InputHelper::Key::ENTER) {
            if (selectedMenu == 0) {
                if (player.hasRoundCard()) {
                    player.openNextCard();
                }
            }
            else if (selectedMenu == 1) {
                stop = true;
            }
            else if (selectedMenu == 2) {
                int hiddenIndex = selectHiddenCard();
                if (hiddenIndex != -1) {
                    player.useHiddenCard(hiddenIndex);
                }
            }

            if (player.calculateScore() > 21) {
                stop = true;
            }

            if (!player.hasRoundCard() && selectedMenu == 0) {
                stop = true;
            }
        }
    }

    return State::PauseResult::RESUME;
}

void Blackjack::judgeRound() {
    int playerScore = player.calculateScore();
    int dealerScore = dealer.calculateScore();

    if (playerScore == 21) {
        player.addWinPoint();
        return;
    }

    if (playerScore > 21) {
        dealer.addWinPoint();
        return;
    }

    if (dealerScore > 21) {
        player.addWinPoint();
        return;
    }

    int playerDiff = 21 - playerScore;
    int dealerDiff = 21 - dealerScore;

    if (playerDiff < dealerDiff) {
        player.addWinPoint();
    }
    else if (dealerDiff < playerDiff) {
        dealer.addWinPoint();
    }
}

std::vector<Card> Blackjack::getPossibleRemainingCardsForDealer() const {
    std::vector<Card> possible;

    std::vector<std::string> ranks = {
        "A", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "J", "Q", "K"
    };

    std::vector<std::string> suits = {
        "Spade", "Diamond", "Heart", "Clover"
    };

    // 전체 52장 생성
    for (const auto& suit : suits) {
        for (const auto& rank : ranks) {
            possible.push_back(Card(rank, suit));
        }
    }

    auto removeCardOnce = [&](const Card& usedCard) {
        for (auto it = possible.begin(); it != possible.end(); ++it) {
            if (it->getRank() == usedCard.getRank() &&
                it->getSuit() == usedCard.getSuit()) {
                possible.erase(it);
                break;
            }
        }
        };

    // 현재 라운드에서 이미 공개된 카드 제거
    for (const Card& card : player.getOpenedCards()) {
        removeCardOnce(card);
    }

    for (const Card& card : dealer.getOpenedCards()) {
        removeCardOnce(card);
    }

    // 플레이어 히든카드도 딜러가 모른다고 보면 제거 안 해도 되고,
    // 이미 게임 시스템상 존재하는 카드이므로 확률 정확도를 높이려면 제거 가능
    for (const Card& card : player.getHiddenCards()) {
        removeCardOnce(card);
    }

    return possible;
}

State::PauseResult Blackjack::playRound() {
    player.prepareRoundCards();
    dealer.prepareRoundCards();

    if (player.hasRoundCard()) {
        player.openNextCard();
    }

    State::PauseResult result = playerTurn();

    if (result == State::PauseResult::RESTART || result == State::PauseResult::EXIT_TO_MENU) {
        player.clearRound();
        dealer.clearRound();
        return result;
    }

    dealer.playTurn(player.calculateScore(), difficulty);

    judgeRound();
    view.showRoundResult(player, dealer, currentRound);

    player.clearRound();
    dealer.clearRound();

    return State::PauseResult::RESUME;
}

void Blackjack::play() {
    while (true) {
        initializeGame();

        bool restartGame = false;

        for (currentRound = 1; currentRound <= 5; currentRound++) {
            State::PauseResult result = playRound();

            if (result == State::PauseResult::RESTART) {
                restartGame = true;
                break;
            }
            else if (result == State::PauseResult::EXIT_TO_MENU) {
                return;
            }
        }

        if (restartGame) {
            continue;
        }

        view.showFinalResult(player.getWinPoint(), dealer.getWinPoint());
        return;
    }
}
