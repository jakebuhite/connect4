// 
// connect-four.h
// Jake Buhite and Nick Abegg
// 10/26/2023
//
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "actor.h"

constexpr auto AI_WIN = 9999999;
constexpr auto PLAYER_WIN = -9999999;

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

	// Utils
	void initBoard();
	bool validMove(int row, int col);
	std::string repeat(std::string s, int n);
public:
	// Constructors
	Connect4();
	Connect4(int rows, int cols);
	~Connect4();

	// Game Functions
	void printBoard();
	bool isGoalState();
	bool isDominateMove(int col);
	int nextRow(int col);

	// Board management
	void addDisc(int row, int col);
	void addDisc(int row, int col, Actor actor);
	void removeDisc(int row, int col);

	// Getters and Setters
	void incrementRound();
	void setWinner(Actor a);
	void setCurrentTurn(Actor a);
	Actor getWinner();
	Actor getCurrentTurn();
	int getAvailableSpaces();
	int getCell(int x, int y);
	int getRows();
	int getCols();
	std::pair<int, int> getLastTurn();
};