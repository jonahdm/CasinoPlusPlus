#include <iostream>
#include <vector>
#include <string>
#include <locale.h>
#include <time.h> 

#include "games.h"
#include "cards.h"
#include "inventories.h"
#include "players.h"

int main()
{    
    Player p("Player", "human", 500);
    Player d("Dean", "dealer", 1000);
    Player a("Alan", "player", 500); 
    Player b("Bob", "player", 500);
    std::vector<Player *> players = {&p , &d, &a, &b};

    setlocale(LC_ALL, ".UTF-8"); // Needed to properly display Unicode in VSCode Terminal
    std::locale::global(std::locale(".UTF-8")); // Needed to properly display Unicode in VSCode Terminal

    std::cout << "Welcome to the casino.\n"
    "Enter a digit to pick your poison:\n\n"
    "1: Blackjack\n"
    "2: Exit.\n\n";

    
    std::string choice;
    do {
        std::cin >> choice;
        if (!std::isdigit(choice[0]) || (std::stoi(choice) > 2) || (std::stoi(choice) < 1)){ // If input is not a digit OR the input digit is outside range
            std::cout << "Not a valid choice, partner. Try again.\n";
        };
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while(!std::isdigit(choice[0]) || (std::stoi(choice) > 2) || (std::stoi(choice) < 1));  

    int choice_switch = stoi(choice);

    switch(choice_switch) {
        case 1: {
            BlackJack game(players);
            break;
        }
        case 2: {
            std::cout << "Come back when you've got something to lose.\n";
            break;
        }
    }   

    return true;
};