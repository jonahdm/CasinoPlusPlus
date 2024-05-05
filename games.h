#ifndef GAMES
#define GAMES

#include <algorithm>
#include <iostream>
#include <random>
#include <set>
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
    

    int action_select() {
        std::cout << "\n\nSelect your next move:\n";

        for (int i = 1; i <= actions.size(); ++i) {
            std::cout << i << ": " << actions[i-1] << "\n";
        };

        std::string choice;
        do {
            std::cin >> choice;
            if (!std::isdigit(choice[0]) || (std::stoi(choice) > actions.size() || (std::stoi(choice) == 0))) { // If input is not a digit OR the input digit is outside range
                std::cout << "Not a valid choice, partner. Try again.\n";
            };
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } while(!std::isdigit(choice[0]) || (std::stoi(choice) > actions.size() || (std::stoi(choice) == 0)));    

        return std::stoi(choice) - 1;
    };
};

class BlackJack : public Game{
    public:
        using Game::Game;
        Deck deck;
        std::vector<Deck> player_hands;
        std::vector<int> player_scores;
        std::set<int> bust_players = {};
        std::default_random_engine rng;

    BlackJack(std::vector<std::string> player_names, std::vector<Inventory> player_inventories) : Game (player_names, player_inventories) {
        
        actions = {"Hit", "Stand", "Double", "Fold"};

        for (int i = 0;  i < player_names.size();  ++ i){
            Deck new_deck;
            player_hands.push_back(new_deck);
            player_scores.push_back(0);
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
            Deck& curr_deck = player_hands[i];            
            curr_deck.add_top_deck(deck.draw_n_cards(2));

            if (i == 0){
                std::cout << "Your hand: " << curr_deck.display_contents() << "\n";
            } else {
                std::cout << player_names[i] << "'s hand is: " << curr_deck.display_contents() << "\n";
            }
        };

        int curr_player_id = 0;
        while (!game_over) {
            bool curr_player_active = true;

            while (curr_player_active){
                curr_player_active = next_move(player_hands[curr_player_id]);
            }
            if (curr_player_id < player_names.size() - 1){
                curr_player_id ++;
            } else {
                game_over = true;
            }
        }
    };

    bool next_move(Deck& curr_hand) {
        std::string selection = actions[action_select()];
        std::cout << selection << "\n";
        bool has_legal_moves = true;

        if (selection == "Hit") {
            hit(curr_hand, deck);
        } else if (selection == "Stand") {

        } else if (selection == "Double") {

        } else if (selection == "Split") {

        } else if (selection == "Fold") {

        }

        int curr_player_score = curr_hand.get_total_value();

        if (curr_player_score > 21){
            bust();
            has_legal_moves = false;
        } else if (curr_player_score == 21){
            has_legal_moves = false;
        };

        return has_legal_moves;
    };

    void hit(Deck& curr_hand, Deck& curr_deck) {
        std::cout << "you hit! Curr_hand is: " << curr_hand.display_contents() << "\n Curr deck is : " << curr_deck.display_contents() << "\n"; 
        curr_hand.add_top_card(curr_deck.draw_top_card());
        std::cout << "you hit! Curr_hand is: " << curr_hand.display_contents() << "\n Curr deck is : " << curr_deck.display_contents() << "\n"; 

        //return curr_hand;
    };

    void stand() {

    };

    Deck double_down(Deck curr_hand){

    };

    void split() {

    };

    void fold() {

    };

    void bust() {

    };

};
#endif //GAMES