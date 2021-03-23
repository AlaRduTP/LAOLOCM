#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

enum CardType
{
    CREATURE,
    GREENITEM,
    REDITEM,
    BLUEITEM
};

enum Location
{
    IN_HAND,
    MY_SIDE,
    OPPONENT_SIDE = -1
};

enum Lane
{
    LEFT,
    RIGHT,
    NOT_IN = -1
};

enum BoardNumber
{
    MYLEFT,
    MYRIGHT,
    OPPONENTLEFT,
    OPPONENTRIGHT
};

double p2(double a)
{
    return a * a;
}

class Card
{
public:
    Card(int cardNumber, int instanceId, int location, int cardType, int cost, int atk, int def, std::string abilities, int myhealthChange, int opponentHealthChange, int cardDraw, int lane, int index) : cardNumber_(cardNumber), instanceId_(instanceId), location_(location), cardType_(cardType), cost_(cost), attack_(atk), defense_(def), abilities_(abilities), myhealthChange_(myhealthChange), opponentHealthChange_(opponentHealthChange), cardDraw_(cardDraw), lane_(lane), _index(index){};
    int instanceID() const { return instanceId_; };
    int location() const { return location_; };
    int cost() const { return cost_; };
    int attack() const { return attack_; };
    int defense() const { return defense_; };
    int lane() const { return lane_; };
    std::string abilities() const { return abilities_; };
    void atkDiff(int atk) { attack_ += atk; };
    void defDiff(int def) { defense_ += def; };
    bool operator<(const Card &rhs) const { return score_ < rhs.score_; };
    bool operator==(const Card &rhs) const { return score_ == rhs.score_; };
    void calculateGetScore()
    {
        score_ = 1;
    }
    void pick(std::string &action) const
    {
        action += "PICK " + std::to_string(_index) + ";";
    };

private:
    int cardNumber_, instanceId_, location_, cardType_, cost_, attack_, defense_;
    int myhealthChange_, opponentHealthChange_, cardDraw_, lane_, _index;
    double score_ = -1;
    std::string abilities_;
    friend class CreatureCard;
    friend class ItemCard;
};

class CreatureCard : public Card
{
public:
    CreatureCard(int cardNumber, int instanceId, int location, int cardType, int cost, int atk, int def, std::string abilities, int myhealthChange, int opponentHealthChange, int cardDraw, int lane, int index) : Card(cardNumber, instanceId, location, cardType, cost, atk, def, abilities, myhealthChange, opponentHealthChange, cardDraw, lane, index){};
    void summon(std::string &action, int lane) const
    {
        action += "SUMMON " + std::to_string(instanceID()) + " " + std::to_string(lane) + ";";
    };
    bool canKill(const CreatureCard &c) { return c.defense() <= this->attack(); };
    void attackTo(std::string &action, CreatureCard &target)
    {
        target.defDiff(this->attack());
        this->defDiff(target.attack());
        action += "ATTACK " + std::to_string(this->instanceID()) + " " + std::to_string(target.instanceID()) + ";";
    };
    void calculateUseScore(int enemyTotalHP, int ownTotalHP, int enemyTotalAttack, int ownTotalAttack)
    {
        score_ = (((((defense_ - cost_) - exp(abilities_[4])) + p2(exp(cardDraw_ + enemyTotalAttack) / cost_)) + ((cardDraw_ - cost_) - 16)) + p2(exp(exp(p2(abilities_[4])) / sqrt((attack_ * enemyTotalAttack) - log((3 * attack_) - log(p2(attack_))))) / enemyTotalAttack));
    }

private:
};

class ItemCard : public Card
{
public:
    ItemCard(int cardNumber, int instanceId, int location, int cardType, int cost, int atk, int def, std::string abilities, int myhealthChange, int opponentHealthChange, int cardDraw, int lane, int index) : Card(cardNumber, instanceId, location, cardType, cost, atk, def, abilities, myhealthChange, opponentHealthChange, cardDraw, lane, index){};
    void use(std::string &action, int target)
    {
        action += "USE " + std::to_string(instanceID()) + " " + std::to_string(target) + ";";
    };

    void calculateUseScore()
    {
        score_ = 1;
    }

private:
};

int main()
{
    //    std::string action;
    //    CreatureCard c(1, 1, 1, 1, 1, 1, 1, "G", 1, 1, 1, 1), d(1, 2, 1, 1, 1, 1, 1, "G", 1, 1, 1, 1);
    //    c.summon(action, 0);
    //    d.summon(action, 1);
    //    std::cout << c.canKill(d) << std::endl;
    //    c.attackTo(action, d);

    //    ItemCard i(1, 1, 1, 1, 1, 1, 1, "G", 1, 1, 1, 0);
    //    i.use(action, d.instanceID());

    //    std::cout << action << std::endl;
    //    std::cout << (i == d)<< std::endl;
    while (1)
    {
        std::vector<Card> cardOptions;
        std::vector<CreatureCard> board[4]; // myLeft myRight opponentLeft opponentRight
        int enemyLeftTotalHP = 0, enemyLeftTotalAttack = 0, enemyRightTotalHP = 0, enemyRightTotalAttack = 0, ownLeftTotalHP = 0, ownLeftTotalAttack = 0, ownRightTotalHP = 0, ownRightTotalAttack = 0;
        // game information
        int playerHealth, playerMana, playerDeck, playerRune, playerDraw;
        int opponentHealth, opponentMana, opponentDeck, opponentRune, opponentDraw;

        std::cin >> playerHealth >> playerMana >> playerDeck >> playerRune >> playerDraw;
        std::cin >> opponentHealth >> opponentMana >> opponentDeck >> opponentRune >> opponentDraw;

        CreatureCard player(-1, -1, MY_SIDE, CREATURE, 0, 0, playerHealth, "", 0, 0, 0, -1, -1);
        CreatureCard opponent(-1, -1, OPPONENT_SIDE, CREATURE, 0, 0, opponentHealth, "", 0, 0, 0, -1, -1);

        int opponentHand;
        int opponentActions;
        std::cin >> opponentHand >> opponentActions;
        std::cin.ignore();

        for (int i = 0; i < opponentActions; i++)
        {
            std::string cardNumberAndAction;
            getline(std::cin, cardNumberAndAction);
        }
        int cardCount;
        std::cin >> cardCount;
        std::cin.ignore();

        for (int i = 0; i < cardCount; i++)
        {
            int cardNumber, instanceID, location, cardType, cost, attack, defense, myHealthChange, opponentHealthChange, cardDraw, lane;
            std::string abilities;
            std::cin >> cardNumber >> instanceID >> location >> cardType >> cost >> attack >> defense >> abilities >> myHealthChange >> opponentHealthChange >> cardDraw >> lane;
            std::cin.ignore();

            if (location == IN_HAND)
            { // in my hand
                if (cardType == CREATURE)
                {
                    CreatureCard summonLeft(cardNumber, instanceID, IN_HAND, cardType, cost, attack, defense, abilities, myHealthChange, opponentHealthChange, cardDraw, LEFT, i);
                    cardOptions.push_back(summonLeft);
                    CreatureCard summonRight(cardNumber, instanceID, IN_HAND, cardType, cost, attack, defense, abilities, myHealthChange, opponentHealthChange, cardDraw, RIGHT, i);
                    cardOptions.push_back(summonRight);
                }
                else
                {
                    ItemCard item(cardNumber, instanceID, IN_HAND, cardType, cost, attack, defense, abilities, myHealthChange, opponentHealthChange, cardDraw, lane, i);
                    cardOptions.push_back(item);
                }
            }
            else if (location == OPPONENT_SIDE)
            { // in opponent side of board
                CreatureCard tmp(cardNumber, instanceID, OPPONENT_SIDE, CREATURE, cost, attack, defense, abilities, myHealthChange, opponentHealthChange, cardDraw, lane, i);
                if (lane == LEFT)
                {
                    board[OPPONENTLEFT].push_back(tmp);
                    enemyLeftTotalAttack += attack;
                    enemyLeftTotalHP += defense;
                }
                else
                {
                    board[OPPONENTRIGHT].push_back(tmp);
                    enemyRightTotalAttack += attack;
                    enemyRightTotalHP += defense;
                }
            }
            else if (location == MY_SIDE)
            { // in my side of board
                CreatureCard tmp(cardNumber, instanceID, IN_HAND, CREATURE, cost, attack, defense, abilities, myHealthChange, opponentHealthChange, cardDraw, lane, i);
                if (lane == LEFT)
                {
                    board[MYLEFT].push_back(tmp);
                    ownLeftTotalAttack += attack;
                    ownLeftTotalHP += defense;
                }
                else
                {
                    board[MYRIGHT].push_back(tmp);
                    ownRightTotalAttack += attack;
                    ownRightTotalHP += defense;
                }
            }
        }

        std::string actions;

        // Draw Phase
        if (playerMana == 0)
        {
            for (auto &option : cardOptions)
            {
                option.calculateGetScore();
            }
            std::sort(cardOptions.begin(), cardOptions.end());
            for (const auto &option : cardOptions)
            {
                option.pick(actions);
            }
        }
        // Battle Phase
        else
        {
            // Summon
            for (auto &option : cardOptions)
            {
                if (option.lane() == LEFT)
                { // Left
                    ((CreatureCard *)&option)->calculateUseScore(enemyLeftTotalHP, ownLeftTotalHP, enemyLeftTotalAttack, ownLeftTotalAttack);
                }
                else if (option.lane() == RIGHT)
                { // Right
                    ((CreatureCard *)&option)->calculateUseScore(enemyRightTotalHP, ownRightTotalHP, enemyRightTotalAttack, ownRightTotalAttack);
                }
                else
                { // Item
                    ((ItemCard *)&option)->calculateUseScore();
                }
            }

            std::sort(cardOptions.begin(), cardOptions.end());
            for (const auto &option : cardOptions)
            {
                if (option.lane() != NOT_IN)
                    ((CreatureCard *)&option)->summon(actions, option.lane());
                else
                    ((ItemCard *)&option)->use(actions, -1);
            }

            // Use Item
            // TODO

            // Attack
            // Now with simple greedy strategy
            if (board[2].size() == 0)
            {
                for (CreatureCard &creature : board[0])
                {
                    creature.attackTo(actions, opponent);
                }
            }
            else
            {
                for (CreatureCard &creature : board[0])
                {
                    creature.attackTo(actions, board[2][0]);
                }
            }
            if (board[3].size() == 0)
            {
                for (CreatureCard &creature : board[1])
                {
                    creature.attackTo(actions, opponent);
                }
            }
            else
            {
                for (CreatureCard &creature : board[1])
                {
                    creature.attackTo(actions, board[3][0]);
                }
            }
        }
        // do actions
        if (actions.size())
            std::cout << actions << std::endl;
        else
            std::cout << "PASS" << std::endl;
    }
    return 0;
}
