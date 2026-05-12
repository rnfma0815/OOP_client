#pragma once
#include <vector>
#include "Card.h"

class Deck {
private:
    std::vector<Card> cards;
    int nextIndex;

public:
    Deck();

    void reset();
    void shuffle();
    Card draw();
};