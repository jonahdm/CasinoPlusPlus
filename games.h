#ifndef GAMES
#define GAMES

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <time.h> 
#include <vector>


#include "cards.h"
#include "inventorySystem.h"

class Game {
    public:
        std::vector<std::string> actions;

    void action_select() {
        std::cout << "\n\nSelect your next move:\n";

        for (int i = 0; i < actions.size(); ++i) {
            std::cout << i << ": " << actions[i] << "\n";
        };

        std::string choice;
        do {
            std::cin >> choice;
            if (!std::isdigit(choice[0]) || (std::stoi(choice) >= actions.size())){ // If input is not a digit OR the input digit is outside range
                std::cout << "Not a valid choice, partner. Try again.\n";
            };
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } while(!std::isdigit(choice[0]) || (std::stoi(choice) >= actions.size()));    
    };
};

class BlackJack : public Game{
    public:
        Deck deck;
        Deck player_hand;
        Deck dealer_hand;

        Inventory player_inv;
        Inventory dealer_inv;

    BlackJack(Inventory p_inv, Inventory d_inv) {
        player_inv = p_inv,
        dealer_inv = d_inv,
        actions = {"Hit", "Stand", "Double", "Fold"};
    }

    void initialize() {
        
        std::cout << "\n\033[1;30m♠\033[0m\033[1;31m♥\033[0m\033[1;31m♦\033[0m\033[1;30m♣\033[0m "
        "Beginning a game of Blackjack. "
        "\033[1;30m♠\033[0m\033[1;31m♥\033[0m\033[1;31m♦\033[0m\033[1;30m♣\033[0m\n\n";

        std::cout << "You currently have: $" << player_inv.money << "\n";

        srand(time(NULL)); //Initialize random number generator
        std::random_device rd; 
        auto rng = std::default_random_engine { rd() };

        deck.build_standard_deck();
        deck.shuffle_deck(rng);

        player_hand.add_top_deck(deck.draw_n_cards(2));
        dealer_hand.add_top_deck(deck.draw_n_cards(2));

        std::cout << "Your hand is currently: " << player_hand.display_contents();

        action_select();

    };

    Card hit() {

    };

    void stand() {

    };

    Card double_down(){

    };

    void split() {

    };

    void fold() {

    };

};
#endif //GAMES