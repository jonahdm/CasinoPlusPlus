#ifndef CHARACTERS
#define CHARACTERS

#include <string>

#include "cards.h"
#include "inventories.h"

class Player {
    public:
        std::string name;
        Inventory inventory;
        int type; //0 = Human Player, 1 = Computer Player, 2 = Computer Dealer

        Deck hand;
        int current_bet = 0;
        int current_score = 0;

    Player(std::string name, int type, int money = 0): name(name), type(type), inventory(Inventory(money)) {
    }

    Player(std::string name, std::string p_type = "computer", int money = 0): name(name), inventory(Inventory(money)) {
        std::transform(p_type.begin(), p_type.end(), p_type.begin(), std::tolower);
        if (p_type == "human"){
            type = 0;
        } else if (p_type == "computer"){
            type = 1;
        } else if (p_type == "dealer"){
            type = 2;
        } else { // Default new players to computer players
            type = 1;
        }
    }

    int get_current_money(){
        return inventory.money;
    }
};


#endif //CHARACTERS