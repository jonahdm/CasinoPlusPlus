#include <iostream>
#include <vector>
#include <string>
#include <locale.h>
#include <time.h> 

#include "cards.h"

int main()
{    
    srand(time(NULL)); //Initialize random number generator
    setlocale(LC_ALL, ".UTF-8"); // Needed to properly display Unicode in VSCode Terminal
    std::locale::global(std::locale(".UTF-8")); // Needed to properly display Unicode in VSCode Terminal

    Card card1;
    Card card2;
    Card card3;

    std::cout << card1.display_card() << "\n";
    std::cout << card2.display_card() << "\n";
    std::cout << card3.display_card() << "\n";

    return true;
}