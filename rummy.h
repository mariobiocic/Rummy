#ifndef RUMMY_H
#define RUMMY_H

#include <iostream>
#include <vector>

enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
enum Rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };

struct Card {
    Suit suit;
    Rank rank;
};

class Deck {
private:
    std::vector<Card> cards;

public:
    Deck();
    void shuffleDeck();
    Card drawCard();
    bool empty() const;
    void printDeck() const;
    char getSuitSymbol(Suit suit) const;
    char getRankSymbol(Rank rank) const;
};

struct Player {
    std::vector<Card> hand;
    std::vector<std::vector<Card>> melds;
    std::vector<Card> discardPile;

    void printHand() const;
    void printHandASCII() const;  // Dodana funkcija za prikaz karata u ASCII artu
    Card drawCard(Deck& deck);
    void discardCard(size_t index);
    char getSuitSymbol(Suit suit) const;
    char getRankSymbol(Rank rank) const;
    bool hasValidMeld() const;
    void addToMeld(const std::vector<Card>& meld);
};

class RummyGame {
private:
    Deck deck;
    std::vector<Player> players;
    size_t currentPlayerIndex;
    

public:
    RummyGame(size_t numPlayers);
    void dealInitialHands();
    void playGame();

private:
    bool isGameOver() const;
    void displayScoresAndWinner() const;
    int calculateScore(const Player& player) const;
    int getCardValue(const Card& card) const;
};

#endif
