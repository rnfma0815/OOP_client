#include "Deck.h"
#include <algorithm>
#include <random>
#include <chrono>

Deck::Deck() {
    reset();
    shuffle();
}

void Deck::reset() {
    cards.clear();
    nextIndex = 0;

    std::vector<std::string> ranks = {
        "A", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "J", "Q", "K"
    };

    std::vector<std::string> suits = {
        "Spade", "Diamond", "Heart", "Clover"
    };

    for (const auto& suit : suits) {
        for (const auto& rank : ranks) {
            cards.push_back(Card(rank, suit));
        }
    }
}

void Deck::shuffle() {
    unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
    nextIndex = 0;
}

Card Deck::draw() {
    return cards[nextIndex++];
}