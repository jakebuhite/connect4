#pragma once
#include "minimax.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>

constexpr auto CONNECT_COUNT = 4;

enum Actor {
	NONE = 0,
	PLAYER1 = 1,
	PLAYER2 = 2 // AI
};

class Connect4 {
private:
	int** board;
	int cols;
	int rows;
	int availableSpaces;
	int round;
	Actor currentTurn;
	Actor winner;
	std::pair<int, int> lastMove;

	void initBoard();
	bool validMove(int row, int col);
	std::string repeat(std::string s, int n);
public:
	Connect4();
	Connect4(int rows, int cols);
	~Connect4();
	void beginGame();
	void beginGame(MiniMax* agent);
	void printBoard();
	bool isGoalState();
	bool addDisc(int row, int col);
	bool addDisc(int row, int col, Actor actor);
	void removeDisc(int row, int col);
	int nextRow(int col);
	int getCols();
	int getRows();
	int getPos(int row, int col);
	std::vector<int> getValidActions();
	int getAvailableSpaces();
	Actor getWinner();
	Actor getCurrentTurn();
};