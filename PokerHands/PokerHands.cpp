#include <iostream>
#include <unordered_map>
#include <set>

struct PokerHand {
    const char* pokerhand_;
    std::multiset<int> values_;
    int rank_;

    enum ranks {
        HighCard,
        Pair,
        TwoPair,
        ThreeOfAKind,
        Straight,
        Flush,
        FullHouse,
        FourOfAKind,
        StraightFlush,
        RoyalFlush
    };

    std::unordered_map<char, int> charValues {
        {'2', 2},
        {'3', 3},
        {'4', 4},
        {'5', 5},
        {'6', 6},
        {'7', 7},
        {'8', 8},
        {'9', 9},
        {'T', 10},
        {'J', 11},
        {'Q', 12},
        {'K', 13},
        {'A', 14}
    };

    int hasCountOf(int n) {
        int currentCount = 0;
        for (const auto& value : values_) {
            currentCount = std::count(values_.begin(), values_.end(), value);
            if (currentCount == n)
                return currentCount;
        }
        return 0;
    }

    bool isTwoPair() {
        int currentCount = 0;
        int numberOfPairs = 0;
        int firstPairValue = 0;

        for (const auto& value : values_) {
            currentCount = std::count(values_.begin(), values_.end(), value);
            if (currentCount == 2) {
                numberOfPairs++;
                break;
            }
        }

        for (const auto& value : values_) {
            if (value != firstPairValue) {
                currentCount = std::count(values_.begin(), values_.end(), value);
                if (currentCount == 2) {
                    numberOfPairs++;
                    break;
                }
            }
        }

        return numberOfPairs == 2 ? true : false;
    }

    bool isFlush() {
        for (int i = 1; i <= 10; i += 3)
            if (pokerhand_[i] != pokerhand_[i + 3])
                return false;
        return true;
    }

    bool isStraight() {
        for (auto it = values_.begin(); it != --values_.end(); ++it) {
            if (*it + 1 != *++it)
                return false;
            --it;
        }
        return true;
    }

public:
    PokerHand(const char* pokerhand)
        : pokerhand_{ pokerhand }
    {
        for (int i = 0; i <= 12; i += 3) {
            values_.insert(charValues.at(pokerhand[i]));
        }

        bool flush{ isFlush() };
        bool straight{ isStraight() };

        if (flush && straight && *values_.begin() == 10)
            rank_ = RoyalFlush;
        else if (flush && straight)
            rank_ = StraightFlush;
        else if (hasCountOf(4))
            rank_ = FourOfAKind;
        else if (hasCountOf(3) && hasCountOf(2))
            rank_ = FullHouse;
        else if (flush)
            rank_ = Flush;
        else if (straight)
            rank_ = Straight;
        else if (hasCountOf(3))
            rank_ = ThreeOfAKind;
        else if (isTwoPair())
            rank_ = TwoPair;
        else if (hasCountOf(2))
            rank_ = Pair;
        else
            rank_ = HighCard;
    };

    const int getRank() const {
        return rank_;
    }

    const std::multiset<int> getValues() const {
        return values_;
    }
};

enum class Result { Win, Loss, Tie };

Result compare(const PokerHand& player, const PokerHand& opponent) {
    int playerRank = player.getRank();
    int opponentRank = opponent.getRank();

    if (playerRank > opponentRank) {
        return Result::Win;
    }
    else if (opponentRank > playerRank) {
        return Result::Loss;
    }
    else {
        auto playerValues = player.getValues();
        auto opponentValues = opponent.getValues();

        auto playerIt = playerValues.rbegin();
        auto opponentIt = opponentValues.rbegin();

        while (playerIt != playerValues.rend() && *playerIt == *opponentIt) {
            ++playerIt;
            ++opponentIt;
        }

        if (playerIt == playerValues.rend())
            return Result::Tie;

        if (*playerIt > *opponentIt)
            return Result::Win;
        else if (*opponentIt > *playerIt)
            return Result::Loss;
        else
            return Result::Tie;
    }
}

bool run_test_hands(const PokerHand& player, const PokerHand& opponent, Result outcome) {
    return outcome == compare(player, opponent);
}

bool run_test(const char* player, const char* opponent, Result outcome) {
    return run_test_hands(PokerHand(player), PokerHand(opponent), outcome);
}

int main()
{
    /*PokerHand hand = PokerHand("2H 3H 4H 5H 6H");
    PokerHand hand2 = PokerHand("KS AS TS QS JS");
    Result result = compare(hand, hand2);*/

    //std::cout << run_test("2H 3H 4H 5H 6H", "KS AS TS QS JS", Result::Loss); // "Highest straight flush wins"
    //std::cout << run_test("2H 3H 4H 5H 6H", "AS AD AC AH JD", Result::Win); // "Straight flush wins of 4 of a kind"
    //std::cout << run_test("AS AH 2H AD AC", "JS JD JC JH 3D", Result::Win); // "Highest 4 of a kind wins"
    //std::cout << run_test("2S AH 2H AS AC", "JS JD JC JH AD", Result::Loss); // "4 Of a kind wins of full house"
    //std::cout << run_test("2S AH 2H AS AC", "2H 3H 5H 6H 7H", Result::Win); // "Full house wins of flush"
    //std::cout << run_test("AS 3S 4S 8S 2S", "2H 3H 5H 6H 7H", Result::Win); // "Highest flush wins"
    //std::cout << run_test("2H 3H 5H 6H 7H", "2S 3H 4H 5S 6C", Result::Win); // "Flush wins of straight"
    //std::cout << run_test("2S 3H 4H 5S 6C", "3D 4C 5H 6H 2S", Result::Tie); // "Equal straight is tie"
    //std::cout << run_test("2S 3H 4H 5S 6C", "AH AC 5H 6H AS", Result::Win); // "Straight wins of three of a kind"
    std::cout << run_test("2S 3H 4H 5S AC", "AH AC 5H 6H AS", Result::Win); // "Low-ace straight wins of three of a kind"
    //std::cout << run_test("2S 2H 4H 5S 4C", "AH AC 5H 6H AS", Result::Loss); // "3 Of a kind wins of two pair"
    std::cout << run_test("2S 2H 4H 5S 4C", "AH AC 5H 6H 7S", Result::Win); // "2 Pair wins of pair"
    //std::cout << run_test("6S AD 7H 4S AS", "AH AC 5H 6H 7S", Result::Loss); // "Highest pair wins"
    //std::cout << run_test("2S AH 4H 5S KC", "AH AC 5H 6H 7S", Result::Loss); // "Pair wins of nothing"
    //std::cout << run_test("2S 3H 6H 7S 9C", "7H 3C TH 6H 9S", Result::Loss); // "Highest card loses"
    //std::cout << run_test("4S 5H 6H TS AC", "3S 5H 6H TS AC", Result::Win); // "Highest card wins"
    //std::cout << run_test("2S AH 4H 5S 6C", "AD 4C 5H 6H 2C", Result::Tie);
    //hand.print();
    //std::cout << hand.isFlush() << std::endl;
    //std::cout << hand.isStraight() << std::endl;
    //std::cout << hand.getRank();
}
