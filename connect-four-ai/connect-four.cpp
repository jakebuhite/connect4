#include "connect-four.h"

Connect4::Connect4() {
	rows = 6;
	cols = 7;
	availableSpaces = rows * cols;
	round = 1;
	currentTurn = PLAYER2;
	winner = NONE;
	lastMove = std::pair<int, int>(-1, -1);
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

Connect4::Connect4(int rows, int cols) {
	this->rows = rows;
	this->cols = cols;
	availableSpaces = rows * cols;
	round = 1;
	currentTurn = PLAYER2;
	winner = NONE;
	lastMove = std::pair<int, int>(-1, -1);
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
		if (addDisc(row, choice))
			lastMove = std::pair<int, int>(row, choice);
		printBoard();
		if (currentTurn == PLAYER1)
			round++;
	}
	printBoard();
	std::cout << "And the winner is... " << actors[winner] << "!" << std::endl;
}

void Connect4::beginGame(MiniMax* agent) {
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
			choice = agent->getAIMove();
		}
		int row = nextRow(choice);
		if (addDisc(row, choice))
			lastMove = std::pair<int, int>(row, choice);
		printBoard();
		if (currentTurn == PLAYER1)
			round++;
	}
	printBoard();
	std::cout << "And the winner is... " << actors[winner] << "!" << std::endl;
}

void Connect4::initBoard() {
	for (int i = 0; i < rows; i++)
		board[i] = new int[cols];

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			board[i][j] = 0;
}

bool Connect4::addDisc(int row, int col) {
	// Check if invalid move
	if (!validMove(0, col) || row == -1) {
		std::cout << "Invalid move on board...skipping turn" << std::endl;
		return false;
	}
	availableSpaces--;
	board[row][col] = (currentTurn == PLAYER1) ? 1 : 2;
	return true;
}

bool Connect4::addDisc(int row, int col, Actor actor) {
	// Check if invalid move
	if (!validMove(0, col) || row == -1) {
		std::cout << "Invalid move on board...skipping turn" << std::endl;
		return false;
	}
	availableSpaces--;
	board[row][col] = actor;
	return true;
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
	int player = (currentTurn == PLAYER1) ? 1 : 2;
	int n = 1;

	// Check if the board has filled
	if (availableSpaces == 0)
		return true;

	// Check up
	for (int i = lastMove.first - 1; i >= 0; i--) {
		if (board[i][lastMove.second] != player)
			break;
		n++;
		if (n >= CONNECT_COUNT) {
			winner = currentTurn;
			return true;
		}
	}

	// Check down
	for (int i = lastMove.first + 1; i < rows; i++) {
		if (board[i][lastMove.second] != player)
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
		if (board[lastMove.first][i] != player)
			break;
		n++;
		if (n >= CONNECT_COUNT) {
			winner = currentTurn;
			return true;
		}
	}

	// Check right
	for (int i = lastMove.second + 1; i < cols; i++) {
		if (board[lastMove.first][i] != player)
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
		if (board[lastMove.first + i][lastMove.second + i] != player)
			break;
		n++;
		if (n >= CONNECT_COUNT) {
			winner = currentTurn;
			return true;
		}
	}
	
	for (int i = 1; validMove(lastMove.first - i, lastMove.second - i); i++) {
		if (board[lastMove.first - i][lastMove.second - i] != player)
			break;
		n++;
		if (n >= CONNECT_COUNT) {
			winner = currentTurn;
			return true;
		}
	}

	n = 1;

	for (int i = 1; validMove(lastMove.first - i, lastMove.second + i); i++) {
		if (board[lastMove.first - i][lastMove.second + i] != player)
			break;
		n++;
		if (n >= CONNECT_COUNT) {
			winner = currentTurn;
			return true;
		}
	}

	for (int i = 1; validMove(lastMove.first + i, lastMove.second - i); i++) {
		if (board[lastMove.first + i][lastMove.second - i] != player)
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

std::vector<int> Connect4::getValidActions() {
	std::vector<int> actions;
	for (int i = 0; i < cols; i++) {
		if (!board[0][i])
			actions.push_back(i);
	}
	return actions;
}

void Connect4::removeDisc(int row, int col) {
	if (!validMove(row, col)) {
		return;
	}
	board[row][col] = 0;
}

int Connect4::getCols() { return cols; }

int Connect4::getRows() { return rows; }

int Connect4::getPos(int row, int col) { return board[row][col]; }

int Connect4::getAvailableSpaces() { return availableSpaces; }

Actor Connect4::getWinner() { return winner; }

Actor Connect4::getCurrentTurn() { return currentTurn; }