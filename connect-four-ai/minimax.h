// 
// minimax.h
// Jake Buhite and Nick Abegg
// 10/26/2023
//
#pragma once
#include "agent.h"
#include "connect-four.h"

constexpr auto AI_WIN = 9999999;
constexpr auto PLAYER_WIN = -9999999;

class MiniMax : public Agent {
private:
	int maxDepth;
	Connect4* game;
	std::vector<int> optimalMoveOrder;

	int miniMax(int alpha, int beta);
	std::pair<int, int> minValue(int alpha, int beta, int depth);
	std::pair<int, int> maxValue(int alpha, int beta, int depth);
	std::vector<int> getValidActions();
	int utility(int depth);
	int nInARow(Actor player);
	void generateOptimalMoveOrder();

	Actor player;
	Actor opponent;
public:
	MiniMax();
	MiniMax(Connect4* game);
	MiniMax(Connect4* game, int depth, Actor player);

	int getAgentMove();
};