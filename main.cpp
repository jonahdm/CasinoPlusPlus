#include <iostream>
#include <vector>
#include <string>
#include <locale.h>
#include <time.h> 

#include "games.h"
#include "cards.h"
#include "inventorySystem.h"1

int main()
{    

    Inventory p_inv(500);
    Inventory d_inv(1000);
    std::vector<std::string> player_names = {"Player", "Dealer"};
    std::vector<Inventory> player_inventories = {p_inv, d_inv};

    setlocale(LC_ALL, ".UTF-8"); // Needed to properly display Unicode in VSCode Terminal
    std::locale::global(std::locale(".UTF-8")); // Needed to properly display Unicode in VSCode Terminal

    std::cout << "Welcome to the casino.\n"
    "Don't stress about the time.\n"
    "Enter a digit to pick your poison:\n"
    "1: Blackjack\n"
    "2: Exit.\n\n";

    std::string choice;
    do {
        std::cin >> choice;
        if (!std::isdigit(choice[0]) || (std::stoi(choice) >= 2) || (std::stoi(choice) < 1)){ // If input is not a digit OR the input digit is outside range
            std::cout << "Not a valid choice, partner. Try again.\n";
        };
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while(!std::isdigit(choice[0]) || (std::stoi(choice) >= 2) || (std::stoi(choice) < 1));  

    int choice_switch = stoi(choice);
    switch(choice_switch) {
        case 1: {
            BlackJack game(player_names, player_inventories);
            game.initialize();
            break;
        };
        case 2:
            std::cout << "Come back when you've got something to lose.\n";
            break;
    };

    return true;
};