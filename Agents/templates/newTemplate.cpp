#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

const int VERSION = 2;

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
    Card(int cardNumber, int instanceId, int location, int cardType, int cost, int atk, int def, std::string abilities, int myhealthChange, int opponentHealthChange, int cardDraw, int lane, int index) : cardNumber_(cardNumber), instanceId_(instanceId), location_(location), cardType_(cardType), cost_(cost), attack_(atk), defense_(def), myhealthChange_(myhealthChange), opponentHealthChange_(opponentHealthChange), cardDraw_(cardDraw), lane_(lane), _index(index)
    {
        for (int i = 0; i < 7; ++i)
        { // the last ability "P" indicates thet if it is the player himself
            if (i < abilities.length() && abilities[i] != '-')
                abilities_[i] = 1;
            else
                abilities_[i] = 0;
        }
    };
    int instanceID() const { return instanceId_; };
    int location() const { return location_; };
    int cardType() const { return cardType_; };
    int cost() const { return cost_; };
    int attack() const { return attack_; };
    int defense() const { return defense_; };
    int lane() const { return lane_; };
    int ability(int index) const { return abilities_[index]; };
    void atkDiff(int atk) { attack_ += atk; };
    void defDiff(int def) { defense_ += def; };
    bool operator<(const Card &rhs) const { return score_ < rhs.score_; };
    bool operator==(const Card &rhs) const { return score_ == rhs.score_; };
    void calculateGetScore()
    {
        score_ = (double)-attack_ / cost_;
    }
    void pick(std::string &action) const
    {
        action += "PICK " + std::to_string(_index);
    }
    void getDamaged(int amount)
    {
        if (amount > 0)
        {
            if (abilities_[5])
                abilities_[5] = 0;
            else
                defDiff(abilities_[4] ? -999 : -amount);
        }
        else
            defDiff(-amount);
    }

private:
    int cardNumber_, instanceId_, location_, cardType_, cost_, attack_, defense_;
    int myhealthChange_, opponentHealthChange_, cardDraw_, lane_, _index;
    int abilities_[7]; // BCDGLWP
    double score_ = -1;
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
        target.getDamaged(this->attack());
        getDamaged(target.attack());
        action += "ATTACK " + std::to_string(this->instanceID()) + " " + std::to_string(target.instanceID()) + ";";
    };
    void calculateUseScore(int enemyTotalHP, int ownTotalHP, int enemyTotalAttack, int ownTotalAttack, int playerHP, int enemyHP)
    {
        // score_ = (double)-attack_ / cost_;
        score_ = {{exprs[0]}};
    }
    void calculateAttackScore(CreatureCard &attacker)
    {
        if (abilities_[6]) // if there are no cards with Guard, attack the player first.
            score_ = -500;
        else if (attacker.ability(4)) // if it has Lethal, kill the one with the most health
            score_ = -defense_;
        else
        { // if it can kill, kill the one with the most health, if not, attack the one with the least health
            score_ = attacker.attack() - (abilities_[5] ? 0 : defense_);
            if (score_ < 0)
                score_ = defense_;
        }
        if (abilities_[3]) // the one with Guard needs to be damaged first
            score_ -= 1000;
    }

private:
};

class ItemCard : public Card
{
public:
    ItemCard(int cardNumber, int instanceId, int location, int cardType, int cost, int atk, int def, std::string abilities, int myhealthChange, int opponentHealthChange, int cardDraw, int lane, int index) : Card(cardNumber, instanceId, location, cardType, cost, atk, def, abilities, myhealthChange, opponentHealthChange, cardDraw, lane, index){};
    void use(std::string &action, CreatureCard &target)
    {
        target.atkDiff(attack_);
        target.getDamaged(-defense_);
        action += "USE " + std::to_string(instanceID()) + " " + std::to_string(target.instanceID()) + ";";
    };

    void calculateUseScore()
    {
        score_ = (double)-attack_ / cost_;
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
        int enemyTotalHP[2] = {0}, enemyTotalAttack[2] = {0}, ownTotalHP[2] = {0}, ownTotalAttack[2] = {0};
        // game information
        int playerHealth, playerMana, playerDeck, playerRune, playerDraw;
        int opponentHealth, opponentMana, opponentDeck, opponentRune, opponentDraw;

        std::cin >> playerHealth >> playerMana >> playerDeck >> playerRune >> playerDraw;
        std::cin >> opponentHealth >> opponentMana >> opponentDeck >> opponentRune >> opponentDraw;

        CreatureCard player(-1, -1, MY_SIDE, CREATURE, 0, 0, playerHealth, "------P", 0, 0, 0, -1, -1);
        CreatureCard opponent(-1, -1, OPPONENT_SIDE, CREATURE, 0, 0, opponentHealth, "------P", 0, 0, 0, -1, -1);

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
            int cardNumber, instanceID, location, cardType, cost, attack, defense, myHealthChange, opponentHealthChange, cardDraw, lane = 0;
            std::string abilities;
            std::cin >> cardNumber >> instanceID >> location >> cardType >> cost >> attack >> defense >> abilities >> myHealthChange >> opponentHealthChange >> cardDraw;
            if (VERSION == 2)
                std::cin >> lane;
            std::cin.ignore();

            if (playerMana == 0)
            { // Draw Phase
                Card card(cardNumber, instanceID, IN_HAND, cardType, cost, attack, defense, abilities, myHealthChange, opponentHealthChange, cardDraw, NOT_IN, i);
                cardOptions.push_back(card);
            }
            else
            { // Battle Phase
                if (location == IN_HAND)
                { // in my hand
                    if (cardType == CREATURE)
                    {
                        CreatureCard summonLeft(cardNumber, instanceID, IN_HAND, cardType, cost, attack, defense, abilities, myHealthChange, opponentHealthChange, cardDraw, LEFT, i);
                        cardOptions.push_back(summonLeft);
                        if (VERSION == 2)
                        {
                            CreatureCard summonRight(cardNumber, instanceID, IN_HAND, cardType, cost, attack, defense, abilities, myHealthChange, opponentHealthChange, cardDraw, RIGHT, i);
                            cardOptions.push_back(summonRight);
                        }
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
                    board[2 + lane].push_back(tmp);
                    enemyTotalAttack[lane] += attack;
                    enemyTotalHP[lane] += defense;
                }
                else if (location == MY_SIDE)
                { // in my side of board
                    CreatureCard tmp(cardNumber, instanceID, IN_HAND, CREATURE, cost, attack, defense, abilities, myHealthChange, opponentHealthChange, cardDraw, lane, i);
                    board[lane].push_back(tmp);
                    ownTotalAttack[lane] += attack;
                    ownTotalHP[lane] += defense;
                }
            }
        }
        board[OPPONENTLEFT].push_back(opponent);
        board[OPPONENTRIGHT].push_back(opponent);

        std::string actions;

        // Draw Phase
        if (playerMana == 0)
        {
            for (auto &option : cardOptions)
            {
                option.calculateGetScore();
            }
            std::sort(cardOptions.begin(), cardOptions.end());
            cardOptions[0].pick(actions);
        }
        // Battle Phase
        else
        {
            // Summon
            for (auto &option : cardOptions)
            {
                if (option.cardType() == CREATURE)
                {
                    ((CreatureCard *)&option)->calculateUseScore(enemyTotalHP[option.lane()], ownTotalHP[option.lane()], enemyTotalAttack[option.lane()], ownTotalAttack[option.lane()], playerHealth, opponentHealth);
                }
                else
                { // Item
                    ((ItemCard *)&option)->calculateUseScore();
                }
            }

            std::sort(cardOptions.begin(), cardOptions.end());
            for (const auto &option : cardOptions)
            {
                switch (option.cardType())
                {
                case CREATURE:
                    ((CreatureCard *)&option)->summon(actions, option.lane());
                    if (option.ability(1)) //charge
                        board[option.lane()].push_back(*(CreatureCard *)&option);
                    break;

                // TODO better strategy
                case GREENITEM:
                    if (board[0].size() > 0)
                        ((ItemCard *)&option)->use(actions, board[0][0]);
                    else if (board[1].size() > 0)
                        ((ItemCard *)&option)->use(actions, board[1][0]);
                    break;

                case REDITEM:
                    if (board[2].size() > 0)
                        ((ItemCard *)&option)->use(actions, board[2][0]);
                    else if (board[3].size() > 0)
                        ((ItemCard *)&option)->use(actions, board[3][0]);
                    break;

                case BLUEITEM:
                    ((ItemCard *)&option)->use(actions, opponent);
                    break;
                }
            }

            // Attack
            for (int i = 0; i < 2; i++)
            {
                for (CreatureCard &creature : board[i])
                {
                    auto &targets = board[2 + i];
                    for (CreatureCard &enemy : targets)
                    {
                        enemy.calculateAttackScore(creature);
                    }
                    std::sort(targets.begin(), targets.end());
                    std::reverse(targets.begin(), targets.end());
                    creature.attackTo(actions, targets[targets.size() - 1]);
                    if (targets[targets.size() - 1].defense() <= 0)
                        targets.pop_back();
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