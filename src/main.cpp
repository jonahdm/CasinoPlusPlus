#include <iostream>
#include <vector>
#include <string>
#include <locale.h>
#include <time.h> 

#include "..\include\games.h"
#include "..\include\cards.h"
#include "..\include\inventories.h"
#include "..\include\characters.h"

int main()
{
    setlocale(LC_ALL, ".UTF-8"); // Needed to properly display Unicode in VSCode Terminal
    std::locale::global(std::locale(".UTF-8")); // Needed to properly display Unicode in VSCode Terminal

    std::cout << "Welcome stranger. Care to share your name?\n\n";

    std::string player_name;
    std::cin >> player_name;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Create character profiles, incuding the human character's
    Character p(player_name, "human", 500);
    Character d("Dean", "dealer", 1000);
    Character a("Alan", "character", 500); 
    Character b("Bob", "character", 500);
    std::vector<Character *> players = {&p , &d, &a, &b};

    std::cout << 
    "\nWelcome, " << player_name << "\n" <<
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