#include "rummy.h"
#include <algorithm>
#include <climits>
#include <random>
#include <chrono>

using namespace std;

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
    unsigned seed = static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count());
    mt19937 generator(seed);

    shuffle(cards.begin(), cards.end(), generator);
}

// Implementacija funkcije drawCard
Card Deck::drawCard() {
    if (!empty()) {
        Card drawnCard = cards.back();
        cards.pop_back();
        return drawnCard;
    }
    else {
        cerr << "Error: The deck is empty.\n";
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
        cout << "[" << getSuitSymbol(card.suit) << getRankSymbol(card.rank) << "] ";
    }
    cout << "\n";
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
        cout << "[" << i + 1 << ": " << getSuitSymbol(hand[i].suit) << getRankSymbol(hand[i].rank) << "] ";
    }
    cout << "\n";
}

// Implementacija funkcije printHandASCII
void Player::printHandASCII() const {
    for (size_t i = 0; i < hand.size(); ++i) {
        cout << " _________\n";
        cout << "|         |\n";
        cout << "|    " << getRankSymbol(hand[i].rank) << "    |\n";
        cout << "|    " << getSuitSymbol(hand[i].suit) << "    |\n";
        cout << "|         |\n";
        cout << "|_________|\n";
    }
    cout << "\n";
}

// Implementacija funkcije drawCard za igrača
Card Player::drawCard(Deck& deck) {
    // Uzimanje nove karte
    Card drawnCard = deck.drawCard();
    hand.push_back(drawnCard);

    // Ako igrač ima više od 10 karata, pitajte ga koju kartu želi odbaciti
    if (hand.size() > 10) {
        cout << "Your hand has more than 10 cards. Choose a card to discard:\n";
        printHand();

        int discardIndex;
        do {
            cout << "Enter the index of the card to discard (1 to " << hand.size() << "): ";
            cin >> discardIndex;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                discardIndex = -1;
            }
        } while (discardIndex < 1 || discardIndex > static_cast<int>(hand.size()));

        // Odbacivanje odabrane karte
        discardCard(static_cast<size_t>(discardIndex));
    }

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
        cout << "Invalid index. Try again.\n";
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
void Player::addToMeld(const vector<Card>& meld) {
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

        cout << "\nPlayer " << currentPlayerIndex + 1 << "'s turn:\n";
        currentPlayer.printHandASCII();

        // Draw a card
        Card drawnCard = currentPlayer.drawCard(deck);
        cout << "Drew Card: [" << deck.getSuitSymbol(drawnCard.suit) << deck.getRankSymbol(drawnCard.rank) << "]\n";

        // Ask user which card to discard
        if (currentPlayer.hand.size() > 10) {
            cout << "Your hand has more than 10 cards. Choose a card to discard:\n";
            currentPlayer.printHand();

            int discardIndex;
            do {
                cout << "Enter the index of the card to discard (1 to " << currentPlayer.hand.size() << "): ";
                cin >> discardIndex;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number.\n";
                    discardIndex = -1;
                }
            } while (discardIndex < 1 || discardIndex > static_cast<int>(currentPlayer.hand.size()));

            // Discard the chosen card
            currentPlayer.discardCard(static_cast<size_t>(discardIndex));
            cout << "Discarded Card: [" << deck.getSuitSymbol(currentPlayer.discardPile.back().suit)
                << deck.getRankSymbol(currentPlayer.discardPile.back().rank) << "]\n";
        }

        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    }

    cout << "\nGame over!\n";

    displayScoresAndWinner();
}

// Implementacija funkcije calculateScore
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
    cout << "\nScores:\n";
    for (size_t i = 0; i < players.size(); ++i) {
        cout << "Player " << i + 1 << ": " << calculateScore(players[i]) << " points\n";
    }

    int winningScore = INT_MAX;
    size_t winnerIndex = 0;

    for (size_t i = 0; i < players.size(); ++i) {
        if (calculateScore(players[i]) < winningScore) {
            winnerIndex = i;
            winningScore = calculateScore(players[i]);
        }
    }

    cout << "\nPlayer " << winnerIndex + 1 << " wins with " << winningScore << " points!\n";
}

// Implementacija funkcije getCardValue
int RummyGame::getCardValue(const Card& card) const {
    if (card.rank == Rank::WILD) {
        // Ako je wild card, dodijeli mu 25 bodova
        return 25;
    }
    else {
        switch (card.rank) {
        case Rank::ACE:
            return 1;
        case Rank::TWO:
        case Rank::THREE:
        case Rank::FOUR:
        case Rank::FIVE:
        case Rank::SIX:
        case Rank::SEVEN:
        case Rank::EIGHT:
        case Rank::NINE:
        case Rank::TEN:
            return static_cast<int>(card.rank);
        case Rank::JACK:
        case Rank::QUEEN:
        case Rank::KING:
            return 10;
        default:
            return 0;  
        }
    }
}
int main() {
    // Kreirajte Remi igru sa 2 igrača
    RummyGame game(2);

    // Počnite igru
    game.playGame();

    return 0;
}