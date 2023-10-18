#pragma once
#include <string>
#include <vector>
#include "connect-four.h"

class MiniMax {
private:
	Connect4* game;
	int maxDepth;

	std::vector<int> validActions();
	int countThreats(Actor actor);
	int utility();
	bool terminalTest();
	std::pair<int, int> minValue(int alpha, int beta, int depth);
	std::pair<int, int> maxValue(int alpha, int beta, int depth);
	int miniMax(int alpha, int beta);
public:
	MiniMax(Connect4* game);
	MiniMax(Connect4* game, int d);
	int getAIMove();
};