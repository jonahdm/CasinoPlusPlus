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
#include "inventories.h"
#include "players.h"

class Game {
    public:
        std::vector<Player*> players;
        std::vector<std::string> actions;

        bool game_over = false; // Whether or not to continue the individual instance of a game
        bool keep_playing = true; // Whether or not to keep playing a specific type of game (ie BlackJack)

        int money_pool;
    
    Game (std::vector<Player*> players): players(players) {}     
    

    int action_select() {
        std::cout << "\n\nSelect your next move:\n";

        for (int i = 1; i <= actions.size(); ++i) {
            std::cout << i << ": " << actions[i-1] << "\n";
        }

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
    }
};

class BlackJack : public Game{
    public:
        using Game::Game;
        Deck deck;
        std::default_random_engine rng;

    BlackJack(std::vector<Player*> players) : Game (players) {
        
        //Initialize random number generator
        srand(time(NULL)); 
        std::random_device rd; 
        rng = std::default_random_engine { rd() };

        // Ensure that there is only one Dealer, and that they are last in the turn order
        bool dealer_found = false;
        for (int i = 0;  i < players.size();  ++ i){
            
            if ((players[i] -> type == 2) && !(i == players.size() - 1) && (dealer_found == false)) { // If the player is a dealer, and isn't at the end of the turn order, and a dealer hasn't already been found
                std::rotate(players.begin() + i, players.begin() + (i + 1) , players.end());
                dealer_found = true;
            } else if ((players[i] -> type == 2)  && !(i == players.size() - 1) && (dealer_found == true)) { // BlackJack can only have one dealer, so any found after the first are set to normal players
                players[i] -> type = 1;
            }
            if ((i == players.size() - 1) && (dealer_found == false)){ // If no dealer was found, set the last player as the dealer.
                players[i] -> type = 2;
            }
        }

        this -> players = players; // Makes the player order re-arrangement permanent

        while (keep_playing == true){
            start_new_game();
        }

        std::cout << "This game's over, pal.";
    }

    void start_new_game() {
        // Establish the first set of viable actions in the game
        actions = {"Hit", "Stand", "Double", "Fold"};


        // Draw each player a hand of 2 cards 
        for (int i = 0;  i < players.size();  ++ i){
            Deck new_deck;
            players[i] -> hand = new_deck;
            players[i] -> score = 0;
        }
        
        // Print out pretty colored text
        std::cout << "\n\033[1;30m♠\033[0m\033[1;31m♥\033[0m\033[1;31m♦\033[0m\033[1;30m♣\033[0m "
        "Beginning a new game of Blackjack. "
        "\033[1;30m♠\033[0m\033[1;31m♥\033[0m\033[1;31m♦\033[0m\033[1;30m♣\033[0m\n\n";


        // Create the standard deck of cards and give each player 2 from the top
        deck.build_standard_deck();
        deck.shuffle_deck(rng);
        for (int i = 0; i < players.size(); ++ i){            
            players[i] -> hand.add_top_deck(deck.draw_n_cards(2));
        }

        display_game_state();

        // Players in the game make their decisions in play order until they hold or are out
        for (int i = 0; i < players.size(); ++ i){          
            bool curr_player_active = true;
            while (curr_player_active){
                curr_player_active = next_move(players[i] -> hand);
            }
        }
    }

    bool next_move(Deck& curr_hand) {
        std::string selection = actions[action_select()];
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
        }

        return has_legal_moves;
    }

    void hit(Deck& curr_hand, Deck& curr_deck) {
        curr_hand.add_top_card(curr_deck.draw_top_card());
    }

    void stand() {

    }

    void double_down(){

    }

    void split() {

    }

    void fold() {

    }

    void bust() {

    }

    void display_game_state() {
        for (int i = 0; i < players.size(); ++ i){
            if (players[i] -> type == 0){
                std::cout << "Your hand is: " << players[i]->hand.get_contents_display() << " Your pot is: " << players[i]->inventory.money << "\n";
            } else if (players[i] -> type == 2){
                std::cout << "Dealer " << players[i]->name << "'s hand is: " << players[i]->hand.get_contents_display() <<  " Dealer's pot is: " << players[i]->inventory.money << "\n";
            } else {
                std::cout << players[i]->name << "'s hand is: " << players[i]->hand.get_contents_display() << players[i]->name << "'s pot is: " << players[i]->inventory.money<< "\n";
            }
        }
    }

};
#endif //GAMES