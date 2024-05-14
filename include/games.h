#ifndef GAMES
#define GAMES

#include <algorithm>
#include <cmath>
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
        std::vector<int> inactive_ids;
        std::vector<std::string> actions;

        bool keep_playing = true; // Whether or not to keep playing a specific type of game (ie BlackJack)
    
    Game (std::vector<Player*> players): players(players) {}     
    
    // Prompts the user to select from an option from the class member actions.
    int action_select() {
        for (int i = 1; i <= actions.size(); ++i) {
            std::cout << i << ": " << actions[i-1] << "\n";
        }

        std::string choice;
        int ichoice = -1;
        do {
            std::cin >> choice;
            if (!std::isdigit(choice[0])) { // If the input isn't a number
                std::cout << "You need to bet a NUMBER. Try again.\n";
            } else {
                ichoice = stoi(choice);
                if ((ichoice  > actions.size() || (ichoice  == 0))) { // If input isn't a number OR the input digit is outside acceptable range
                    std::cout << "Not a valid choice, partner. Try again.\n";
                }
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } while(!std::isdigit(choice[0]) || (ichoice > actions.size() || (ichoice == 0)));   

        return ichoice  - 1;
    }

    // Prompt the user to make a bet
    int make_bet(Player* curr_player, int min_bet = 10, int max_bet = 500){
        
        int ibet = -1;
        int* curr_money = &curr_player->inventory.money;

        if (curr_player->type == 0){
            std::cout << "You have $" << *curr_money << "\n";
            std::cout << "Please enter your bet amount, at least $" << min_bet << " and up to $" << max_bet << "\nYou can enter 0 to pass this round.\n";
            
            std::string bet;
            do {
                std::cin >> bet;
                if (!std::isdigit(bet[0])) { // If the input isn't a number
                    std::cout << "You need to bet a NUMBER. Try again.\n";
                } else {
                    ibet = stoi(bet);
                    if (ibet > *curr_money){
                        std::cout << "You can't bet more than you got! Try again.\n";
                    }
                    else if (((ibet < min_bet || ibet < 0) && (ibet != 0)) || ((ibet > max_bet))){ 
                        std::cout << "That bet ain't right. Try again.\n";
                    }
                }
            }while (!std::isdigit(bet[0]) || (ibet > *curr_money) || ((ibet < min_bet || ibet < 0) && (ibet != 0)) || ((ibet > max_bet)));
            
            if (ibet > 0){
                std::cout <<  "You bet $" << ibet << "\n";
                curr_player -> inventory -= ibet;
                curr_player -> current_bet += ibet;

            } else {
                std::cout <<  "You pass this round.\n";
            }

        } else {
            ibet = min_bet; // This will be updated to have computers make a semi-random bet based on their "personality"
            
            if (ibet > 0){
                std::cout <<  curr_player -> name << " bets $" << ibet << "\n";
                curr_player -> inventory -= ibet;
                curr_player -> current_bet += ibet;

            } else {
                std::cout << curr_player -> name << " passes this round.\n";
            }
        }

        return ibet;
    }

    void win_bet(Player* player, int odds){
        player->inventory += (player->current_bet * odds);
        player->current_bet = 0;
    }

    void lose_bet(Player* player){
        player->current_bet = 0;
    }

    void tie_bet(Player* player){
        player->inventory += player->current_bet;
        player->current_bet = 0;
    }
};

class BlackJack : public Game{
    public:
        using Game::Game;
        Deck deck;
        std::default_random_engine rng;

        int dealer_score;

    BlackJack(std::vector<Player*> players) : Game (players) {
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
        do {
            keep_playing = new_game();
        } while (keep_playing == true);

        std::cout << "\nThis game's over, pal.";
    }

    bool new_game() {

        //Initialize random number generator
        srand(time(NULL)); 
        std::random_device rd; 
        rng = std::default_random_engine { rd() };

        // Establish the first set of viable actions in the game
        actions = {"Hit", "Stand", "Double", "Surrender"};

        // Output new game text
        std::cout << "\n♠♥♦♣ Beginning a new game of Blackjack ♠♥♦♣\n";

        // // ANSI Escape Codes can be enabled so that text output has color, but this is fragile and dependent on OS
        // std::cout << "\n\033[1;30m♠\033[0m\033[1;31m♥\033[0m\033[1;31m♦\033[0m\033[1;30m♣\033[0m "
        // "Beginning a new game of Blackjack. "
        // "\033[1;30m♠\033[0m\033[1;31m♥\033[0m\033[1;31m♦\033[0m\033[1;30m♣\033[0m\n\n";

        // Create the standard deck of cards
        deck.build_standard_deck();
        deck.shuffle_deck(rng);

        Player* dealer;
        for (int i = 0; i < players.size(); ++ i){
            
            if (players[i]-> type == 2){ // Checks if currenty player is the dealer
                    Deck new_deck;
                    players[i] -> hand = new_deck;
                    
                    players[i] -> hand.add_top_deck(deck.draw_n_cards(1)); // Dealer only gets 1 card to start
                    dealer = players[i]; // Create a reference to the dealer for later
                    dealer->current_score = dealer->hand.get_total_value_blackjack();
            } else {
                int bet = make_bet(players[i]);
                if (bet > 0){
                    Deck new_deck;
                    players[i] -> hand = new_deck;
                    players[i] -> hand.add_top_deck(deck.draw_n_cards(2)); // Deal two from the top
                    players[i] ->current_score = players[i]->hand.get_total_value_blackjack();
                } else {
                        inactive_ids.push_back(i);
                }
            }
        }

        dealer_score = dealer -> hand.get_total_value_blackjack(); // Get the score from the last player, who should always be the dealer.

        std::cout << "\n";
        display_game_state();

        // Players in the game make their decisions in play order until they choose to stop or bust
        for (int i = 0; i < players.size(); ++ i){
            if (!(std::find(inactive_ids.begin(), inactive_ids.end(), i) != inactive_ids.end())){ // If the current player id is not in the inactive list
                bool first_move = true;          
                bool curr_player_active = true;
                do {
                    if (first_move == true){
                        if (players[i]->current_score == 21){
                            // 21 on the first hand is a BlackJack, which pays 3:2 odds.
                            std::cout << "\n" << players[i]->name << " Has BlackJack! ";
                            players[i]->current_bet =  round((players[i]->current_bet) * 3/2);
                            curr_player_active = false;
                            continue;
                        } else {
                            std::cout << "\n" << players[i]->name << "'s turn! ";
                            display_current_player_hand(*players[i]);
                            first_move = false;
                        }
                    }
                    curr_player_active = next_move(players[i]);
                } while (curr_player_active == true);
            }
        }

        dealer_score = dealer -> hand.get_total_value_blackjack(); // Get the score from the last player, who should always be the dealer.
        std::cout << "\n";
        display_game_state();
        std::cout << "\n";

        // For each active player, check their score against the dealer's to determine win/loss/tie
        for (int i = 0; i < players.size() - 1; ++ i){ // size() - 1 because we don't score against the dealer.
            if (!(std::find(inactive_ids.begin(), inactive_ids.end(), i) != inactive_ids.end())){ // If the current player id is not in the inactive list OR current player is not the dealer.
                Player* curr_player = players[i];
                int player_score = curr_player->current_score;

                if ((player_score > 21) || (player_score < dealer_score) && (dealer_score <= 21)){ // If the player busted, or their score was less than the dealer's
                    std::cout << curr_player->name << " Loses $" << curr_player->current_bet << "!\n";
                    lose_bet(curr_player);
                } else if ((player_score > dealer_score) || (player_score < dealer_score) && (dealer_score > 21)){
                    std::cout << curr_player->name << " Wins $" << curr_player->current_bet << "!\n";
                    win_bet(curr_player, 2); // A normal win returns 2:1 odds
                } else if (player_score == dealer_score){
                    std::cout << curr_player->name << " ties with the dealer! $" << curr_player->current_bet << " returned!\n";
                    tie_bet(curr_player);
                }
            }
        }   
        
        actions = {"Yes", "No"};
        std::cout << "\nContinue playing?\n";
        int choice = action_select() + 1;
        bool keep_playing = false;
        switch(choice){
            case 1:
                keep_playing = true;
                break;
            case 2:
                keep_playing = false;
                break;
        }

        return keep_playing;
    }

    bool next_move(Player* curr_player) {
        bool has_legal_moves = true;
        std::string selection;

        switch(curr_player->type){
            case 0:
                std::cout << "\nSelect your next move:\n";
                selection = actions[action_select()];
                break;
            case 1:
                selection = player_choose_move(*curr_player);
                break;
            case 2:
                selection = dealer_choose_move(*curr_player);
                break;
        }

        if (selection == "Hit") {
            std::cout << curr_player->name << " Hits! ";
            hit(curr_player->hand, deck);
        } else if (selection == "Stand") {
            std::cout << curr_player->name << " Stands! ";
            has_legal_moves = false;
        } else if (selection == "Double") {
            curr_player->inventory -= curr_player->current_bet;
            curr_player->current_bet += curr_player->current_bet;
            std::cout << curr_player->name << " Doubles! Their new bet is: $" << curr_player->current_bet << "\n";

            hit(curr_player->hand, deck);
            has_legal_moves = false;

        } else if (selection == "Split") {
            std::cout << curr_player->name << " Splits! ";

        } else if (selection == "Surrender") {
            std::cout << curr_player->name << " Surrenders! ";
            has_legal_moves = false;
        }

        display_current_player_hand(*curr_player);
        curr_player->current_score = curr_player->hand.get_total_value_blackjack();

        if (curr_player->current_score > 21){
            std::cout << curr_player->name << " Busts!\n";
            has_legal_moves = false;
        } else if (curr_player->current_score == 21){
            std::cout << curr_player->name << " Has 21!\n";
            has_legal_moves = false;
        } else {
        }

        return has_legal_moves;
    }

    void hit(Deck& curr_hand, Deck& curr_deck) {
        curr_hand.add_top_card(curr_deck.draw_top_card());
    }

    void split() {

    }

    void display_game_state() {
        for (int i = 0; i < players.size(); ++ i){
            if (!(std::find(inactive_ids.begin(), inactive_ids.end(), i) != inactive_ids.end())){ // If the current player id is not in the inactive list
                if (players[i] -> type == 0){
                    std::cout << "Your hand is: " << players[i]->hand.get_contents_display() << "\n";
                } else if (players[i] -> type == 2){
                    std::cout << "Dealer " << players[i]->name << "'s hand is: " << players[i]->hand.get_contents_display() << "\n";
                } else {
                    std::cout << players[i]->name << "'s hand is: " << players[i]->hand.get_contents_display() << "\n";
                }
            }
        }
    }

    void display_current_player_hand(Player& curr_player) {
        std::cout << curr_player.name << "'s Hand is: " << curr_player.hand.get_contents_display() << "\n";
    }

    std::string player_choose_move(Player& curr_player) { // Reference to player here for future implementation of stats to weight decisions
        int curr_score = curr_player.current_score;
        if ((curr_score <= 16)){
            if ((dealer_score >= 7 || curr_score <= 12) ||
                (dealer_score <= 6 && curr_score <= 11) ||
                (dealer_score <= 3 && curr_score <= 12)) {
                 return "Hit";
            } else {
                return "Stand";
            }
        } else {
            return "Stand";
        }
    }

    std::string dealer_choose_move(Player& curr_player) { // Reference to player here for future implementation of stats to weight decisions
        int curr_score = curr_player.current_score;
        if (curr_score <= 16){
            return "Hit";
        } else {
            return "Stand";
        }
    }

};
#endif //GAMES