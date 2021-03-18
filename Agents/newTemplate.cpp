#include <iostream>
#include <string>

class Card{
public:
    Card(int cardNumber, int instanceId, int location, int cardType, int cost, int atk, int def, std::string abilities,int myhealthChange, int opponentHealthChange, int cardDraw, int lane):cardNumber_(cardNumber), instanceId_(instanceId), location_(location), cardType_(cardType), cost_(cost), attack_(atk), defense_(def), abilities_(abilities),myhealthChange_(myhealthChange), opponentHealthChange_(opponentHealthChange), cardDraw_(cardDraw), lane_(lane){};
    int instanceID() const{return instanceId_;};
    int location() const{return location_;};
    int cost() const{return cost_;};
    int attack() const{return attack_;};
    int defense() const{return defense_;};
    int lane() const{return lane_;};
    std::string abilities() const{return abilities_;};
    void atkDiff(int atk){attack_ += atk;};
    void defDiff(int def){defense_ += def;};
private:
    int cardNumber_, instanceId_, location_, cardType_, cost_, attack_, defense_;
    int myhealthChange_, opponentHealthChange_, cardDraw_, lane_;
    std::string abilities_;
};

class CreatureCard : public Card {
public:
    CreatureCard(int cardNumber, int instanceId, int location, int cardType, int cost, int atk, int def, std::string abilities,int myhealthChange, int opponentHealthChange, int cardDraw, int lane):Card(cardNumber, instanceId, location, cardType, cost, atk, def, abilities, myhealthChange, opponentHealthChange, cardDraw,  lane){};
    void summon(std::string &action, int lane) {
        action += "SUMMON " + std::to_string(instanceID()) + " " + std::to_string(lane) + ";";
    };
    bool canKill(const CreatureCard &c){return  c.defense() <= this->attack();};
    void attackTo(std::string &action, CreatureCard &target){
        target.defDiff(this->attack());
      this->defDiff(target.attack());
      action += "ATTACK " + std::to_string(this->instanceID()) + " " + std::to_string(target.instanceID()) + ";";
    };
private:
};

class ItemCard : public Card {
public:
    ItemCard(int cardNumber, int instanceId, int location, int cardType, int cost, int atk, int def, std::string abilities,int myhealthChange, int opponentHealthChange, int cardDraw, int lane):Card(cardNumber, instanceId, location, cardType, cost, atk, def, abilities, myhealthChange, opponentHealthChange, cardDraw,  lane){};
    void use(std::string &action, int target){
        action += "USE " + std::to_string(instanceID()) + " " + std::to_string(target) + ";";
    };
private:
};

int main() {
    std::string action;
    CreatureCard c(1, 1, 1, 1, 1, 1, 1, "G", 1, 1, 1, 1), d(1, 2, 1, 1, 1, 1, 1, "G", 1, 1, 1, 1);
    c.summon(action, 0);
    d.summon(action, 1);
    std::cout << c.canKill(d) << std::endl;
    c.attackTo(action, d);

    ItemCard i(1, 1, 1, 1,1, 1, 1, "G", 1, 1, 1, 0);
    i.use(action, d.instanceID());

    std::cout << action << std::endl;
    return 0;
}

