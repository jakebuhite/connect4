// 
// minimax.h
// Jake Buhite and Nick Abegg
// 10/26/2023
//
#pragma once
#include "connect-four.h"

class MiniMax {
private:
	int maxDepth = 6;
	Connect4* game;

	int miniMax(int alpha, int beta);
	std::pair<int, int> minValue(int alpha, int beta, int depth);
	std::pair<int, int> maxValue(int alpha, int beta, int depth);
	std::vector<int> getValidActions();
	int utility(int depth);
	int nInARow(Actor actor);
public:
	MiniMax();
	MiniMax(Connect4* game);
	MiniMax(Connect4* game, int depth);

	int getAIMove();
};