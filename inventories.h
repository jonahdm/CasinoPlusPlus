#ifndef INVENTORIES
#define INVENTORIES

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

    Inventory(): money(0), itemList({}) {}
    Inventory(int m): money(m), itemList({}) {}

    void add_item(Item i){
        itemList.push_back(i);
    }

    Inventory operator+=(int n){
        money += n;
        return  *this;
    }

    Inventory operator-=(int n){
        money -= n;
        return  *this;
    }
};

#endif //INVENTORIES