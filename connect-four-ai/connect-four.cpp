// 
// connect-four.cpp
// Jake Buhite and Nick Abegg
// 10/26/2023
//
#include "connect-four.h"

Connect4::Connect4() { initBoard(); }

Connect4::Connect4(int rows, int cols) {
	this->rows = rows;
	this->cols = cols;
	availableSpaces = rows * cols;
	initBoard();
}

Connect4::~Connect4() {
	for (int i = 0; i < rows; i++)
		delete[] board[i];
	delete[] board;
}

void Connect4::initBoard() {
	// Initialize rows
	try
	{
		board = new int* [rows];
	}
	catch (std::bad_alloc)
	{
		std::cerr << "Could not allocate sufficient space" << std::endl;
		std::exit(1);
	}

	// Initialize cols
	for (int i = 0; i < rows; i++)
		board[i] = new int[cols];

	// Mark the entire board as empty
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
	lastMove = { row, col };
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
	lastMove = { row, col };
}

void Connect4::removeDisc(int row, int col) {
	if (validMove(row, col))
		board[row][col] = 0;
	availableSpaces++;
}

void Connect4::printBoard() {
	std::string result = "+" + repeat("---+", cols) + "\n";
	for (int i = 0; i < rows; i++) {
		result += "|";
		for (int j = 0; j < cols; j++)
			result += (!board[i][j]) ? "   |" : " " + std::to_string(board[i][j]) + " |";
		result += "\n+" + repeat("---+", cols) + '\n';
	}
	result += "| 0 | 1 | 2 | 3 | 4 | 5 | 6 |\n";
	std::cout << result;
}

int Connect4::nextRow(int col) {
	int n = -1;
	for (int i = 0; i < rows; i++)
		if (!board[i][col])
			n = i;
	return n;
}

bool Connect4::validMove(int row, int col) { return (col >= 0) && (col < cols) && (row >= 0) && (row < rows); }

bool Connect4::isDominateMove(int col) { return round == 1 && (col == 0 || (cols % 2 == 1 && col == cols / 2) || col == cols - 1); }

bool Connect4::isGoalState() {
	int n = 0;
	int row = lastMove.first;
	int col = lastMove.second;

	// Check horizontally
	int start = std::max(col - 3, 0);
	int end = std::min(cols - 4, col) + 1;
	for (int i = start; i < end; i++) {
		int cell = board[row][i];
		if (cell != 0 && cell == board[row][i + 1] && cell == board[row][i + 2] && cell == board[row][i + 3]) 
			return true;
	}

	// Check vertically
	start = std::max(row - 3, 0);
	end = std::min(rows - 4, col) + 1;
	for (int i = start; i < end; i++) {
		int cell = board[i][col];
		if (cell != 0 && cell == board[i + 1][col] && cell == board[i + 2][col] && cell == board[i + 3][col])
			return true;
	}

	// Check diagonally (left to right)
	for (int i = 0; i <= 3; i++) {
		if (validMove(row + i, col + i) && validMove(row + i - 3, col + i - 3)) {
			int cell = board[row + i][col + i];
			if (cell != 0 && cell == board[row + i - 1][col + i - 1] && cell == board[row + i - 2][col + i - 2] && cell == board[row + i - 3][col + i - 3])
				return true;
		}
	}

	// Check diagonally (right to left)
	for (int i = 0; i <= 3; i++) {
		if (validMove(row + i, col - i) && validMove(row + i - 3, col - i + 3)) {
			int cell = board[row + i][col - i];
			if (cell != 0 && cell == board[row + i - 1][col - i + 1] && cell == board[row + i - 2][col - i + 2] && cell == board[row + i - 3][col - i + 3])
				return true;
		}
	}

	return false;
}

std::string Connect4::repeat(std::string s, int n) {
	std::string ans = "";
	for (int i = 0; i < n; i++)
		ans += s;
	return ans;
}

void Connect4::incrementRound() { round++; }

void Connect4::setWinner(Actor a) { winner = a; }

Actor Connect4::getWinner() { return winner; }

Actor Connect4::getCurrentTurn() { return currentTurn; }

void Connect4::setCurrentTurn(Actor a) { currentTurn = a; }

int Connect4::getAvailableSpaces() { return availableSpaces; }

int Connect4::getCell(int x, int y) { return board[x][y]; }

int Connect4::getRows() { return rows; }

int Connect4::getCols() { return cols; }