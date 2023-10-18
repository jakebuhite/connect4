#include "minimax.h"

MiniMax::MiniMax(Connect4* game) {
    this->game = game;
    maxDepth = 5;
}

MiniMax::MiniMax(Connect4* game, int d) {
    this->game = game;
    maxDepth = d;
}

std::pair<int, int> MiniMax::minValue(int alpha, int beta, int depth) {
    if (terminalTest() || depth <= 0)
        return { utility(), -1 };
    std::pair<int, int> bestMove = { INT_MAX, -1 };
    for (int move : validActions()) {
        int row = game->nextRow(move);
        game->addDisc(row, move, PLAYER1);
        int newValue = maxValue(alpha, beta, depth - 1).first;
        game->removeDisc(row, move);
        if (newValue < bestMove.first)
            bestMove = { newValue, move };
        alpha = std::min(alpha, newValue);
        if (alpha >= beta)
            break;
    }
    return bestMove;
}

std::pair<int, int> MiniMax::maxValue(int alpha, int beta, int depth) {
    if (terminalTest() || depth <= 0)
        return { utility(), -1 };
    std::pair<int, int> bestMove = { INT_MIN, -1 };
    for (int move : validActions()) {
        int row = game->nextRow(move);
        game->addDisc(row, move, PLAYER2);
        int newValue = minValue(alpha, beta, depth - 1).first;
        game->removeDisc(row, move);
        if (newValue > bestMove.first)
            bestMove = { newValue, move };
        alpha = std::max(alpha, newValue);
        if (alpha >= beta)
            break;
    }
    return bestMove;
}

int MiniMax::getAIMove() {
    return miniMax(INT_MIN, INT_MAX);
}

int MiniMax::miniMax(int alpha, int beta) {
    if (game->getCurrentTurn() == PLAYER2) {
        return maxValue(alpha, beta, maxDepth).second;
    }
    else { // Player
        return minValue(alpha, beta, maxDepth).second;
    }
}

std::vector<int> MiniMax::validActions() {
    return game->getValidActions();
}

int MiniMax::utility() {
    // Check if the game is won
    if (game->isGoalState()) {
        if (game->getWinner() == PLAYER2)
            return -100000;
        else if (game->getWinner() == PLAYER1)
            return 100000;
        else
            return 0;
    }

    // Calculate the difference in the number of threats
    return countThreats(PLAYER2) - countThreats(PLAYER1);
}

int MiniMax::countThreats(Actor actor) {
    int threats = 0;
    for (int i = 0; i < game->getCols(); i++) {
        for (int j = 0; j < game->getRows(); j++) {
            if (game->getPos(j, i) == 0) {
                if ((j == game->getRows() - 1 || game->getPos(j + 1, i) != 0) && (j == 0 || game->getPos(j - 1, i) == 0))
                    threats++;
                else if (j < game->getRows() - 2 && game->getPos(j + 1, i) == actor && game->getPos(j + 2, i) == actor)
                    threats++;
            }
        }
    }
    return threats;
}

bool MiniMax::terminalTest() {
    return game->isGoalState();
}