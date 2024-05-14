#ifndef CARDS
#define CARDS

#include <algorithm>
#include <numeric>       
#include <random>
#include <string>
#include <vector>

class Card {
    public:
        int value; // Supports values 1 - 13
        int suit; // Supports values 0 - 3

        Card (int v = rand() % 13 + 1, int s =  rand() % 4):
            value(v),
            suit(s)
            {}
        
        void generate_random_card () {
            value = rand() % 13 + 1;
            suit = rand() % 4;
        }

        std::string get_value_display () {
            switch(value){
                case 1: return "A";
                case 11: return "J";
                case 12: return  "Q";
                case 13: return "K";
                default: return std::to_string(value);
            }
        }

        std::string get_suit_display () {
            switch(this -> suit){
                case 0: return "♠"; //Spade
                case 1: return "♥"; //Heart
                case 2: return  "♦"; //Diamond
                case 3: return "♣"; //Club
                default: return "?";
            }    
        }
        // // ANSI Escape Codes can be enabled so that text output has color, but this is fragile and dependent on OS
        // std::string get_suit_display () {
        //     switch(this -> suit){
        //         case 0: return "\033[1;30m♠\033[0m"; //Black Spade
        //         case 1: return "\033[1;31m♥\033[0m"; //Red Heart
        //         case 2: return  "\033[1;31m♦\033[0m"; //Red Diamond
        //         case 3: return "\033[1;30m♣\033[0m"; //Black Club
        //         default: return "?";
        //     }
        // }

        std::string get_card_display() {
            return get_value_display() +get_suit_display();
        }
};

class Deck {
    public:
        std::vector<Card> contents;

    int size(){
        return contents.size();
    }

    int get_total_value_blackjack(){
        int sum = 0;
        bool any_ace = false;

        for (int i = 0; i < size(); ++i){
            if (contents[i].value == 1){
                // Aces are scored as either 1 or 11, depending on what's best for the player
                if (sum < 11) {
                    sum += 11;
                } else {
                    sum += 1;
                }
                any_ace = true;
            } else if (contents[i].value > 10) { // Face cards are worth 10
                sum += 10;
            } else {
                sum += contents[i].value;
            }
        }
        if ((sum > 21) && (any_ace == true)){ // If the hand would be a bust, treat the first ace as a 1 instead of an 11
            sum -= 10;
        }
        return sum;
    }

    void add_top_card(Card new_card){
        contents.push_back(new_card);
    }

    void add_top_deck(std::vector<Card> deck_in){
        contents.insert(contents.end(), std::begin(deck_in), std::end(deck_in));
    }

    void add_top_deck(Deck deck_in){
        contents.insert(contents.end(), std::begin(deck_in.contents), std::end(deck_in.contents));
    }

    void add_bottom_deck(std::vector<Card> deck_in){
        contents.insert(contents.begin(), std::begin(deck_in), std::end(deck_in));
    }

    void add_bottom_deck(Deck deck_in){
        contents.insert(contents.begin(), std::begin(deck_in.contents), std::end(deck_in.contents));
    }

    void add_bottom_card(Card new_card){  
        contents.insert(contents.begin(), new_card);
    }

    void shuffle_deck(std::default_random_engine rng){
        std::shuffle(contents.begin(), contents.end(), rng);
    }
    
    Card draw_top_card() {
        Card drawn_card =  contents.back();
        contents.pop_back();
        return drawn_card;
    }


    Deck draw_n_cards(int n) {
        Deck drawn_cards;
        std::vector<Card> drawn_contents(contents.end() - n, contents.end());
        drawn_cards.add_top_deck(drawn_contents);

        contents.resize(contents.size()-n);

        return drawn_cards;
    }       

    std::string get_contents_display(){
        std::string output = "";
        for (Card this_card : contents)
            output += this_card.get_card_display() + " ";
        return output;
    }

    void build_standard_deck(){
        contents.clear();
        for (int suit = 0; suit <= 3; ++suit){
            for (int value = 1; value <= 13; ++value){
                contents.push_back(Card(value, suit));
            }
        }
    }
};

#endif //CARDS