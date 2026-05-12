#include "BlackjackPlayer.h"

BlackjackPlayer::BlackjackPlayer() : Player() {}

void BlackjackPlayer::clearHiddenCards() {
    hiddenCards.clear();
}

void BlackjackPlayer::setHiddenCards(const std::vector<Card>& cards) {
    hiddenCards = cards;
}

void BlackjackPlayer::useHiddenCard(int index) {
    if (index >= 0 && index < (int)hiddenCards.size()) {
        openedCards.push_back(hiddenCards[index]);
        hiddenCards.erase(hiddenCards.begin() + index);
    }
}

bool BlackjackPlayer::hasHiddenCard() const {
    return !hiddenCards.empty();
}

int BlackjackPlayer::getHiddenCount() const {
    return (int)hiddenCards.size();
}

const std::vector<Card>& BlackjackPlayer::getHiddenCards() const {
    return hiddenCards;
}

int BlackjackPlayer::calculateScore() const {
    int total = 0;
    int aceCount = 0;

    for (const Card& card : openedCards) {
        total += card.getValue();
        if (card.getRank() == "A") {
            aceCount++;
        }
    }

    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }

    return total;
}