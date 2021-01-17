#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cstdlib>
#include <ctime>
#include <cmath>
#define DEBUG

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::string;
typedef std::tuple<int, int, int, bool, bool, bool, bool, bool, bool, int, int, int> cardInfo; // cost attack defense abilities(BCDGLW) playerHP enemyHP cardDraw

double p2(double a)
{
    return a * a;
}

enum CardType
{
    CREATURE,
    GREENITEM,
    REDITEM,
    BLUEITEM
};

enum Lane
{
    LEFT,
    RIGHT
};

enum BoardNumber
{
    MYLEFT,
    MYRIGHT,
    OPPONENTLEFT,
    OPPONENTRIGHT
};

struct cardsInfo
{
    cardInfo get(int id)
    {
        cardInfo res = std::make_tuple(costs[id], attacks[id], defenses[id], abilities[id][0], abilities[id][1], abilities[id][2], abilities[id][3], abilities[id][4], abilities[id][5], playerHPs[id], enemyHPs[id], cardDraws[id]);
        return res;
    };
    cardInfo get(int id) const
    {
        cardInfo res = std::make_tuple(costs[id], attacks[id], defenses[id], abilities[id][0], abilities[id][1], abilities[id][2], abilities[id][3], abilities[id][4], abilities[id][5], playerHPs[id], enemyHPs[id], cardDraws[id]);
        return res;
    };
    const int costs[161] = {0, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 6, 7, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 5, 6, 6, 6, 1, 1, 3, 3, 4, 6, 6, 6, 9, 2, 1, 2, 3, 4, 4, 4, 3, 2, 4, 4, 6, 7, 7, 9, 12, 2, 2, 2, 5, 6, 6, 3, 4, 4, 4, 4, 5, 5, 6, 7, 8, 8, 8, 9, 7, 0, 2, 3, 3, 4, 5, 5, 8, 0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 7, 8, 12, 1, 0, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 4, 6, 0, 2, 2, 4, 2, 0, 0, 0, 1, 3, 4, 2, 2, 3, 2, 5, 7, 2, 2, 3, 3, 3, 3, 4, 2};
    const int attacks[161] = {0, 2, 1, 2, 1, 4, 3, 2, 2, 3, 3, 5, 2, 5, 9, 4, 6, 4, 7, 5, 8, 6, 7, 8, 1, 3, 3, 2, 1, 2, 4, 3, 3, 4, 3, 5, 4, 5, 1, 2, 2, 2, 4, 5, 3, 6, 7, 1, 1, 1, 3, 3, 2, 1, 2, 0, 2, 1, 5, 7, 4, 10, 12, 0, 1, 2, 5, 5, 7, 4, 6, 3, 5, 4, 5, 6, 5, 7, 5, 8, 8, 6, 5, 1, 1, 2, 1, 2, 4, 4, 5, 1, 0, 2, 1, 2, 3, 3, 2, 2, 1, 3, 3, 3, 4, 4, 5, 3, 2, 5, 0, 6, 4, 2, 7, 5, 8, 1, 0, 1, 1, 0, 1, 4, 2, 1, 2, 0, 4, 2, 0, 4, 3, 4, 2, 5, 1, 0, 0, 0, 0, -1, 0, 0, 0, -2, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const int defenses[161] = {0, 1, 2, 2, 5, 1, 2, 2, 3, 4, 1, 2, 5, 3, 1, 5, 2, 5, 4, 6, 2, 5, 5, 8, 1, 1, 2, 2, 2, 1, 2, 1, 2, 3, 5, 2, 4, 7, 3, 1, 3, 2, 2, 5, 7, 5, 7, 5, 1, 2, 2, 5, 4, 1, 2, 5, 7, 8, 6, 7, 8, 10, 12, 4, 1, 2, 1, 5, 5, 4, 3, 2, 3, 4, 4, 5, 5, 7, 5, 8, 8, 6, 5, 1, 1, 3, 5, 5, 4, 1, 5, 2, 1, 1, 4, 3, 2, 3, 4, 5, 6, 4, 3, 6, 4, 6, 5, 3, 6, 6, 9, 6, 7, 4, 7, 5, 8, 1, 3, 2, 0, 3, 3, 0, 1, 4, 3, 6, 3, 5, 6, 1, 3, 0, 2, 5, 1, 0, 0, 0, 0, -1, 0, 0, -2, -2, -2, -1, -2, 0, -3, -99, -7, 0, 0, -3, 0, -1, -4, -3, 0};
    const int playerHPs[161] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 3, 1, 0, 3, 2};
    const int enemyHPs[161] = {0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -2, -1, 0, 0, 0, -2, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, -2, -1, -3, 0, 0, 0, -2};
    const int cardDraws[161] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0};
    const int abilities[161][6] = {
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0},
        {0, 0, 1, 1, 0, 0},
        {0, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 1, 0},
        {1, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 0},
        {0, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 1, 0},
        {0, 0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {1, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 1},
        {0, 0, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {1, 0, 0, 1, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {1, 0, 0, 1, 0, 0},
        {1, 1, 0, 0, 0, 0},
        {1, 0, 1, 0, 0, 1},
        {0, 1, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 1},
        {0, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 0},
        {0, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 1},
        {1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 1},
        {0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0}};
};

class CardOption
{
public:
    // for summoning creature
    CardOption(int id, int lane, const cardsInfo &cards) : id_(id), lane_(lane)
    {
        cardInfo nowCard = cards.get(id_);
        std::tie(cost_, attack_, defense_, abilities_[0], abilities_[1], abilities_[2], abilities_[3], abilities_[4], abilities_[5], playerHP_, enemyHP_, cardDraw_) = nowCard;
    };
    CardOption(int id, int instanceID, int cost, int attack, int defense, string abilities, int playerHP, int enemyHP, int cardDraw, int lane) : id_(id), instanceID_(instanceID), cost_(cost), attack_(attack), defense_(defense), playerHP_(playerHP), enemyHP_(enemyHP), cardDraw_(cardDraw), lane_(lane)
    {
        for (int i = 0; i < 6; ++i)
        {
            if (abilities[i] != '-')
                abilities_[i] = 1;
            else
                abilities_[i] = 0;
        }
    };
    // for use item
    CardOption(int id, const cardsInfo &cards) : id_(id)
    {
        cardInfo nowCard = cards.get(id_);
        std::tie(cost_, attack_, defense_, abilities_[0], abilities_[1], abilities_[2], abilities_[3], abilities_[4], abilities_[5], playerHP_, enemyHP_, cardDraw_) = nowCard;
    };
    CardOption(int id, int instanceID, int cost, int attack, int defense, string abilities, int playerHP, int enemyHP, int cardDraw) : id_(id), instanceID_(instanceID), cost_(cost), attack_(attack), defense_(defense), playerHP_(playerHP), enemyHP_(enemyHP), cardDraw_(cardDraw)
    {
        for (int i = 0; i < 6; ++i)
        {
            if (abilities[i] != '-')
                abilities_[i] = 1;
            else
                abilities_[i] = 0;
        }
    };
    void itemCalulateScore(){
        // TODO
    };
    void creatureCalulateScore(int enemyTotalHP, int ownTotalHP, int enemyTotalAttack, int ownTotalAttack)
    {
        score_ = {{exprs[0]}};
    };
    void creatureTake(string &actions)
    {
        actions += "SUMMON " + std::to_string(instanceID_) + " " + std::to_string(lane_) + ";";
    };
    void creatureTake(string &actions) const
    {
        actions += "SUMMON " + std::to_string(instanceID_) + " " + std::to_string(lane_) + ";";
    };
    void itemTake(){

    };
    void itemTake() const {

    };
    int getLane()
    {
        return lane_;
    };
    bool operator<(const CardOption &rhs) const
    {
        return score_ < rhs.score_;
    };
    bool operator<(const CardOption &rhs)
    {
        return score_ < rhs.score_;
    };

private:
    int id_, instanceID_, lane_;
    int cost_, attack_, defense_, abilities_[6], playerHP_, enemyHP_, cardDraw_;
    int score_;
};

struct Creature
{
    Creature(int instanceID, int attack, int defense, string abilities, int lane) : instanceID(instanceID), attack(attack), defense(defense), abilities(abilities), lane(lane){};
    int instanceID, attack, defense, lane;
    string abilities;
};

int main()
{
    // srand(time(NULL));
    srand(28976);
    const cardsInfo cards;
    while (1)
    {
        // store status of the board
        std::vector<CardOption> creatureCardOptions;
        std::vector<Creature> board[4]; // myLeft myRight opponentLeft opponentRight
        int enemyLeftTotalHP = 0, enemyLeftTotalAttack = 0, enemyRightTotalHP = 0, enemyRightTotalAttack = 0, ownLeftTotalHP = 0, ownLeftTotalAttack = 0, ownRightTotalHP = 0, ownRightTotalAttack = 0;

        // game information
        int playerHealth, playerMana, playerDeck, playerRune, playerDraw;
        int opponentHealth, opponentMana, opponentDeck, opponentRune, opponentDraw;

        cin >> playerHealth >> playerMana >> playerDeck >> playerRune >> playerDraw;
        cin >> opponentHealth >> opponentMana >> opponentDeck >> opponentRune >> opponentDraw;

        int opponentHand;
        int opponentActions;
        cin >> opponentHand >> opponentActions;
        cin.ignore();
        for (int i = 0; i < opponentActions; i++)
        {
            string cardNumberAndAction;
            getline(cin, cardNumberAndAction);
        }
        int cardCount;
        cin >> cardCount;
        cin.ignore();
        for (int i = 0; i < cardCount; i++)
        {
            int cardNumber, instanceID, location, cardType, cost, attack, defense, myHealthChange, opponentHealthChange, cardDraw, lane;
            string abilities;
            cin >> cardNumber >> instanceID >> location >> cardType >> cost >> attack >> defense >> abilities >> myHealthChange >> opponentHealthChange >> cardDraw >> lane;
            cin.ignore();

            if (location == 0 && cardType == CREATURE)
            { // in my hand
                CardOption summonLeft(cardNumber, instanceID, cost, attack, defense, abilities, myHealthChange, opponentHealthChange, cardDraw, 0);
                CardOption summonRight(cardNumber, instanceID, cost, attack, defense, abilities, myHealthChange, opponentHealthChange, cardDraw, 1);
                creatureCardOptions.emplace_back(summonLeft);
                creatureCardOptions.emplace_back(summonRight);
            }
            else if (location == -1)
            { // in opponent side of board
                Creature tmp(instanceID, attack, defense, abilities, lane);
                if (lane == LEFT)
                {
                    board[OPPONENTLEFT].emplace_back(tmp);
                    enemyLeftTotalAttack += attack;
                    enemyLeftTotalHP += defense;
                }
                else
                {
                    board[OPPONENTRIGHT].emplace_back(tmp);
                    enemyRightTotalAttack += attack;
                    enemyRightTotalHP += defense;
                }
            }
            else if (location == 1)
            { // in my side of board
                Creature tmp(instanceID, attack, defense, abilities, lane);
                if (lane == LEFT)
                {
                    board[MYLEFT].emplace_back(tmp);
                    ownLeftTotalAttack += attack;
                    ownLeftTotalHP += defense;
                }
                else
                {
                    board[MYRIGHT].emplace_back(tmp);
                    ownRightTotalAttack += attack;
                    ownRightTotalHP += defense;
                }
            }
        }

        // Draw Phase
        if (playerMana == 0)
        {
            cout << "PASS" << endl;
            // TODO Not Random Draw
        }
        // Battle Phase
        else
        {
            string actions;
            // Summon
            for (auto &option : creatureCardOptions)
            {
                if (option.getLane() == 0)
                { // Left
                    option.creatureCalulateScore(enemyLeftTotalHP, ownLeftTotalHP, enemyLeftTotalAttack, ownLeftTotalAttack);
                }
                else if (option.getLane() == 1)
                { // Right
                    option.creatureCalulateScore(enemyRightTotalHP, ownRightTotalHP, enemyRightTotalAttack, ownRightTotalAttack);
                }
            }

            sort(creatureCardOptions.begin(), creatureCardOptions.end());
            for (const auto &option : creatureCardOptions)
            {
                option.creatureTake(actions);
            }

            // Use Item
            // TODO

            // Attack
            // Now with simple greedy strategy
            if (board[2].size() == 0)
            {
                for (const auto &creature : board[0])
                {
                    actions += "ATTACK " + std::to_string(creature.instanceID) + " -1;";
                }
            }
            else
            {
                for (const auto &creature : board[0])
                {
                    actions += "ATTACK " + std::to_string(creature.instanceID) + " " + std::to_string(board[2][0].instanceID) + ";";
                }
            }
            if (board[3].size() == 0)
            {
                for (const auto &creature : board[1])
                {
                    actions += "ATTACK " + std::to_string(creature.instanceID) + " -1;";
                }
            }
            else
            {
                for (const auto &creature : board[1])
                {
                    actions += "ATTACK " + std::to_string(creature.instanceID) + " " + std::to_string(board[3][0].instanceID) + ";";
                }
            }
            // do actions
            if (actions.size())
                cout << actions << endl;
            else
                cout << "PASS" << endl;
        }
    }
}