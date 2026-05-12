#include "Card.h"

Card::Card() : rank("A"), suit("Spade") {}

Card::Card(const std::string& rank, const std::string& suit)
    : rank(rank), suit(suit) {}

std::string Card::getRank() const {
    return rank;
}

std::string Card::getSuit() const {
    return suit;
}

std::string Card::getSuitSymbol() const {
    if (suit == "Spade") return "♠";
    if (suit == "Diamond") return "♦";
    if (suit == "Heart") return "♥";
    return "♣";
}

int Card::getValue() const {
    if (rank == "A") return 11;
    if (rank == "K" || rank == "Q" || rank == "J") return 10;
    return std::stoi(rank);
}

std::string Card::toString() const {
    return rank + getSuitSymbol();
}