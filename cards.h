#include <string>

class Card {
    public:
        int value; // Supports values 1 - 13
        int suit; // Supports values 0 - 3

        Card (int a = rand() % 13 + 1, int b =  rand() % 4) {this -> value = a; this -> suit = b;}
        
        void generate_random_card () {
            //srand (time(NULL));
            value = rand() % 13 + 1;
            suit = rand() % 4;
        }

        std::string display_card_value () {
            switch(this -> value){
                case 1: return "A";
                case 11: return "J";
                case 12: return  "Q";
                case 13: return "K";
                default: return std::to_string(this -> value);
            }
        }

        std::string display_card_suit () {
            switch(this -> suit){
                case 0: return "\033[1;30m♠\033[0m\n"; //Black Spade
                case 1: return "\033[1;31m♥\033[0m\n"; //Red Heart
                case 2: return  "\033[1;31m♦\033[0m\n"; //Red Diamond
                case 3: return "\033[1;30m♣\033[0m\n"; //Black Club
                default: return "?";
            }
        }

        std::string display_card() {
            return display_card_value() + display_card_suit();
        }
};

class Deck {

};