// 
// connect-four-ai.cpp
// Jake Buhite and Nick Abegg
// 10/20/2023
//
#include <iostream>
#include "connect-four.h"

int main()
{
    int rows = -1;
    int cols = -1;
    int depth = 10;

    std::cout << "Please enter the number of rows: ";
    std::cin >> rows;

    std::cout << "Please enter the number of cols: ";
    std::cin >> cols;

    Connect4* game = new Connect4(rows, cols, depth);
    //game->beginPvP();
    game->beginPvA();
    //game->beginAvA();
    delete game;
}
