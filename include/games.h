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
#include "characters.h"

class Game {
    public:
        std::vector<Character*> characters;
        std::vector<Character*> inactive_characters;
        std::vector<std::string> actions;

        bool keep_playing = true; // Whether or not to keep playing a specific type of game (ie BlackJack)
    
    Game (std::vector<Character*> characters): characters(characters) {}     
    
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
    int make_bet(Character* curr_player, int min_bet = 10, int max_bet = 500){
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
                
                CharacterHand new_hand(ibet);
                curr_player -> inventory -= ibet;
                curr_player->hands.push_back(new_hand);

            } else {
                std::cout <<  "You pass this round.\n";
            }

        } else {
            ibet = min_bet; // This will be updated to have computers make a semi-random bet based on their "personality"
            
            if (ibet > 0){
                std::cout <<  curr_player -> name << " bets $" << ibet << "\n";
                CharacterHand new_hand(ibet);
                curr_player -> inventory -= ibet;
                curr_player->hands.push_back(new_hand);

            } else {
                std::cout << curr_player -> name << " passes this round.\n";
            }
        }

        return ibet;
    }

    void win_bet(Character* character, CharacterHand* hand, int odds){
        character->inventory += (hand->bet * odds);
        hand->bet = 0;
    }

    void lose_bet(Character* character, CharacterHand* hand){
        hand->bet = 0;
    }

    void tie_bet(Character* character, CharacterHand* hand){
        character->inventory += hand->bet;
        hand->bet = 0;
    }
};

class BlackJack : public Game{
    public:
        using Game::Game;
        Deck deck;
        std::default_random_engine rng;

        int dealer_score;

    BlackJack(std::vector<Character*> characters) : Game (characters) {
        // Ensure that there is only one Dealer, and that they are last in the turn order
        bool dealer_found = false;
        for (int i = 0;  i < characters.size();  ++ i){
            if ((characters[i] -> type == 2) && !(i == characters.size() - 1) && (dealer_found == false)) { // If the character is a dealer, and isn't at the end of the turn order, and a dealer hasn't already been found
                std::rotate(characters.begin() + i, characters.begin() + (i + 1) , characters.end());
                dealer_found = true;
            } else if ((characters[i] -> type == 2)  && !(i == characters.size() - 1) && (dealer_found == true)) { // BlackJack can only have one dealer, so any found after the first are set to normal characters
                characters[i] -> type = 1;
            }
            if ((i == characters.size() - 1) && (dealer_found == false)){ // If no dealer was found, set the last character as the dealer.
                characters[i] -> type = 2;
            }
        }

        this -> characters = characters; // Makes the character order re-arrangement permanent
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

        Character* dealer;
        CharacterHand* dealer_hand;

        for (Character* curr_player : characters){
            if (curr_player->type == 2){ // Checks if current character is the dealer
                    dealer = curr_player; // Create a reference to the dealer for later
                    Deck new_deck;
                    CharacterHand new_hand(new_deck);
                    curr_player -> hands.push_back(new_hand);

                    dealer_hand = &(curr_player -> hands[0]); // The dealer will only ever have one hand so safe to make this asignment now
                    dealer_hand->cards = new_deck;
                    dealer_hand->cards.add_top_deck(deck.draw_n_cards(1)); // Dealer only gets 1 card to start
                    dealer_hand->score = dealer_hand -> cards.get_total_value_blackjack();
                    dealer_score = dealer_hand -> score; // This will be referenced by computer players for decision making.

            } else {
                int bet = make_bet(curr_player);
                if (bet > 0){
                    Deck new_deck;
                    curr_player -> hands[0].cards = new_deck;
                    curr_player -> hands[0].cards.add_top_deck(deck.draw_n_cards(2)); // Deal two from the top
                    curr_player -> hands[0].score = curr_player -> hands[0].cards.get_total_value_blackjack();
                } else {
                    inactive_characters.push_back(curr_player);
                }
            }
        }

        std::cout << "\n";
        display_game_state();

        // Characters in the game make their decisions in play order until they choose to stop or bust
        for (Character* curr_player : characters){
            if (!(std::find(inactive_characters.begin(), inactive_characters.end(), curr_player) != inactive_characters.end())){ // If the current character id is not in the inactive list
                
                // Turn start message
                if (curr_player->type == 0){
                    std::cout << "\n" <<"Your turn! ";
                } else if (curr_player->type == 1){
                    std::cout << "\n" << curr_player->name << "'s turn! ";
                } else {
                    std::cout << "\nDealer " << curr_player->name << "'s turn! ";
                }

                // Each player's hand has it's own set of turns. This helps keep track of them
                int hand_count = 0;
                do {
                    CharacterHand* curr_hand = &(curr_player->hands[hand_count]);
                    bool first_move = true;          
                    bool curr_hand_active = true;
                    do {
                        if (first_move == true){
                            if (curr_hand->score == 21){
                                // 21 on the first hand is a BlackJack, which pays 3:2 odds.
                                std::cout << "\n" << curr_player->name << " Has BlackJack! ";
                                curr_hand->bet =  round((curr_hand->bet) * 3/2);
                                curr_hand_active = false;
                                continue;
                            } 
                            else {
                                if (curr_hand->cards.has_any_duplicates() == true){ // If it is the first draw and a hand has duplicates, splitting is allowed
                                    actions = {"Hit", "Stand", "Double", "Split", "Surrender"};
                                }
                                display_character_hand(*curr_player, *curr_hand);
                                first_move = false;
                            }
                        }
                        curr_hand_active = next_move(curr_player, curr_hand);
                    } while (curr_hand_active == true);
                    hand_count ++;
                } while (hand_count < curr_player->hands.size());
            }
        }

        dealer_score = dealer_hand -> score;

        std::cout << "\n";
        display_game_state();
        std::cout << "\n";

        // For each active character, check their score against the dealer's to determine win/loss/tie
        for (Character* curr_player : characters){
            if (!(std::find(inactive_characters.begin(), inactive_characters.end(), curr_player) != inactive_characters.end()) // If the current character id is not in the inactive list 
                || (curr_player->type != 2)){ // OR current character is not the dealer.
                    for (CharacterHand& curr_hand : curr_player->hands){

                        int character_score = curr_hand.score;

                        if ((character_score > 21) || (character_score < dealer_score) && (dealer_score <= 21)){ // If the character busted, or their score was less than the dealer's
                            if (curr_player->type == 0){
                                std::cout << "You lose $" << curr_hand.bet << "!\n";
                            } else {
                                std::cout << curr_player->name << " loses $" << curr_hand.bet << "!\n";
                            }
                            lose_bet(curr_player, &curr_hand);
                        } else if ((character_score > dealer_score) || (character_score < dealer_score) && (dealer_score > 21)){
                            if (curr_player->type == 0){
                                std::cout << "You win $" << curr_hand.bet << "!\n";
                            } else {
                                std::cout << curr_player->name << " wins $" << curr_hand.bet << "!\n";
                            }
                            win_bet(curr_player, &curr_hand, 2); // A normal win returns 2:1 odds
                        } else if (character_score == dealer_score){
                            if (curr_player->type == 0){
                                std::cout << "You tie with the dealer! You have" << curr_hand.bet << " returned!\n";
                            } else {
                                std::cout << curr_player->name << " ties with the dealer! " << curr_player->name << " has " << curr_hand.bet << " returned!\n";
                            }
                            tie_bet(curr_player, &curr_hand);
                    }
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

    bool next_move(Character* curr_player, CharacterHand* curr_hand) {
        bool has_legal_moves = true;
        std::string selection;

        switch(curr_player->type){
            case 0:
                std::cout << "\nSelect your next move:\n";
                selection = actions[action_select()];
                break;
            case 1:
                selection = character_choose_move(*curr_player, *curr_hand);
                break;
            case 2:
                selection = dealer_choose_move(*curr_player, *curr_hand);
                break;
        }

        if (selection == "Hit") {
            std::cout << curr_player->name << " Hits! ";
            hit(curr_hand->cards, deck);
        } else if (selection == "Stand") {
            std::cout << curr_player->name << " Stands! ";
            has_legal_moves = false;
        } else if (selection == "Double") {
            curr_player->inventory -= curr_hand->bet;
            curr_hand->bet += curr_hand->bet;

            if (curr_player->type == 0){
                std::cout << "You double! Your new bet is: $" << curr_hand->bet << "\n";
            } else {
                std::cout << curr_player->name << " Doubles! Their new bet is: $" << curr_hand->bet << "\n";
            }

            hit(curr_hand->cards, deck);
            has_legal_moves = false;

        } else if (selection == "Split") {
            std::cout << curr_player->name << " Splits! ";
            actions = {"Hit", "Stand", "Double", "Surrender"}; // Return the action selection to normal
        } else if (selection == "Surrender") {
            std::cout << curr_player->name << " Surrenders! ";
            has_legal_moves = false;
        }

        display_character_hand(*curr_player, *curr_hand);
        curr_hand->score = curr_hand->cards.get_total_value_blackjack();

        if (curr_hand->score > 21){
            std::cout << curr_player->name << " Busts!\n";
            has_legal_moves = false;
        } else if (curr_hand->score == 21){
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
        for (Character* curr_player : characters){
            if (!(std::find(inactive_characters.begin(), inactive_characters.end(), curr_player) != inactive_characters.end())){ // If the current character id is not in the inactive list
                for (CharacterHand curr_hand : curr_player -> hands){
                    if (curr_player -> type == 0){
                        std::cout << "Your hand is: " << curr_hand.cards.get_contents_display() << "\n";
                    } else if (curr_player -> type == 2){
                        std::cout << "Dealer " << curr_player->name << "'s hand is: " << curr_hand.cards.get_contents_display() << "\n";
                    } else {
                        std::cout << curr_player->name << "'s hand is: " << curr_hand.cards.get_contents_display() << "\n";
                    }
                }
            }
        }
    }

    void display_character_hand(Character& curr_player, CharacterHand& curr_hand) {
        if (curr_player.type == 0){
            std::cout << "Your hand is: " << curr_hand.cards.get_contents_display() << "\n";

        } else {
            std::cout << curr_player.name << "'s hand is: " << curr_hand.cards.get_contents_display() << "\n";
        }
    }

    std::string character_choose_move(Character& curr_player, CharacterHand& curr_hand) { // Reference to character here for future implementation of stats to weight decisions
        int curr_score = curr_hand.score;
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

    std::string dealer_choose_move(Character& curr_player, CharacterHand& curr_hand) { // Reference to character here for future implementation of stats to weight decisions
        int curr_score = curr_hand.score;
        if (curr_score <= 16){
            return "Hit";
        } else {
            return "Stand";
        }
    }

};
#endif //GAMES