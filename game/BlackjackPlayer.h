#pragma once
#include "Player.h"

class BlackjackPlayer : public Player {
private:
    std::vector<Card> hiddenCards;

public:
    BlackjackPlayer();

    void clearHiddenCards();
    void setHiddenCards(const std::vector<Card>& cards);
    void useHiddenCard(int index);

    bool hasHiddenCard() const;
    int getHiddenCount() const;

    const std::vector<Card>& getHiddenCards() const;

    int calculateScore() const override;
};