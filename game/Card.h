#pragma once
#include <string>

class Card {
private:
    std::string rank;
    std::string suit;

public:
    Card();
    Card(const std::string& rank, const std::string& suit);

    std::string getRank() const;
    std::string getSuit() const;
    std::string getSuitSymbol() const;
    int getValue() const;
    std::string toString() const;
};