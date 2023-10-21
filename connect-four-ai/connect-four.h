#pragma once
#include <iostream>
#include <string>
#include <vector>

constexpr auto CONNECT_COUNT = 4;

enum Actor { NONE = 0, PLAYER1 = 1, PLAYER2 = 2 /* AI */ };

class Connect4 {
private:
	// Board
	int** board;
	int cols = 7;
	int rows = 6;
	int availableSpaces = 42;

	// Game management
	int round = 1;
	Actor currentTurn = PLAYER2;
	Actor winner = NONE;
	std::pair<int, int> lastMove = { 0, 0 };

	void initBoard();

	// Board management
	void addDisc(int row, int col);
	void addDisc(int row, int col, Actor actor);
	void removeDisc(int row, int col);
	void printBoard();

	// Utils
	int nextRow(int col);
	bool validMove(int row, int col);
	bool isDominateMove(int col);
	bool isGoalState();
	std::string repeat(std::string s, int n);

	// MiniMax
	int maxDepth = 6;

	int getAIMove();
	int miniMax(int alpha, int beta);
	std::pair<int, int> minValue(int alpha, int beta, int depth);
	std::pair<int, int> maxValue(int alpha, int beta, int depth);
	std::vector<int> getValidActions();
	int utility();
	int nInARow(Actor actor);
public:
	// Constructors
	Connect4();
	Connect4(int rows, int cols, int d);
	~Connect4();

	// Game Functions
	void beginPvP();
	void beginPvA();
	void beginAvA();
};