#include "Player.h"

Player::Player() : winPoint(0) {}

void Player::setPileCards(const std::vector<Card>& cards) {
    pileCards = cards;
}

void Player::prepareRoundCards() {
    roundCards.clear();
    openedCards.clear();

    for (int i = 0; i < 5; i++) {
        roundCards.push_back(pileCards.front());
        pileCards.erase(pileCards.begin());
    }
}

void Player::clearRound() {
    roundCards.clear();
    openedCards.clear();
}

void Player::openNextCard() {
    if (!roundCards.empty()) {
        openedCards.push_back(roundCards.front());
        roundCards.erase(roundCards.begin());
    }
}

bool Player::hasRoundCard() const {
    return !roundCards.empty();
}

int Player::getWinPoint() const {
    return winPoint;
}

void Player::addWinPoint() {
    winPoint++;
}

void Player::resetWinPoint() {
    winPoint = 0;
}

const std::vector<Card>& Player::getPileCards() const {
    return pileCards;
}

const std::vector<Card>& Player::getRoundCards() const {
    return roundCards;
}

const std::vector<Card>& Player::getOpenedCards() const {
    return openedCards;
}