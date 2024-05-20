#ifndef CHARACTERS
#define CHARACTERS

#include <string>
#include <vector>

#include "cards.h"
#include "inventories.h"


struct CharacterHand {
    Deck cards;
    int bet;
    int score;

    bool surrendered = false;
    bool blackJack = false;

    // CharacterHand(): bet(0), score(0) {}

    CharacterHand(int bet = 0): bet(bet), score(0) {}
    CharacterHand(Deck cards): cards(cards), bet(0), score(0) {}
    CharacterHand(Deck cards, int bet): cards(cards), bet(bet), score(0) {}

};

class Character {
    public:
        std::string name;
        Inventory inventory;
        int type; //0 = Human, 1 = Computer Character, 2 = Computer Dealer
        std::vector<CharacterHand> hands;

    Character(std::string name, int type, int money = 0): name(name), type(type), inventory(Inventory(money)) {}

    Character(std::string name, std::string p_type = "computer", int money = 0): name(name), inventory(Inventory(money)) {
        std::transform(p_type.begin(), p_type.end(), p_type.begin(), std::tolower);
        if (p_type == "human"){
            type = 0;
        } else if (p_type == "computer"){
            type = 1;
        } else if (p_type == "dealer"){
            type = 2;
        } else { // Default new characters to computer characters
            type = 1;
        }

    }

    int get_current_money(){
        return inventory.money;
    }
};



#endif //CHARACTERS