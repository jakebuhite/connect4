#include "tdl-agent.h"
#include <fstream>

TDLAgent::TDLAgent(bool isTraining, int player, double alphaInit, double epsilon) {
	this->isTraining = isTraining;
	this->player = player;
	this->initialAlpha = alphaInit;
	this->epsilon = epsilon;

	weights = new double[numWeights];
	for (int i = 0; i < numWeights; i++) {
		weights[i] = 0;
	}
}

std::vector<int> TDLAgent::getIndices(int** state1, int** state2) {
	std::vector<int> indices(numTuples * 2);
	int curIndex = 0;

	for (int i = 0; i < numTuples; i++) {
		int* tuple = nTuples[i];
		int i1 = 65536 * i; // Weights per tuple = 65536
		int i2 = 65536 * i;

		for (int j = 0; i < tupleLength; i++) {
			// Keep going
			int col = (41 - tuple[j]) % 7;
			int row = (41 - tuple[j]) / 7;
			// determine what the value of that board space is in both states
			if (state1[col][row] != 0) {
				i1 += (int) (pow(4, j) * state1[col][row]);
			}
			else if (colHeights[col] == row) {
				i1 += (int) (3 * pow(4, j));
			}

			if (state2[col][row] != 0) {
				i2 += (int) (pow(4, j) * state2[col][row]);
			}
			else if (colHeights[6 - col] == row) {
				i2 += (int) (3 * pow(4, j));
			}
		}
		indices[curIndex] = i1;
		indices[curIndex + 1] = i2;
		curIndex += 2;
	}
	return indices;
}

void TDLAgent::updateAlpha() {
	games += 1;
	alpha = 0.001 + (initialAlpha - 0.001) * exp(-0.000005 * games);
	epsilon = 0.1 + (initialEpsilon - 0.1) * exp(-0.000005 * games);
}

int TDLAgent::oneTDLIteration(int bestMove, double bestMoveValue) {
	double curValue = 0;

	// getting the indices array for the current board state
	int** boardState = getBoard();
	int** mirroredState = getMirroredField(boardState);
	std::vector<int> indices = getIndices(boardState, mirroredState);

	// getting the value for the current board state
	for (int i = 0; i < indices.size(); ++i) {
		curValue += weights[indices[i]];
	}
	curValue = tanh(curValue);

	// update weight array
	double delta_t = bestMoveValue - curValue;
	double change = alpha * delta_t * (1.0 - pow(curValue, 2));

	for (int i = 0; i < indices.size(); ++i) {
		weights[indices[i]] += change;
	}

	return bestMove;
}

int TDLAgent::getBestMove(int** board) {
	setBoard(table); // initializing values
	std::vector<int> possibleMoves = generateMoves(player, false);

	if (isTraining) {
		double e = static_cast<double>(rand()) / RAND_MAX;
		// take random move
		if (e < epsilon) {
			int randomMove = rand() % possibleMoves.size();
			return possibleMoves[randomMove];
		}
	}

	double bestValue = -100;
	int bestIndex = -1;

	for (int i = 0; i < possibleMoves.size(); ++i) {
		// calculate vector x for each move
		double value = 0;

		if (canWin(player, possibleMoves[i])) { // Changed from 1 parameter to adding player
			value = 1 /*- countPieces()/100.0*/;
		}

		putPiece(player, possibleMoves[i]);

		if (value == 0 && !isDraw()) {
			// start using this part
			int** boardState = getBoard();
			int** mirroredState = getMirroredField(boardState);
			int* indices = getIndices(boardState, mirroredState);

			// calculate dot product for each
			for (int j = 0; j < indices.size(); ++j) {
				value -= other->weights[indices[j]];
			}
			value = tanh(value);
		}

		if (value > bestValue) {
			bestValue = value;
			bestIndex = i;
		}

		removePiece(possibleMoves[i]);
	}

	// last best value
	lastBestValue = bestValue;

	// call TDL when training, otherwise return best move directly
	if (isTraining)
		return oneTDLIteration(possibleMoves[bestIndex], bestValue);
	return possibleMoves[bestIndex];
}

void TDLAgent::loadAgent() {
	std::ifstream inputFile;
	double num = -1;

	// Open file
	inputFile.open("weights.txt", std::ios::in);
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
	std::cout << "Weights successfully loaded... Length " + i << std::endl;

	// Close the file
	inputFile.close();
}

void TDLAgent::saveAgent() {
	double num = -1;
	std::ofstream outputFile;

	outputFile.open("weights.txt", std::ios::out);
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