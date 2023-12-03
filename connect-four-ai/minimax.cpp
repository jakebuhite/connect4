// 
// minimax.cpp
// Jake Buhite and Nick Abegg
// 10/26/2023
//
#include "minimax.h"

MiniMax::MiniMax() { game = nullptr; }

MiniMax::MiniMax(Connect4* game) { 
	this->game = game; 
	generateOptimalMoveOrder();
}

MiniMax::MiniMax(Connect4* game, int depth) : MiniMax(game) { maxDepth = depth; }

int MiniMax::getAgentMove() { return miniMax(INT_MIN, INT_MAX); }

int MiniMax::miniMax(int alpha, int beta) { 
	std::pair<int, int> move;
	if (game->getCurrentTurn() == PLAYER2) {
		move = maxValue(alpha, beta, maxDepth - 1);
	}
	else {
		move = minValue(alpha, beta, maxDepth - 1);
	}
	return move.second;
}

std::pair<int, int> MiniMax::minValue(int alpha, int beta, int depth) {
	std::vector<int> actions = getValidActions();
	if (game->isGoalState() || depth <= 0) return { utility(depth), -1 };
	std::pair<int, int> bestMove = { AI_WIN, actions[0] };
	for (int move : actions) {
		int row = game->nextRow(move);
		game->addDisc(row, move, PLAYER1);
		int newValue = maxValue(alpha, beta, depth - 1).first;
		game->removeDisc(row, move);
		if (newValue < bestMove.first) bestMove = { newValue, move };
		beta = std::min(beta, newValue);
		if (alpha >= beta) break;
	}
	return bestMove;
}

std::pair<int, int> MiniMax::maxValue(int alpha, int beta, int depth) {
	std::vector<int> actions = getValidActions();
	if (game->isGoalState() || depth <= 0) return { utility(depth), -1 };
	std::pair<int, int> bestMove = { PLAYER_WIN, actions[0] };
	for (int move : actions) {
		int row = game->nextRow(move);
		game->addDisc(row, move, PLAYER2);
		int newValue = minValue(alpha, beta, depth - 1).first;
		game->removeDisc(row, move);
		if (newValue > bestMove.first) bestMove = { newValue, move };
		alpha = std::max(alpha, newValue);
		if (alpha >= beta) break;
	}
	return bestMove;
}

std::vector<int> MiniMax::getValidActions() {
	std::vector<int> actions;
	for (int i = 0; i < optimalMoveOrder.size(); i++)
		if (!game->isDominateMove(optimalMoveOrder[i]) && !game->getCell(0, optimalMoveOrder[i]))
			actions.push_back(optimalMoveOrder[i]);
	if (actions.empty()) return { -1 };
	return actions;
}

int MiniMax::utility(int depth) {
	int scorePlayer = nInARow(PLAYER1);
	int scoreAI = nInARow(PLAYER2);

	if (scorePlayer == PLAYER_WIN) return scorePlayer - depth;
	else if (scoreAI == AI_WIN) { return scoreAI + depth; }
	else if (game->getAvailableSpaces() == 0) return 0;

	return (scoreAI - scorePlayer);
}

int MiniMax::nInARow(Actor actor) {
	int score = 0;
	int cols = game->getCols();
	int rows = game->getRows();

	// Check vertical n in a row
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows - 3; j++) {
			int n = 0;
			int emptyCells = 0;
			for (int k = 0; k < 4; k++) {
				if (game->getCell(j + k, i) == actor) n++;
				else if (game->getCell(j + k, i) == 0) emptyCells++;
				else break;
			}
			if (n == 4) return (actor == 2) ? AI_WIN : PLAYER_WIN;
			else if (n == 3 && emptyCells == 1) score += 1000;
			else if (n == 2 && emptyCells == 2) score += 10;
		}
	}

	// Check horizontal n in a row
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols - 3; j++) {
			int n = 0;
			int emptyCells = 0;
			for (int k = 0; k < 4; k++) {
				if (game->getCell(i, j + k) == actor) n++;
				else if (game->getCell(i, j + k) == 0) emptyCells++;
				else break;
			}
			if (n == 4) return (actor == 2) ? AI_WIN : PLAYER_WIN;
			else if (n == 3 && emptyCells == 1) score += 1000;
			else if (n == 2 && emptyCells == 2) score += 10;
		}
	}

	// Check diagonal n in a row
	for (int i = 0; i < rows - 3; i++) {
		for (int j = 0; j < cols - 3; j++) {
			int n = 0;
			int emptyCells = 0;
			for (int k = 0; k < 4; k++) {
				if (game->getCell(i + k, j + k) == actor) n++;
				else if (game->getCell(i + k, j + k) == 0) emptyCells++;
				else break;
			}
			if (n == 4) return (actor == 2) ? AI_WIN : PLAYER_WIN;
			else if (n == 3 && emptyCells == 1) score += 1000;
			else if (n == 2 && emptyCells == 2) score += 10;

			n = 0;
			emptyCells = 0;
			for (int k = 0; k < 4; k++) {
				if (game->getCell(i + k, j + 3 - k)  == actor) n++;
				else if (game->getCell(i + k, j + 3 - k) == 0) emptyCells++;
				else break;
			}
			if (n == 4) return (actor == 2) ? AI_WIN : PLAYER_WIN;
			else if (n == 3 && emptyCells == 1) score += 1000;
			else if (n == 2 && emptyCells == 2) score += 10;
		}
	}

	return score;
}

void MiniMax::generateOptimalMoveOrder() {
	// Start with the center column
	int middle = game->getCols() / 2;
	optimalMoveOrder.push_back(middle);

	for (int i = 1; i <= middle; i++) {
		optimalMoveOrder.push_back(middle - i);
		optimalMoveOrder.push_back(middle + i);
	}
}