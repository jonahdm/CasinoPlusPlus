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

    void operator+=(Item i){
        itemList.push_back(i);
    }

    void operator+=(int n){
        money += n;
    }

    void operator-=(int n){
        money -= n;
    }
};

#endif //INVENTORIES