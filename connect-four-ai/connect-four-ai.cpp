// 
// connect-four-ai.cpp
// Jake Buhite and Nick Abegg
// 10/17/2023
//
#include <iostream>
#include "connect-four.h"

int main()
{
    int rows = -1;
    int cols = -1;

    std::cout << "Please enter the number of rows: ";
    std::cin >> rows;

    std::cout << "Please enter the number of cols: ";
    std::cin >> cols;

    Connect4* game = new Connect4(rows, cols);
    game->beginGame();
    delete game;
}
