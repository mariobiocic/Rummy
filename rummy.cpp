#include "rummy.h"
#include <algorithm> // Dodato za std::find
#include <climits>   // Dodato za INT_MAX

// Implementacija konstruktora klase Deck
Deck::Deck() {
    for (int suit = static_cast<int>(Suit::HEARTS); suit <= static_cast<int>(Suit::SPADES); ++suit) {
        for (int rank = static_cast<int>(Rank::ACE); rank <= static_cast<int>(Rank::KING); ++rank) {
            cards.push_back({ static_cast<Suit>(suit), static_cast<Rank>(rank) });
        }
    }
    shuffleDeck();
}

// Implementacija funkcije shuffleDeck
void Deck::shuffleDeck() {
    std::random_shuffle(cards.begin(), cards.end());
}

// Implementacija funkcije drawCard
Card Deck::drawCard() {
    if (!empty()) {
        Card drawnCard = cards.back();
        cards.pop_back();
        return drawnCard;
    }
    else {
        std::cerr << "Error: The deck is empty.\n";
        exit(EXIT_FAILURE);
    }
}

// Implementacija funkcije empty
bool Deck::empty() const {
    return cards.empty();
}

// Implementacija funkcije printDeck
void Deck::printDeck() const {
    for (const auto& card : cards) {
        std::cout << "[" << getSuitSymbol(card.suit) << getRankSymbol(card.rank) << "] ";
    }
    std::cout << "\n";
}

// Implementacija funkcije getSuitSymbol
char Deck::getSuitSymbol(Suit suit) const {
    switch (suit) {
    case Suit::HEARTS:
        return 'H';
    case Suit::DIAMONDS:
        return 'D';
    case Suit::CLUBS:
        return 'C';
    case Suit::SPADES:
        return 'S';
    default:
        return '?';
    }
}

// Implementacija funkcije getRankSymbol
char Deck::getRankSymbol(Rank rank) const {
    switch (rank) {
    case Rank::ACE:
        return 'A';
    case Rank::JACK:
        return 'J';
    case Rank::QUEEN:
        return 'Q';
    case Rank::KING:
        return 'K';
    default:
        return static_cast<char>('0' + static_cast<int>(rank));
    }
}

// Implementacija funkcije printHand
void Player::printHand() const {
    for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << "[" << i + 1 << ": " << getSuitSymbol(hand[i].suit) << getRankSymbol(hand[i].rank) << "] ";
    }
    std::cout << "\n";
}

// Implementacija funkcije printHandASCII
void Player::printHandASCII() const {
    for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << " _________\n";
        std::cout << "|         |\n";
        std::cout << "|    " << getRankSymbol(hand[i].rank) << "    |\n";
        std::cout << "|    " << getSuitSymbol(hand[i].suit) << "    |\n";
        std::cout << "|         |\n";
        std::cout << "|_________|\n";
    }
    std::cout << "\n";
}

// Implementacija funkcije drawCard za igrača
Card Player::drawCard(Deck& deck) {
    Card drawnCard = deck.drawCard();
    hand.push_back(drawnCard);
    return drawnCard;
}

// Implementacija funkcije discardCard
void Player::discardCard(size_t index) {
    if (index >= 1 && index <= hand.size()) {
        Card discardedCard = hand[index - 1];
        discardPile.push_back(discardedCard);
        hand.erase(hand.begin() + index - 1);
    }
    else {
        std::cout << "Invalid index. Try again.\n";
    }
}

// Implementacija funkcije getSuitSymbol za igrača
char Player::getSuitSymbol(Suit suit) const {
    switch (suit) {
    case Suit::HEARTS:
        return 'H';
    case Suit::DIAMONDS:
        return 'D';
    case Suit::CLUBS:
        return 'C';
    case Suit::SPADES:
        return 'S';
    default:
        return '?';
    }
}

// Implementacija funkcije getRankSymbol za igrača
char Player::getRankSymbol(Rank rank) const {
    switch (rank) {
    case Rank::ACE:
        return 'A';
    case Rank::JACK:
        return 'J';
    case Rank::QUEEN:
        return 'Q';
    case Rank::KING:
        return 'K';
    default:
        return static_cast<char>('0' + static_cast<int>(rank));
    }
}

// Implementacija funkcije hasValidMeld
bool Player::hasValidMeld() const {
    // Jednostavan primer - proveravamo da li igrač ima bar jednu kartu u meld-u
    return !melds.empty() && !melds[0].empty();
}

// Implementacija funkcije addToMeld
void Player::addToMeld(const std::vector<Card>& meld) {
    melds.push_back(meld);
}

// Implementacija konstruktora klase RummyGame
RummyGame::RummyGame(size_t numPlayers) : currentPlayerIndex(0) {
    for (size_t i = 0; i < numPlayers; ++i) {
        players.push_back(Player());
    }

    dealInitialHands();
}

// Implementacija funkcije dealInitialHands
void RummyGame::dealInitialHands() {
    for (Player& player : players) {
        for (int i = 0; i < 10; ++i) {
            player.drawCard(deck);
        }
    }
}

// Implementacija funkcije playGame
void RummyGame::playGame() {
    while (!isGameOver()) {
        Player& currentPlayer = players[currentPlayerIndex];

        std::cout << "\nPlayer " << currentPlayerIndex + 1 << "'s turn:\n";
        currentPlayer.printHandASCII();  // Promenjeno za prikaz u ASCII artu

        if (currentPlayerIndex == 0) {
            // Korisnički unos za prvog igrača
            int choice;
            do {
                std::cout << "Choose an action:\n"
                    "1. Draw a card\n"
                    "2. Discard a card\n"
                    "Enter your choice (1 or 2): ";
                std::cin >> choice;

                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Please enter a number.\n";
                    choice = -1;
                }
            } while (choice != 1 && choice != 2);

            if (choice == 1) {
                // Draw a card
                Card drawnCard = currentPlayer.drawCard(deck);
                std::cout << "Drew Card: [" << deck.getSuitSymbol(drawnCard.suit) << deck.getRankSymbol(drawnCard.rank) << "]\n";
            }
            else {
                // Discard a card
                currentPlayer.printHand();
                int discardIndex;
                do {
                    std::cout << "Enter the index of the card to discard (1 to " << currentPlayer.hand.size() << "): ";
                    std::cin >> discardIndex;

                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Please enter a number.\n";
                        discardIndex = -1;
                    }
                } while (discardIndex < 1 || discardIndex > static_cast<int>(currentPlayer.hand.size()));

                currentPlayer.discardCard(static_cast<size_t>(discardIndex));
                std::cout << "Discarded Card: [" << deck.getSuitSymbol(currentPlayer.discardPile.back().suit)
                    << deck.getRankSymbol(currentPlayer.discardPile.back().rank) << "]\n";
            }
        }
        else {
            // Automatski potezi za drugog igrača
            Card drawnCard = currentPlayer.drawCard(deck);
            currentPlayer.discardCard(1);
            std::cout << "Drew Card: [" << deck.getSuitSymbol(drawnCard.suit) << deck.getRankSymbol(drawnCard.rank) << "]\n";
            std::cout << "Discarded Card: [" << deck.getSuitSymbol(currentPlayer.discardPile.back().suit)
                << deck.getRankSymbol(currentPlayer.discardPile.back().rank) << "]\n";
        }

        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    }

    std::cout << "\nGame over!\n";

    displayScoresAndWinner();
}

int RummyGame::calculateScore(const Player& player) const {
    int score = 0;

    // Bodovanje preostalih karata u ruci
    for (const auto& card : player.hand) {
        score += getCardValue(card);
    }

    // Bodovanje za svaki meld
    for (const auto& meld : player.melds) {
        for (const auto& card : meld) {
            score += getCardValue(card);
        }
    }

    return score;
}

// Implementacija funkcije isGameOver
bool RummyGame::isGameOver() const {
    return deck.empty();
}

// Implementacija funkcije displayScoresAndWinner
void RummyGame::displayScoresAndWinner() const {
    std::cout << "\nScores:\n";
    for (size_t i = 0; i < players.size(); ++i) {
        std::cout << "Player " << i + 1 << ": " << calculateScore(players[i]) << " points\n";
    }

    int winningScore = INT_MAX;
    size_t winnerIndex = 0;

    for (size_t i = 0; i < players.size(); ++i) {
        if (calculateScore(players[i]) < winningScore) {
            winnerIndex = i;
            winningScore = calculateScore(players[i]);
        }
    }

    std::cout << "\nPlayer " << winnerIndex + 1 << " wins with " << winningScore << " points!\n";
}

// Implementacija funkcije getCardValue
int RummyGame::getCardValue(const Card& card) const {
    switch (card.rank) {
    case Rank::ACE:
        return 1;
    case Rank::JACK:
    case Rank::QUEEN:
    case Rank::KING:
        return 10;
    default:
        return static_cast<int>(card.rank);
    }
}

int main() {
    // Kreirajte Remi igru sa 2 igrača
    RummyGame game(2);

    // Počnite igru
    game.playGame();

    return 0;
}