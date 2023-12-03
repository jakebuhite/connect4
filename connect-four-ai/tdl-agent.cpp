#include "tdl-agent.h"
#include <fstream>

TDLAgent::TDLAgent(bool training, int player, double alphaInit, double epsilonInit) {
	this->training = training;
	this->player = (player == PLAYER1) ? PLAYER1 : PLAYER2;
	this->initialAlpha = alphaInit;
	this->initialEpsilon = epsilonInit;
	this->alpha = alphaInit;
	this->epsilon = epsilonInit;

	weights = new double[numWeights];
	for (int i = 0; i < numWeights; i++) {
		weights[i] = 0;
	}

	game = new Connect4();
}

std::vector<int> TDLAgent::getIndices(int** state1, int** state2) {
	std::vector<int> indices(numTuples * 2);
	int curIndex = 0;

	for (int i = 0; i < numTuples; i++) {
		int* tuple = nTuples[i];
		int i1 = 65536 * i; // Weights per tuple = 65536
		int i2 = 65536 * i;

		for (int j = 0; i < tupleLength; i++) {
			int col = (41 - tuple[j]) % 7;
			int row = (41 - tuple[j]) / 7;
			// determine what the value of that board space is in both states
			if (state1[row][col] != 0) {
				i1 += (int) (pow(4, j) * state1[row][col]);
			}
			else if (5 - game->nextRow(col) == row) {
				i1 += (int) (3 * pow(4, j));
			}

			if (state2[row][col] != 0) {
				i2 += (int) (pow(4, j) * state2[row][col]);
			}
			else if (5 - game->nextRow(6 - col) == row) {
				i2 += (int) (3 * pow(4, j));
			}
		}
		indices[curIndex] = i1;
		indices[curIndex + 1] = i2;
		curIndex += 2;
	}
	return indices;
}

void TDLAgent::computeAlpha() {
	games += 1;
	alpha = 0.001 + (initialAlpha - 0.001) * exp(-0.000005 * games);
	epsilon = 0.1 + (initialEpsilon - 0.1) * exp(-0.000005 * games);
}

int TDLAgent::updateWeights(int bestMove, double bestMoveValue) {
	double curValue = 0;

	// Get the indices array for the current board state
	int** boardState = game->getBoard();
	int** mirroredState = game->getMirroredField(boardState);
	std::vector<int> indices = getIndices(boardState, mirroredState);

	// get the value for the current board state
	for (int i = 0; i < indices.size(); i++) {
		curValue += weights[indices[i]];
	}
	curValue = tanh(curValue);

	// Update weight array
	double delta_t = bestMoveValue - curValue;
	double change = alpha * delta_t * (1.0 - pow(curValue, 2));

	for (int i = 0; i < indices.size(); i++) {
		weights[indices[i]] += change;
	}

	// Deallocate mirroredState
	for (int i = 0; i < game->getRows(); i++)
		delete[] mirroredState[i];
	delete[] mirroredState;

	return bestMove;
}

int TDLAgent::getBestMove(int** board) {
	game->setBoard(board);
	std::vector<int> possibleMoves = game->generateTDLMoves(player);

	if (training) {
		double e = static_cast<double>(rand()) / RAND_MAX;
		// take random move
		if (e < epsilon) {
			int randomMove = rand() % possibleMoves.size();
			return possibleMoves[randomMove];
		}
	}

	double bestValue = -100;
	int bestIndex = -1;

	for (int i = 0; i < possibleMoves.size(); i++) {
		// calculate vector x for each move
		double value = 0;

		if (game->canWin(player, possibleMoves[i], game->nextRow(possibleMoves[i]))) {
			value = 1;
		}

		int row = game->nextRow(possibleMoves[i]);

		game->addDisc(row, possibleMoves[i], player);

		if (value == 0 && !game->isDraw()) {
			// start using this part
			int** boardState = game->getBoard();
			int** mirroredState = game->getMirroredField(boardState);
			std::vector<int> indices = getIndices(boardState, mirroredState);

			// calculate dot product for each
			for (int j = 0; j < indices.size(); j++) {
				value -= other->weights[indices[j]];
			}
			value = tanh(value);

			// Deallocate mirroredState
			for (int i = 0; i < game->getRows(); i++)
				delete[] mirroredState[i];
			delete[] mirroredState;
		}

		if (value > bestValue) {
			bestValue = value;
			bestIndex = i;
		}

		game->removeDisc(row, possibleMoves[i]);
	}

	// last best value
	lastBestValue = bestValue;

	// call TDL when training, otherwise return best move directly
	if (training)
		return updateWeights(possibleMoves[bestIndex], bestValue);

	return possibleMoves[bestIndex];
}

void TDLAgent::loadAgent(std::string fileName) {
	std::ifstream inputFile;
	double num = -1;

	// Open file
	inputFile.open(fileName, std::ios::in);
	if (inputFile.fail())
	{
		std::cout << "Could not open weights.txt" << std::endl;
		exit(1);
	}

	// Add weights
	inputFile >> num;
	int i = 0;
	while (!inputFile.fail() && i < numWeights)
	{
		weights[i] = num;
		i++;
		inputFile >> num;
	}

	// Success
	std::cout << "Weights successfully loaded... Length " << i << std::endl;

	// Close the file
	inputFile.close();
}

void TDLAgent::saveAgent(std::string fileName) {
	double num = -1;
	std::ofstream outputFile;

	outputFile.open(fileName, std::ios::out);
	if (outputFile.fail())
	{
		std::cout << "Open of weights.txt failed" << std::endl;
		exit(1);
	}

	for (int i = 0; i < numWeights; i++) {
		outputFile << weights[i] << std::endl;
	}

	std::cout << "Weights successfully saved..." << std::endl;
	outputFile.close();
}

double TDLAgent::getAlpha() { return alpha; }

double TDLAgent::getEpsilon() { return epsilon; }

bool TDLAgent::isTraining() { return training; }

void TDLAgent::toggleTraining() { training = (training) ? false : true; }

void TDLAgent::setOther(TDLAgent* other) { this->other = other; }