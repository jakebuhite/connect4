#pragma once
#include <iostream>
#include <string>
#include <vector>

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
	void addDisc(int row, int col);
	void addDisc(int row, int col, Actor actor);
	void removeDisc(int row, int col);
	int nextRow(int col);
	bool validMove(int row, int col);
	bool isGoalState();
	std::string repeat(std::string s, int n);
	void printBoard();

	// MiniMax
	int maxDepth;

	std::vector<int> getValidActions();
	int numConsecDiscs();
	int utility();
	int nInARow(Actor actor);
	std::pair<int, int> minValue(int alpha, int beta, int depth);
	std::pair<int, int> maxValue(int alpha, int beta, int depth);
	int getAIMove();
	int miniMax(int alpha, int beta);
public:
	Connect4();
	Connect4(int rows, int cols, int d);
	~Connect4();
	void beginGame();
	void beginAIGame();
};