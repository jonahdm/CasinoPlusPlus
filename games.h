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
        //Todo: Make a player class and update these vectors calls
        std::vector<std::string> player_names;
        std::vector<Inventory> player_inventories;

        bool game_over = false;
        int money_pool = 0;
        int winner_id;
    
    Game (std::vector<std::string> player_names, std::vector<Inventory> player_inventories):
            player_names(player_names),
            player_inventories(player_inventories)
            {}     
    

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
        using Game::Game;
        Deck deck;
        std::vector<Deck> player_decks;

        std::default_random_engine rng;

    BlackJack(std::vector<std::string> player_names, std::vector<Inventory> player_inventories) : Game (player_names, player_inventories) {
        
        actions = {"Hit", "Stand", "Double", "Fold"};

        for (int i = 0; i < player_names.size(); ++ i){
            Deck new_deck;
            player_decks.push_back(new_deck);
        };

        srand(time(NULL)); //Initialize random number generator
        std::random_device rd; 
        rng = std::default_random_engine { rd() };
    }

    void initialize() {
        
        std::cout << "\n\033[1;30m♠\033[0m\033[1;31m♥\033[0m\033[1;31m♦\033[0m\033[1;30m♣\033[0m "
        "Beginning a game of Blackjack. "
        "\033[1;30m♠\033[0m\033[1;31m♥\033[0m\033[1;31m♦\033[0m\033[1;30m♣\033[0m\n\n";


        Inventory& main_player_inv = player_inventories[0];

        std::cout << "You currently have: $" << main_player_inv.money << "\n";

        deck.build_standard_deck();
        deck.shuffle_deck(rng);


        for (int i = 0; i < player_names.size(); ++ i){
            Deck& curr_deck = player_decks[i];            
            curr_deck.add_top_deck(deck.draw_n_cards(2));

            if (i == 0){
                std::cout << "Your hand is currently: " << curr_deck.display_contents() << "\n";
            } else {
                std::cout << player_names[i] << "'s hand is currently: " << curr_deck.display_contents() << "\n";
            }
        };

        action_select();
    };

    bool check_game_end(Deck& hand) {
        bool end = false;

        return end;
    };
    
    void player_bust() {

    };

    void dealer_bust() {

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