#pragma once
#include <vector>
#include "Card.h"

class Player {
protected:
    std::vector<Card> pileCards;     // 개인 더미
    std::vector<Card> roundCards;    // 이번 라운드 5장
    std::vector<Card> openedCards;   // 공개한 카드
    int winPoint;

public:
    Player();
    virtual ~Player() {}

    void setPileCards(const std::vector<Card>& cards);

    virtual void prepareRoundCards();
    virtual void clearRound();
    virtual void openNextCard();

    virtual int calculateScore() const = 0;

    bool hasRoundCard() const;
    int getWinPoint() const;
    void addWinPoint();
    void resetWinPoint();

    const std::vector<Card>& getPileCards() const;
    const std::vector<Card>& getRoundCards() const;
    const std::vector<Card>& getOpenedCards() const;
};