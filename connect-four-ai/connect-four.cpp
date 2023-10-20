#include "connect-four.h"

Connect4::Connect4() {
	rows = 6;
	cols = 7;
	availableSpaces = rows * cols;
	round = 1;
	currentTurn = PLAYER2;
	winner = NONE;
	lastMove = { 0, 0 };
	maxDepth = 6;
	try
	{
		board = new int* [rows];
	}
	catch (std::bad_alloc)
	{
		std::cerr << "Could not allocate sufficient space" << std::endl;
		std::exit(1);
	}
	initBoard();
}

Connect4::Connect4(int rows, int cols, int d) {
	this->rows = rows;
	this->cols = cols;
	maxDepth = d;
	availableSpaces = rows * cols;
	round = 1;
	currentTurn = PLAYER2;
	winner = NONE;
	lastMove = { 0, 0 };
	try
	{
		board = new int* [rows];
	}
	catch (std::bad_alloc)
	{
		std::cerr << "Could not allocate sufficient space" << std::endl;
		std::exit(1);
	}
	initBoard();
}

Connect4::~Connect4() {
	for (int i = 0; i < rows; i++)
		delete[] board[i];
	delete[] board;
}

void Connect4::beginGame() {
	std::string actors[] = { "NONE", "PLAYER 1", "PLAYER 2" };
	int choice = 0;

	// Main game loop
	while (!isGoalState()) {
		currentTurn = (currentTurn == PLAYER1) ? PLAYER2 : PLAYER1;
		std::cout << "It is now " << actors[currentTurn] << " turn. Please select a column: ";
		std::cin >> choice;
		int row = nextRow(choice);
		addDisc(row, choice);
		printBoard();
		if (currentTurn == PLAYER1)
			round++;
	}
	std::cout << "And the winner is... " << actors[winner] << "!" << std::endl;
}

void Connect4::beginAIGame() {
	std::string actors[] = { "NONE", "PLAYER 1", "AI" };
	int choice = 0;

	// Main game loop
	while (!isGoalState()) {
		currentTurn = (currentTurn == PLAYER1) ? PLAYER2 : PLAYER1;
		std::cout << "It is now " << actors[currentTurn] << " turn. Please select a column: ";
		if (currentTurn == PLAYER1) {
			std::cin >> choice;
		}
		else {
			std::cout << std::endl;
			choice = getAIMove();
		}
		int row = nextRow(choice);
		addDisc(row, choice);
		printBoard();
		if (currentTurn == PLAYER1)
			round++;
	}
	std::cout << "And the winner is... " << actors[winner] << "!" << std::endl;
}

void Connect4::initBoard() {
	for (int i = 0; i < rows; i++)
		board[i] = new int[cols];

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			board[i][j] = 0;
}

void Connect4::addDisc(int row, int col) {
	// Check if invalid move
	if (!validMove(0, col) || row == -1) {
		std::cout << "Attempted Move: " << row << ", " << col << std::endl;
		std::cout << "Invalid move on board...skipping turn" << std::endl;
		return;
	}
	availableSpaces--;
	board[row][col] = (currentTurn == PLAYER1) ? 1 : 2;
	lastMove = std::pair<int, int>(row, col);
}

void Connect4::addDisc(int row, int col, Actor actor) {
	// Check if invalid move
	if (!validMove(0, col) || row == -1) {
		std::cout << "Attempted Move: " << row << ", " << col << std::endl;
		std::cout << "Invalid move on board...skipping turn" << std::endl;
		return;
	}
	availableSpaces--;
	board[row][col] = actor;
	lastMove = std::pair<int, int>(row, col);
}

void Connect4::removeDisc(int row, int col) {
	if (validMove(row, col)) {
		board[row][col] = 0;
	}
	availableSpaces++;
}

std::string Connect4::repeat(std::string s, int n)
{
	std::string s1 = "";
	for (int i = 0; i < n; i++)
		s1 += s;
	return s1;
}

void Connect4::printBoard() {
	std::string result = "+" + repeat("---+", cols) + "\n";
	for (int i = 0; i < rows; i++) {
		result += "|";
		for (int j = 0; j < cols; j++)
			result += (!board[i][j]) ? "   |" : " " + std::to_string(board[i][j]) + " |";
		result += "\n+" + repeat("---+", cols) + '\n';
	}
	std::cout << result;
}

bool Connect4::isGoalState() {
	int n = 1;

	// Check if the board has filled
	if (availableSpaces == 0)
		return true;

	// Check up
	for (int i = lastMove.first - 1; i >= 0; i--) {
		if (board[i][lastMove.second] != currentTurn)
			break;
		n++;
		if (n >= CONNECT_COUNT) {
			winner = currentTurn;
			return true;
		}
	}

	// Check down
	for (int i = lastMove.first + 1; i < rows; i++) {
		if (board[i][lastMove.second] != currentTurn)
			break;
		n++;
		if (n >= CONNECT_COUNT) {
			winner = currentTurn;
			return true;
		}
	}

	n = 1;

	// Check left
	for (int i = lastMove.second - 1; i >= 0; i--) {
		if (board[lastMove.first][i] != currentTurn)
			break;
		n++;
		if (n >= CONNECT_COUNT) {
			winner = currentTurn;
			return true;
		}
	}

	// Check right
	for (int i = lastMove.second + 1; i < cols; i++) {
		if (board[lastMove.first][i] != currentTurn)
			break;
		n++;
		if (n >= CONNECT_COUNT) {
			winner = currentTurn;
			return true;
		}
	}

	n = 1;

	// Check diagonals
	for (int i = 1; validMove(lastMove.first + i, lastMove.second + i); i++) {
		if (board[lastMove.first + i][lastMove.second + i] != currentTurn)
			break;
		n++;
		if (n >= CONNECT_COUNT) {
			winner = currentTurn;
			return true;
		}
	}

	for (int i = 1; validMove(lastMove.first - i, lastMove.second - i); i++) {
		if (board[lastMove.first - i][lastMove.second - i] != currentTurn)
			break;
		n++;
		if (n >= CONNECT_COUNT) {
			winner = currentTurn;
			return true;
		}
	}

	n = 1;

	for (int i = 1; validMove(lastMove.first - i, lastMove.second + i); i++) {
		if (board[lastMove.first - i][lastMove.second + i] != currentTurn)
			break;
		n++;
		if (n >= CONNECT_COUNT) {
			winner = currentTurn;
			return true;
		}
	}

	for (int i = 1; validMove(lastMove.first + i, lastMove.second - i); i++) {
		if (board[lastMove.first + i][lastMove.second - i] != currentTurn)
			break;
		n++;
		if (n >= CONNECT_COUNT) {
			winner = currentTurn;
			return true;
		}
	}
	return false;
}

bool Connect4::validMove(int row, int col) {
	return (col >= 0) && (col < cols) && (row >= 0) && (row < rows);
}

int Connect4::nextRow(int col) {
	int n = -1;
	for (int i = 0; i < rows; i++)
		if (!board[i][col])
			n = i;
	return n;
}

std::pair<int, int> Connect4::minValue(int alpha, int beta, int depth) {
	std::vector<int> actions = getValidActions();
	if (isGoalState() || depth <= 0) {
		int move = (depth <= 0) ? 0 : actions[0];
		return { utility(), move };
	}
	std::pair<int, int> bestMove = { INT_MAX, -1 };
	for (int move : actions) {
		int row = nextRow(move);
		addDisc(row, move, PLAYER1);
		int newValue = maxValue(alpha, beta, depth - 1).first;
		removeDisc(row, move);
		if (newValue < bestMove.first)
			bestMove = { newValue, move };
		beta = std::min(beta, newValue);
		if (alpha >= beta) break;
	}
	return bestMove;
}

std::pair<int, int> Connect4::maxValue(int alpha, int beta, int depth) {
	std::vector<int> actions = getValidActions();
	if (isGoalState() || depth <= 0) {
		int move = (depth <= 0) ? 0 : actions[0];
		return { utility(), move };
	}
	std::pair<int, int> bestMove = { INT_MIN, -1 };
	for (int move : actions) {
		int row = nextRow(move);
		addDisc(row, move, PLAYER2);
		int newValue = minValue(alpha, beta, depth - 1).first;
		removeDisc(row, move);
		if (newValue > bestMove.first)
			bestMove = { newValue, move };
		alpha = std::max(alpha, newValue);
		if (alpha >= beta) break;
	}
	return bestMove;
}

int Connect4::getAIMove() {
	return miniMax(INT_MIN, INT_MAX);
}

int Connect4::miniMax(int alpha, int beta) {
	if (currentTurn == PLAYER2) {
		std::pair<int, int> value = maxValue(alpha, beta, maxDepth-1);
		winner = NONE;
		return value.second;
	}
	else {
		std::pair<int, int> value = minValue(alpha, beta, maxDepth-1);
		winner = NONE;
		return value.second;
	}
}

std::vector<int> Connect4::getValidActions() {
	std::vector<int> actions;
	for (int i = 0; i < cols; i++) {
		if (!board[0][i]) {
			actions.push_back(i);
		}
	}
	return actions;
}

int Connect4::utility() {
	if (winner == PLAYER1)
		return INT_MIN;
	else if (winner == PLAYER2)
		return INT_MAX;
	else if (availableSpaces == 0)
		return 0;

	return nInARow(PLAYER2) - (nInARow(PLAYER1) * 10);
}

int Connect4::nInARow(Actor actor) {
	int score = 0;

	// Check vertical n in a row
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows - 3; j++) {
			int n = 0;
			int emptyCells = 0;
			bool isBlocked = false;
			for (int k = 0; k < 4; k++) {
				if (board[j + k][i] == actor) n++;
				else if (board[j + k][i] == 0) emptyCells++;
				else {
					isBlocked = true;
					break;
				}
			}
			if (n == 3 && emptyCells == 1 && !isBlocked) score += 100;
			else if (n == 2 && emptyCells == 2 && !isBlocked) score += 10;
		}
	}

	// Check horizontal n in a row
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols - 3; j++) {
			int n = 0;
			int emptyCells = 0;
			bool isBlocked = false;
			for (int k = 0; k < 4; k++) {
				if (board[i][j + k] == actor) n++;
				else if (board[i][j + k] == 0) emptyCells++;
				else {
					isBlocked = true;
					break;
				}
			}
			if (n == 3 && emptyCells == 1 && !isBlocked) score += 100;
			else if (n == 2 && emptyCells == 2 && !isBlocked) score += 10;
		}
	}

	// Check diagonal n in a row
	for (int i = 0; i < rows - 3; i++) {
		for (int j = 0; j < cols - 3; j++) {
			int n = 0;
			int emptyCells = 0;
			bool isBlocked = false;
			for (int k = 0; k < 4; k++) {
				if (board[i + k][j + k] == actor) n++;
				else if (board[i + k][j + k] == 0) emptyCells++;
				else {
					isBlocked = true;
					break;
				}
			}
			if (n == 3 && emptyCells == 1 && !isBlocked) score += 100;
			else if (n == 2 && emptyCells == 2 && !isBlocked) score += 10;

			n = 0;
			emptyCells = 0;
			isBlocked = false;
			for (int k = 0; k < 4; k++) {
				if (board[i + k][j + 3 - k] == actor) n++;
				else if (board[i + k][j + 3 - k] == 0) emptyCells++;
				else {
					isBlocked = true;
					break;
				}
			}
			if (n == 3 && emptyCells == 1 && !isBlocked) score += 100;
			else if (n == 2 && emptyCells == 2 && !isBlocked) score += 10;
		}
	}

	return score;
}