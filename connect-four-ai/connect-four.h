#pragma once
#include <iostream>
#include <string>

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

	void initBoard();
	bool addDisc(int row, int col);
	int nextRow(int col);
	bool validMove(int row, int col);
	bool isGoalState(int lastX, int lastY);
	std::string repeat(std::string s, int n);
public:
	Connect4();
	Connect4(int rows, int cols);
	~Connect4();
	void beginGame();
	void printBoard();
};