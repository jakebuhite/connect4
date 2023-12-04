// 
// minimax.cpp
// Jake Buhite and Nick Abegg
// 10/26/2023
//
#include "minimax.h"

MiniMax::MiniMax() { 
	game = nullptr;
	player = PLAYER1;
	opponent = PLAYER2;
	maxDepth = 7;
}

MiniMax::MiniMax(Connect4* game) : MiniMax() { 
	this->game = game; 
	generateOptimalMoveOrder();
}

MiniMax::MiniMax(Connect4* game, int depth, Actor player) : MiniMax(game) { 
	this->player = player;
	this->opponent = (player == PLAYER1) ? PLAYER2 : PLAYER1;
	maxDepth = (depth % 2 == 0) ? (depth - 1) : depth; // Ensure that depth is odd 
}

int MiniMax::getAgentMove() { return miniMax(INT_MIN, INT_MAX); }

int MiniMax::miniMax(int alpha, int beta) { return maxValue(alpha, beta, maxDepth).second; }

std::pair<int, int> MiniMax::minValue(int alpha, int beta, int depth) {
	std::vector<int> actions = getValidActions();
	if (game->hasWinner() || game->isDraw() || depth <= 0) return { utility(depth), -1 };
	std::pair<int, int> bestMove = { AI_WIN, actions[0] };
	for (int move : actions) {
		int row = game->nextRow(move);
		game->addDisc(row, move, opponent);
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
	if (game->hasWinner() || game->isDraw() || depth <= 0) return { utility(depth), -1 };
	std::pair<int, int> bestMove = { PLAYER_WIN, actions[0] };
	for (int move : actions) {
		int row = game->nextRow(move);
		game->addDisc(row, move, player);
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
	int opponentScore = nInARow(opponent);
	int playerScore = nInARow(player);

	if (opponentScore == PLAYER_WIN) return opponentScore;
	else if (playerScore == AI_WIN) { return playerScore; }
	else if (game->getAvailableSpaces() == 0) return 0;

	return playerScore - opponentScore;
}

int MiniMax::nInARow(Actor player) {
	int score = 0;
	int cols = game->getCols();
	int rows = game->getRows();

	// Check vertical n in a row
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows - 3; j++) {
			int n = 0;
			int emptyCells = 0;
			for (int k = 0; k < 4; k++) {
				if (game->getCell(j + k, i) == player) n++;
				else if (game->getCell(j + k, i) == 0) emptyCells++;
				else break;
			}
			if (n == 4) return (player == this->player) ? AI_WIN : PLAYER_WIN;
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
				if (game->getCell(i, j + k) == player) n++;
				else if (game->getCell(i, j + k) == 0) emptyCells++;
				else break;
			}
			if (n == 4) return (player == this->player) ? AI_WIN : PLAYER_WIN;
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
				if (game->getCell(i + k, j + k) == player) n++;
				else if (game->getCell(i + k, j + k) == 0) emptyCells++;
				else break;
			}
			if (n == 4) return (player == this->player) ? AI_WIN : PLAYER_WIN;
			else if (n == 3 && emptyCells == 1) score += 1000;
			else if (n == 2 && emptyCells == 2) score += 10;

			n = 0;
			emptyCells = 0;
			for (int k = 0; k < 4; k++) {
				if (game->getCell(i + k, j + 3 - k) == player) n++;
				else if (game->getCell(i + k, j + 3 - k) == 0) emptyCells++;
				else break;
			}
			if (n == 4) return (player == this->player) ? AI_WIN : PLAYER_WIN;
			else if (n == 3 && emptyCells == 1) score += 1000;
			else if (n == 2 && emptyCells == 2) score += 10;
		}
	}

	return score;
}

void MiniMax::generateOptimalMoveOrder() {
	std::vector<int> moves;

	// Add center columns first
	int center = game->getCols() / 2;
	moves.push_back(center);

	// Add columns in order relative to their distance to the center column
	for (int i = 1; i <= center; i++) {
		moves.push_back(center - i);
		moves.push_back(center + i);
	}

	optimalMoveOrder = moves;
}