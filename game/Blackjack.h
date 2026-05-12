#pragma once
#include "Game.h"
#include "Deck.h"
#include "BlackjackPlayer.h"
#include "BlackjackDealer.h"
#include "BlackjackView.h"

class Blackjack : public Game {
private:
    Deck deck;
    BlackjackPlayer player;
    BlackjackDealer dealer;
    BlackjackView view;
    std::vector<Card> getPossibleRemainingCardsForDealer() const;
    int currentRound;

public:
    Blackjack();
    void play() override;

private:
    void initializeGame();
    void dealCards();
    State::PauseResult playRound();

    State::PauseResult playerTurn();
    void judgeRound();

    int selectHiddenCard();
};
