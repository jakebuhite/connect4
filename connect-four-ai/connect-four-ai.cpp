// 
// connect-four-ai.cpp
// Jake Buhite and Nick Abegg
// 10/26/2023
//
#include "connect-four.h"
#include "minimax.h"

void beginPvP(Connect4* game);
void beginPvA(Connect4* game, MiniMax* agent);
void beginAvA(Connect4* game, MiniMax* agent);

int main()
{
    int rows = -1;
    int cols = -1;
    int depth = 10;

    std::cout << "Please enter the number of rows: ";
    std::cin >> rows;

    std::cout << "Please enter the number of cols: ";
    std::cin >> cols;

    Connect4* game = new Connect4(rows, cols);
	MiniMax* agent = new MiniMax(game, depth);
    //beginPvP();
    beginPvA(game, agent);
    //beginAvA();
    delete game;
}

void beginPvP(Connect4* game) {
	std::string actors[] = { "NONE", "PLAYER 1", "PLAYER 2" };
	int choice = 0;

	// Main game loop
	game->printBoard();
	while (!game->isGoalState()) {
		game->setCurrentTurn((game->getCurrentTurn() == PLAYER1) ? PLAYER2 : PLAYER1);
		std::cout << "It is now " << actors[game->getCurrentTurn()] << " turn. Please select a column: ";
		std::cin >> choice;
		while (game->isDominateMove(choice)) {
			std::cout << "ERROR: Please pick a non-dominate move for the first round: ";
			std::cin >> choice;
		}
		int row = game->nextRow(choice);
		game->addDisc(row, choice);
		game->printBoard();
		if (game->getCurrentTurn()  == PLAYER2)
			game->incrementRound();
	}
	game->setWinner(game->getCurrentTurn());
	std::cout << "And the winner is... " << actors[game->getWinner()] << "!" << std::endl;
}

void beginPvA(Connect4* game, MiniMax* agent) {
	std::string actors[] = { "NONE", "PLAYER 1", "AI" };
	int choice = 0;

	// Main game loop
	game->printBoard();
	while (!game->isGoalState()) {
		game->setCurrentTurn((game->getCurrentTurn() == PLAYER1) ? PLAYER2 : PLAYER1);
		std::cout << "It is now " << actors[game->getCurrentTurn()] << " turn. Please select a column: ";
		if (game->getCurrentTurn() == PLAYER1) {
			std::cin >> choice;
			while (game->isDominateMove(choice)) {
				std::cout << "ERROR: Please pick a non-dominate move for the first round: ";
				std::cin >> choice;
			}
		}
		else {
			std::cout << std::endl;
			choice = agent->getAIMove();
		}
		int row = game->nextRow(choice);
		game->addDisc(row, choice);
		game->printBoard();
		if (game->getCurrentTurn() == PLAYER2)
			game->incrementRound();
	}
	game->setWinner(game->getCurrentTurn());
	std::cout << "And the winner is... " << actors[game->getWinner()] << "!" << std::endl;
}

void beginAvA(Connect4* game, MiniMax* agent) {
	std::string actors[] = { "NONE", "PLAYER 1", "PLAYER 2" };
	int choice = 0;

	// Main game loop
	game->printBoard();
	while (!game->isGoalState()) {
		game->setCurrentTurn((game->getCurrentTurn() == PLAYER1) ? PLAYER2 : PLAYER1);
		std::cout << "It is now " << actors[game->getCurrentTurn()] << " turn. Please select a column: ";
		choice = agent->getAIMove();
		int row = game->nextRow(choice);
		game->addDisc(row, choice);
		game->printBoard();
		if (game->getCurrentTurn() == PLAYER2)
			game->incrementRound();
	}
	game->setWinner(game->getCurrentTurn());
	std::cout << "And the winner is... " << actors[game->getWinner()] << "!" << std::endl;
}