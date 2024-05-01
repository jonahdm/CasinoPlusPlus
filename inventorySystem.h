#ifndef INVENTORY_SYSTEM
#define INVENTORY_SYSTEM

#include <iostream>
#include <string>
#include <vector>

struct Item {
    int id;
    int value;
};

class Inventory{
    public:
        int money;
        std::vector<Item> itemList;
        
    Inventory() {money = 0;}
    Inventory(int m) {money = m;}

    void add_item(Item i){
        itemList.push_back(i);
    };

    Inventory operator+=(const int& n){
        this -> money += n;
        return  *this;
    };

    Inventory operator-=(const int& n){
        this -> money -= n;
        return  *this;
    };
};

#endif //INVENTORY_SYSTEM