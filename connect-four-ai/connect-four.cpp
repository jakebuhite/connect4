#include "connect-four.h"

Connect4::Connect4() {
	rows = 6;
	cols = 7;
	availableSpaces = rows * cols;
	round = 1;
	currentTurn = PLAYER1;
	winner = NONE;
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
	currentTurn = PLAYER1;
	winner = NONE;
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

	// First round. Separate from loop to ensure that the dominant moves aren't chosen
	while (round == 1) {
		std::cout << "It is now " << actors[currentTurn] << " turn. Please select a column: ";
		std::cin >> choice;
		while (choice == 0 || choice == cols / 2 || choice == cols - 1) {
			std::cout << "Please choose a column aside from 0, " << std::to_string(cols / 2) << ", and " << std::to_string(cols - 1) << ": ";
			std::cin >> choice;
		}
		int row = nextRow(choice);
		if (addDisc(row, choice) && isGoalState(row, choice)) {
			winner = currentTurn;
			break;
		}
		printBoard();
		if (currentTurn == PLAYER2)
			round++;
		currentTurn = (currentTurn == PLAYER1) ? PLAYER2 : PLAYER1;
	}

	// Main game loop
	while (availableSpaces > 0) {
		std::cout << "It is now " << actors[currentTurn] << " turn. Please select a column: ";
		std::cin >> choice;
		int row = nextRow(choice);
		if (addDisc(row, choice) && isGoalState(row, choice)) {
			winner = currentTurn;
			break;
		}
		printBoard();
		if (currentTurn == PLAYER2)
			round++;
		currentTurn = (currentTurn == PLAYER1) ? PLAYER2 : PLAYER1;
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
	if (!validMove(0, col) || row == -1) {
		std::cout << "Invalid move on board...skipping turn" << std::endl;
		return false;
	}
	// Valid move, continue
	availableSpaces--;
	board[row][col] = (currentTurn == PLAYER1) ? 1 : 2;
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

bool Connect4::isGoalState(int lastX, int lastY) { // Checking based on the last placed disc rather than the entire board
	int player = (currentTurn == PLAYER1) ? 1 : 2;
	int n = 1;

	// Check up
	for (int i = lastX - 1; i >= 0; i--) {
		if (board[i][lastY] != player)
			break;
		n++;
		if (n >= CONNECT_COUNT) return true;
	}

	// Check down
	for (int i = lastX + 1; i < rows; i++) {
		if (board[i][lastY] != player)
			break;
		n++;
		if (n >= CONNECT_COUNT) return true;
	}

	n = 1;

	// Check left
	for (int i = lastY - 1; i >= 0; i--) {
		if (board[lastX][i] != player)
			break;
		n++;
		if (n >= CONNECT_COUNT) return true;
	}

	// Check right
	for (int i = lastY + 1; i < cols; i++) {
		if (board[lastX][i] != player)
			break;
		n++;
		if (n >= CONNECT_COUNT) return true;
	}

	n = 1;

	// Check diagonals
	for (int i = 1; validMove(lastX + i, lastY + i); i++) {
		if (board[lastX + i][lastY + i] != player)
			break;
		n++;
		if (n >= CONNECT_COUNT) return true;
	}
	
	for (int i = 1; validMove(lastX - i, lastY - i); i++) {
		if (board[lastX - i][lastY - i] != player)
			break;
		n++;
		if (n >= CONNECT_COUNT) return true;
	}

	n = 1;

	for (int i = 1; validMove(lastX - i, lastY + i); i++) {
		if (board[lastX - i][lastY + i] != player)
			break;
		n++;
		if (n >= CONNECT_COUNT) return true;
	}

	for (int i = 1; validMove(lastX + i, lastY - i); i++) {
		if (board[lastX + i][lastY - i] != player)
			break;
		n++;
		if (n >= CONNECT_COUNT) return true;
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
