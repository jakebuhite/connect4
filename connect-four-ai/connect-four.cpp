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

void Connect4::setBoard(int** board) {
	resetBoard();
	for (int j = 0; j < rows; j++) {
		for (int i = 0; i < cols; i++) {
			this->board[j][i] = board[j][i];
		}
	}

	// Update availableSpaces based on the last filled row in each column
	for (int i = 0; i < cols; i++) {
		availableSpaces -= rows - (nextRow(i) + 1);
	}
}

int** Connect4::getBoard() { return board; }

void Connect4::resetBoard() {
	for (int i = 0; i < rows; i++)
		delete[] board[i];
	delete[] board;
	initBoard();
	availableSpaces = 42;
}

int** Connect4::getMirroredField(int** board) {
    int** mirroredField = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        mirroredField[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            mirroredField[i][j] = board[rows - i - 1][j];
        }
    }
    return mirroredField;
}

bool Connect4::isDraw() { 
	return availableSpaces <= 0; 
}

bool Connect4::canWin(int player, int col, int row) {
	switch (row * 7 + col) {
	case 35:
		if (isMatchingBoard(player, { 29, 23, 17 }) || isMatchingBoard(player, { 36, 37, 38 }))
			return true;
		break;
	case 28:
		if (isMatchingBoard(player, { 29, 30, 31 }) || isMatchingBoard(player, { 22, 16, 10 }))
			return true;
		break;
	case 21:
		if (isMatchingBoard(player, { 22, 23, 24 }) || isMatchingBoard(player, { 15, 9, 3 }))
			return true;
		break;
	case 14:
		if (isMatchingBoard(player, { 35, 28, 21 }) || isMatchingBoard(player, { 22, 30, 38 }) || isMatchingBoard(player, { 15, 16, 17 }))
			return true;
		break;
	case 7:
		if (isMatchingBoard(player, { 15, 23, 31 }) || isMatchingBoard(player, { 28, 21, 14 }) || isMatchingBoard(player, { 8, 9, 10 }))
			return true;
		break;
	case 0:
		if (isMatchingBoard(player, { 8, 16, 24 }) || isMatchingBoard(player, { 21, 14, 7 }) || isMatchingBoard(player, { 1, 2, 3 }))
			return true;
		break;
	case 36:
		if (isMatchingBoard(player, { 30, 24, 18 }) || isMatchingBoard(player, { 37, 38, 39 }) || isMatchingBoard(player, { 35, 37, 38 }))
			return true;
		break;
	case 29:
		if (isMatchingBoard(player, { 28, 30, 31 }) || isMatchingBoard(player, { 35, 23, 17 })
			|| isMatchingBoard(player, { 30, 31, 32 }) || isMatchingBoard(player, { 23, 17, 11 }))
			return true;
		break;
	case 22:
		if (isMatchingBoard(player, { 21, 23, 24 }) || isMatchingBoard(player, { 14, 30, 38 })
			|| isMatchingBoard(player, { 23, 24, 25 }) || isMatchingBoard(player, { 28, 16, 10 }) || isMatchingBoard(player, { 16, 10, 4 }))
			return true;
		break;
	case 15:
		if (isMatchingBoard(player, { 23, 31, 39 }) || isMatchingBoard(player, { 36, 29, 22 })
			|| isMatchingBoard(player, { 16, 17, 18 }) || isMatchingBoard(player, { 14, 16, 17 })
			|| isMatchingBoard(player, { 21, 9, 3 }) || isMatchingBoard(player, { 7, 23, 31 }))
			return true;
		break;
	case 8:
		if (isMatchingBoard(player, { 16, 24, 32 }) || isMatchingBoard(player, { 9, 10, 11 })
			|| isMatchingBoard(player, { 29, 22, 15 }) || isMatchingBoard(player, { 7, 9, 10 }) || isMatchingBoard(player, { 0, 16, 24 }))
			return true;
		break;
	case 1:
		if (isMatchingBoard(player, { 9, 17, 25 }) || isMatchingBoard(player, { 2, 3, 4 })
			|| isMatchingBoard(player, { 22, 15, 8 }) || isMatchingBoard(player, { 0, 2, 3 }))
			return true;
		break;
	case 37:
		if (isMatchingBoard(player, { 31, 25, 19 }) || isMatchingBoard(player, { 35, 36, 38 })
			|| isMatchingBoard(player, { 38, 39, 40 }) || isMatchingBoard(player, { 36, 38, 39 }))
			return true;
		break;
	case 30:
		if (isMatchingBoard(player, { 14, 22, 38 }) || isMatchingBoard(player, { 28, 29, 31 })
			|| isMatchingBoard(player, { 36, 24, 18 }) || isMatchingBoard(player, { 31, 32, 33 })
			|| isMatchingBoard(player, { 29, 31, 32 }) || isMatchingBoard(player, { 24, 18, 12 }))
			return true;
		break;
	case 23:
		if (isMatchingBoard(player, { 15, 31, 39 }) || isMatchingBoard(player, { 21, 22, 24 })
			|| isMatchingBoard(player, { 35, 29, 17 }) || isMatchingBoard(player, { 22, 24, 25 })
			|| isMatchingBoard(player, { 24, 25, 26 }) || isMatchingBoard(player, { 29, 17, 11 })
			|| isMatchingBoard(player, { 7, 15, 31 }) || isMatchingBoard(player, { 17, 11, 5 }))
			return true;
		break;
	case 16:
		if (isMatchingBoard(player, { 24, 32, 40 }) || isMatchingBoard(player, { 28, 22, 10 })
			|| isMatchingBoard(player, { 14, 15, 17 }) || isMatchingBoard(player, { 15, 17, 18 })
			|| isMatchingBoard(player, { 22, 10, 4 }) || isMatchingBoard(player, { 8, 24, 32 })
			|| isMatchingBoard(player, { 37, 30, 23 }) || isMatchingBoard(player, { 17, 18, 19 }) || isMatchingBoard(player, { 0, 8, 24 }))
			return true;
		break;
	case 9:
		if (isMatchingBoard(player, { 17, 25, 33 }) || isMatchingBoard(player, { 21, 15, 3 })
			|| isMatchingBoard(player, { 8, 10, 11 }) || isMatchingBoard(player, { 1, 17, 25 })
			|| isMatchingBoard(player, { 10, 11, 12 }) || isMatchingBoard(player, { 30, 23, 16 }) || isMatchingBoard(player, { 7, 8, 10 }))
			return true;
		break;
	case 2:
		if (isMatchingBoard(player, { 10, 18, 26 }) || isMatchingBoard(player, { 23, 16, 9 })
			|| isMatchingBoard(player, { 1, 3, 4 }) || isMatchingBoard(player, { 3, 4, 5 }) || isMatchingBoard(player, { 0, 1, 3 }))
			return true;
		break;
	case 38:
		if (isMatchingBoard(player, { 35, 36, 37 }) || isMatchingBoard(player, { 36, 37, 39 })
			|| isMatchingBoard(player, { 37, 39, 40 }) || isMatchingBoard(player, { 39, 40, 41 })
			|| isMatchingBoard(player, { 32, 26, 20 }) || isMatchingBoard(player, { 14, 22, 30 }))
			return true;
		break;
	case 31:
		if (isMatchingBoard(player, { 37, 25, 19 }) || isMatchingBoard(player, { 32, 33, 34 })
			|| isMatchingBoard(player, { 28, 29, 30 }) || isMatchingBoard(player, { 7, 15, 23 })
			|| isMatchingBoard(player, { 15, 23, 39 }) || isMatchingBoard(player, { 30, 32, 33 })
			|| isMatchingBoard(player, { 25, 19, 13 }) || isMatchingBoard(player, { 29, 30, 32 }))
			return true;
		break;
	case 24:
		if (isMatchingBoard(player, { 36, 30, 18 }) || isMatchingBoard(player, { 16, 32, 40 })
			|| isMatchingBoard(player, { 21, 22, 23 }) || isMatchingBoard(player, { 22, 23, 25 })
			|| isMatchingBoard(player, { 23, 25, 26 }) || isMatchingBoard(player, { 25, 26, 27 })
			|| isMatchingBoard(player, { 8, 16, 32 }) || isMatchingBoard(player, { 30, 18, 12 })
			|| isMatchingBoard(player, { 18, 12, 6 }) || isMatchingBoard(player, { 0, 8, 16 }))
			return true;
		break;
	case 17:
		if (isMatchingBoard(player, { 35, 29, 23 }) || isMatchingBoard(player, { 25, 33, 41 })
			|| isMatchingBoard(player, { 38, 31, 24 }) || isMatchingBoard(player, { 14, 15, 16 })
			|| isMatchingBoard(player, { 9, 25, 33 }) || isMatchingBoard(player, { 29, 23, 11 })
			|| isMatchingBoard(player, { 16, 18, 19 }) || isMatchingBoard(player, { 15, 16, 18 })
			|| isMatchingBoard(player, { 1, 9, 25 }) || isMatchingBoard(player, { 23, 11, 5 }) || isMatchingBoard(player, { 18, 19, 20 }))
			return true;
		break;
	case 10:
		if (isMatchingBoard(player, { 28, 22, 16 }) || isMatchingBoard(player, { 31, 24, 17 })
			|| isMatchingBoard(player, { 18, 26, 34 }) || isMatchingBoard(player, { 22, 16, 4 })
			|| isMatchingBoard(player, { 7, 8, 9 }) || isMatchingBoard(player, { 2, 18, 26 })
			|| isMatchingBoard(player, { 8, 9, 11 }) || isMatchingBoard(player, { 9, 11, 12 }) || isMatchingBoard(player, { 11, 12, 13 }))
			return true;
		break;
	case 3:
		if (isMatchingBoard(player, { 24, 17, 10 }) || isMatchingBoard(player, { 21, 15, 9 })
			|| isMatchingBoard(player, { 11, 19, 27 }) || isMatchingBoard(player, { 0, 1, 2 })
			|| isMatchingBoard(player, { 1, 2, 4 }) || isMatchingBoard(player, { 2, 4, 5 }) || isMatchingBoard(player, { 4, 5, 6 }))
			return true;
		break;
	case 39:
		if (isMatchingBoard(player, { 15, 23, 31 }) || isMatchingBoard(player, { 38, 40, 41 })
			|| isMatchingBoard(player, { 37, 38, 40 }) || isMatchingBoard(player, { 36, 37, 38 }))
			return true;
		break;
	case 32:
		if (isMatchingBoard(player, { 38, 26, 20 }) || isMatchingBoard(player, { 16, 24, 40 })
			|| isMatchingBoard(player, { 31, 33, 34 }) || isMatchingBoard(player, { 30, 31, 33 })
			|| isMatchingBoard(player, { 29, 30, 31 }) || isMatchingBoard(player, { 8, 16, 24 }))
			return true;
		break;
	case 25:
		if (isMatchingBoard(player, { 17, 33, 41 }) || isMatchingBoard(player, { 37, 31, 19 })
			|| isMatchingBoard(player, { 23, 24, 26 }) || isMatchingBoard(player, { 22, 23, 24 })
			|| isMatchingBoard(player, { 24, 26, 27 }) || isMatchingBoard(player, { 9, 17, 33 })
			|| isMatchingBoard(player, { 31, 19, 13 }) || isMatchingBoard(player, { 1, 9, 17 }))
			return true;
		break;
	case 18:
		if (isMatchingBoard(player, { 36, 30, 24 }) || isMatchingBoard(player, { 10, 26, 34 })
			|| isMatchingBoard(player, { 39, 32, 25 }) || isMatchingBoard(player, { 16, 17, 19 })
			|| isMatchingBoard(player, { 15, 16, 17 }) || isMatchingBoard(player, { 30, 24, 12 })
			|| isMatchingBoard(player, { 2, 10, 26 }) || isMatchingBoard(player, { 17, 19, 20 }) || isMatchingBoard(player, { 24, 12, 6 }))
			return true;
		break;
	case 11:
		if (isMatchingBoard(player, { 29, 23, 17 }) || isMatchingBoard(player, { 3, 19, 27 })
			|| isMatchingBoard(player, { 32, 25, 18 }) || isMatchingBoard(player, { 9, 10, 12 })
			|| isMatchingBoard(player, { 8, 9, 10 }) || isMatchingBoard(player, { 23, 17, 5 }) || isMatchingBoard(player, { 10, 12, 13 }))
			return true;
		break;
	case 4:
		if (isMatchingBoard(player, { 22, 16, 10 }) || isMatchingBoard(player, { 25, 18, 11 })
			|| isMatchingBoard(player, { 2, 3, 5 }) || isMatchingBoard(player, { 1, 2, 3 }) || isMatchingBoard(player, { 3, 5, 6 }))
			return true;
		break;
	case 40:
		if (isMatchingBoard(player, { 16, 24, 32 }) || isMatchingBoard(player, { 37, 38, 39 }) || isMatchingBoard(player, { 38, 39, 41 }))
			return true;
		break;
	case 33:
		if (isMatchingBoard(player, { 30, 31, 32 }) || isMatchingBoard(player, { 31, 32, 34 })
			|| isMatchingBoard(player, { 17, 25, 41 }) || isMatchingBoard(player, { 9, 17, 25 }))
			return true;
		break;
	case 26:
		if (isMatchingBoard(player, { 23, 24, 25 }) || isMatchingBoard(player, { 24, 25, 27 })
			|| isMatchingBoard(player, { 10, 18, 34 }) || isMatchingBoard(player, { 38, 32, 20 }) || isMatchingBoard(player, { 2, 10, 18 }))
			return true;
		break;
	case 19:
		if (isMatchingBoard(player, { 37, 31, 25 }) || isMatchingBoard(player, { 16, 17, 18 })
			|| isMatchingBoard(player, { 40, 33, 26 }) || isMatchingBoard(player, { 17, 18, 20 })
			|| isMatchingBoard(player, { 3, 11, 27 }) || isMatchingBoard(player, { 31, 25, 13 }))
			return true;
		break;
	case 12:
		if (isMatchingBoard(player, { 30, 24, 18 }) || isMatchingBoard(player, { 9, 10, 11 })
			|| isMatchingBoard(player, { 33, 26, 19 }) || isMatchingBoard(player, { 10, 11, 13 }) || isMatchingBoard(player, { 24, 18, 6 }))
			return true;
		break;
	case 5:
		if (isMatchingBoard(player, { 23, 17, 11 }) || isMatchingBoard(player, { 2, 3, 4 })
			|| isMatchingBoard(player, { 26, 19, 12 }) || isMatchingBoard(player, { 3, 4, 6 }))
			return true;
		break;
	case 41:
		if (isMatchingBoard(player, { 17, 25, 33 }) || isMatchingBoard(player, { 38, 39, 40 }))
			return true;
		break;
	case 34:
		if (isMatchingBoard(player, { 31, 32, 33 }) || isMatchingBoard(player, { 10, 18, 26 }))
			return true;
		break;
	case 27:
		if (isMatchingBoard(player, { 24, 25, 26 }) || isMatchingBoard(player, { 3, 11, 19 }))
			return true;
		break;
	case 20:
		if (isMatchingBoard(player, { 38, 32, 26 }) || isMatchingBoard(player, { 41, 34, 27 }) || isMatchingBoard(player, { 17, 18, 19 }))
			return true;
		break;
	case 13:
		if (isMatchingBoard(player, { 31, 25, 19 }) || isMatchingBoard(player, { 34, 27, 20 }) || isMatchingBoard(player, { 10, 11, 12 }))
			return true;
		break;
	case 6:
		if (isMatchingBoard(player, { 24, 18, 12 }) || isMatchingBoard(player, { 27, 20, 13 }) || isMatchingBoard(player, { 3, 4, 5 }))
			return true;
		break;
	}
	return false;
}

std::vector<int> Connect4::generateTDLMoves(int player) {
	int p = (player == PLAYER1) ? PLAYER2 : PLAYER1;
	std::vector<int> cn(7);
	int count;
	int i;
	int j = 0;
	int index = 0;

	switch (nextRow(0)) {
	case 5:
		count = 7;
		if (!isMatchingBoard(p, { 35, 29, 23, 17 }))
			count--;
		if (!isMatchingBoard(p, { 35, 36, 37, 38 }))
			count--;
		if (!isMatchingBoard(p, { 35, 28, 21, 14 }))
			count--;
		break;
	case 4:
		count = 9;
		if (!isMatchingBoard(p, { 28, 22, 16, 10 }))
			count--;
		if (!isMatchingBoard(p, { 28, 29, 30, 31 }))
			count--;
		if (!isMatchingBoard(p, { 35, 28, 21, 14 }))
			count--;
		if (!isMatchingBoard(p, { 28, 21, 14, 7 }))
			count--;
		break;
	case 3:
		count = 11;
		if (!isMatchingBoard(p, { 21, 15, 9, 3 }))
			count--;
		if (!isMatchingBoard(p, { 21, 22, 23, 24 }))
			count--;
		if (!isMatchingBoard(p, { 35, 28, 21, 14 }))
			count--;
		if (!isMatchingBoard(p, { 28, 21, 14, 7 }))
			count--;
		if (!isMatchingBoard(p, { 21, 14, 7, 0 }))
			count--;
		break;
	case 2:
		count = 10;
		if (!isMatchingBoard(p, { 14, 15, 16, 17 }))
			count--;
		if (!isMatchingBoard(p, { 14, 22, 30, 38 }))
			count--;
		if (!isMatchingBoard(p, { 35, 28, 21, 14 }))
			count--;
		if (!isMatchingBoard(p, { 28, 21, 14, 7 }))
			count--;
		if (!isMatchingBoard(p, { 21, 14, 7, 0 }))
			count--;
		break;
	case 1:
		count = 8;
		if (!isMatchingBoard(p, { 7, 8, 9, 10 }))
			count--;
		if (!isMatchingBoard(p, { 7, 15, 23, 31 }))
			count--;
		if (!isMatchingBoard(p, { 28, 21, 14, 7 }))
			count--;
		if (!isMatchingBoard(p, { 21, 14, 7, 0 }))
			count--;
		break;
	case 0:
		count = 6;
		if (!isMatchingBoard(p, { 0, 1, 2, 3 }))
			count--;
		if (!isMatchingBoard(p, { 0, 8, 16, 24 }))
			count--;
		if (!isMatchingBoard(p, { 21, 14, 7, 0 }))
			count--;
		break;
	default: count = 0;
	}
	cn[0] = count;

	switch (nextRow(1)) {
	case 5:
		count = 9;
		if (!isMatchingBoard(p, { 36, 30, 24, 18 }))
			count--;
		if (!isMatchingBoard(p, { 35, 36, 37, 38 }))
			count--;
		if (!isMatchingBoard(p, { 36, 37, 38, 39 }))
			count--;
		if (!isMatchingBoard(p, { 36, 29, 22, 15 }))
			count--;
		break;
	case 4:
		count = 13;
		if (!isMatchingBoard(p, { 29, 23, 17, 11 }))
			count--;
		if (!isMatchingBoard(p, { 35, 29, 23, 17 }))
			count--;
		if (!isMatchingBoard(p, { 28, 29, 30, 31 }))
			count--;
		if (!isMatchingBoard(p, { 29, 30, 31, 32 }))
			count--;
		if (!isMatchingBoard(p, { 36, 29, 22, 15 }))
			count--;
		if (!isMatchingBoard(p, { 29, 22, 15, 8 }))
			count--;
		break;
	case 3:
		count = 17;
		if (!isMatchingBoard(p, { 22, 16, 10, 4 }))
			count--;
		if (!isMatchingBoard(p, { 28, 22, 16, 10 }))
			count--;
		if (!isMatchingBoard(p, { 21, 22, 23, 24 }))
			count--;
		if (!isMatchingBoard(p, { 22, 23, 24, 25 }))
			count--;
		if (!isMatchingBoard(p, { 14, 22, 30, 38 }))
			count--;
		if (!isMatchingBoard(p, { 36, 29, 22, 15 }))
			count--;
		if (!isMatchingBoard(p, { 29, 22, 15, 8 }))
			count--;
		if (!isMatchingBoard(p, { 22, 15, 8, 1 }))
			count--;
		break;
	case 2:
		count = 16;
		if (!isMatchingBoard(p, { 21, 15, 9, 3 }))
			count--;
		if (!isMatchingBoard(p, { 14, 15, 16, 17 }))
			count--;
		if (!isMatchingBoard(p, { 15, 16, 17, 18 }))
			count--;
		if (!isMatchingBoard(p, { 7, 15, 23, 31 }))
			count--;
		if (!isMatchingBoard(p, { 15, 23, 31, 39 }))
			count--;
		if (!isMatchingBoard(p, { 36, 29, 22, 15 }))
			count--;
		if (!isMatchingBoard(p, { 29, 22, 15, 8 }))
			count--;
		if (!isMatchingBoard(p, { 22, 15, 8, 1 }))
			count--;
		break;
	case 1:
		count = 12;
		if (!isMatchingBoard(p, { 7, 8, 9, 10 }))
			count--;
		if (!isMatchingBoard(p, { 8, 9, 10, 11 }))
			count--;
		if (!isMatchingBoard(p, { 0, 8, 16, 24 }))
			count--;
		if (!isMatchingBoard(p, { 8, 16, 24, 32 }))
			count--;
		if (!isMatchingBoard(p, { 29, 22, 15, 8 }))
			count--;
		if (!isMatchingBoard(p, { 22, 15, 8, 1 }))
			count--;
		break;
	case 0:
		count = 8;
		if (!isMatchingBoard(p, { 0, 1, 2, 3 }))
			count--;
		if (!isMatchingBoard(p, { 1, 2, 3, 4 }))
			count--;
		if (!isMatchingBoard(p, { 1, 9, 17, 25 }))
			count--;
		if (!isMatchingBoard(p, { 22, 15, 8, 1 }))
			count--;
		break;
	default: count = 0;
	}
	cn[1] = count;

	switch (nextRow(2)) {
	case 5:
		count = 11;
		if (!isMatchingBoard(p, { 37, 31, 25, 19 }))
			count--;
		if (!isMatchingBoard(p, { 35, 36, 37, 38 }))
			count--;
		if (!isMatchingBoard(p, { 36, 37, 38, 39 }))
			count--;
		if (!isMatchingBoard(p, { 37, 38, 39, 40 }))
			count--;
		if (!isMatchingBoard(p, { 37, 30, 23, 16 }))
			count--;
		break;
	case 4:
		count = 17;
		if (!isMatchingBoard(p, { 30, 24, 18, 12 }))
			count--;
		if (!isMatchingBoard(p, { 14, 22, 30, 38 }))
			count--;
		if (!isMatchingBoard(p, { 36, 30, 24, 18 }))
			count--;
		if (!isMatchingBoard(p, { 28, 29, 30, 31 }))
			count--;
		if (!isMatchingBoard(p, { 29, 30, 31, 32 }))
			count--;
		if (!isMatchingBoard(p, { 30, 31, 32, 33 }))
			count--;
		if (!isMatchingBoard(p, { 37, 30, 23, 16 }))
			count--;
		if (!isMatchingBoard(p, { 30, 23, 16, 9 }))
			count--;
		break;
	case 3:
		count = 23;
		if (!isMatchingBoard(p, { 23, 17, 11, 5 }))
			count--;
		if (!isMatchingBoard(p, { 7, 15, 23, 31 }))
			count--;
		if (!isMatchingBoard(p, { 29, 23, 17, 11 }))
			count--;
		if (!isMatchingBoard(p, { 21, 22, 23, 24 }))
			count--;
		if (!isMatchingBoard(p, { 22, 23, 24, 25 }))
			count--;
		if (!isMatchingBoard(p, { 23, 24, 25, 26 }))
			count--;
		if (!isMatchingBoard(p, { 15, 23, 31, 39 }))
			count--;
		if (!isMatchingBoard(p, { 35, 29, 23, 17 }))
			count--;
		if (!isMatchingBoard(p, { 37, 30, 23, 16 }))
			count--;
		if (!isMatchingBoard(p, { 30, 23, 16, 9 }))
			count--;
		if (!isMatchingBoard(p, { 23, 16, 9, 2 }))
			count--;
		break;
	case 2:
		count = 22;
		if (!isMatchingBoard(p, { 0, 8, 16, 24 }))
			count--;
		if (!isMatchingBoard(p, { 22, 16, 10, 4 }))
			count--;
		if (!isMatchingBoard(p, { 14, 15, 16, 17 }))
			count--;
		if (!isMatchingBoard(p, { 15, 16, 17, 18 }))
			count--;
		if (!isMatchingBoard(p, { 16, 17, 18, 19 }))
			count--;
		if (!isMatchingBoard(p, { 8, 16, 24, 32 }))
			count--;
		if (!isMatchingBoard(p, { 28, 22, 16, 10 }))
			count--;
		if (!isMatchingBoard(p, { 16, 24, 32, 40 }))
			count--;
		if (!isMatchingBoard(p, { 37, 30, 23, 16 }))
			count--;
		if (!isMatchingBoard(p, { 30, 23, 16, 9 }))
			count--;
		if (!isMatchingBoard(p, { 23, 16, 9, 2 }))
			count--;
		break;
	case 1:
		count = 16;
		if (!isMatchingBoard(p, { 7, 8, 9, 10 }))
			count--;
		if (!isMatchingBoard(p, { 8, 9, 10, 11 }))
			count--;
		if (!isMatchingBoard(p, { 9, 10, 11, 12 }))
			count--;
		if (!isMatchingBoard(p, { 1, 9, 17, 25 }))
			count--;
		if (!isMatchingBoard(p, { 21, 15, 9, 3 }))
			count--;
		if (!isMatchingBoard(p, { 9, 17, 25, 33 }))
			count--;
		if (!isMatchingBoard(p, { 30, 23, 16, 9 }))
			count--;
		if (!isMatchingBoard(p, { 23, 16, 9, 2 }))
			count--;
		break;
	case 0:
		count = 10;
		if (!isMatchingBoard(p, { 0, 1, 2, 3 }))
			count--;
		if (!isMatchingBoard(p, { 1, 2, 3, 4 }))
			count--;
		if (!isMatchingBoard(p, { 2, 3, 4, 5 }))
			count--;
		if (!isMatchingBoard(p, { 2, 10, 18, 26 }))
			count--;
		if (!isMatchingBoard(p, { 23, 16, 9, 2 }))
			count--;
		break;
	default: count = 0;
	}
	cn[2] = count;

	switch (nextRow(3)) {
	case 5:
		count = 15;
		if (!isMatchingBoard(p, { 14, 22, 30, 38 }))
			count--;
		if (!isMatchingBoard(p, { 38, 32, 26, 20 }))
			count--;
		if (!isMatchingBoard(p, { 35, 36, 37, 38 }))
			count--;
		if (!isMatchingBoard(p, { 36, 37, 38, 39 }))
			count--;
		if (!isMatchingBoard(p, { 37, 38, 39, 40 }))
			count--;
		if (!isMatchingBoard(p, { 38, 39, 40, 41 }))
			count--;
		if (!isMatchingBoard(p, { 38, 31, 24, 17 }))
			count--;
		break;
	case 4:
		count = 21;
		if (!isMatchingBoard(p, { 7, 15, 23, 31 }))
			count--;
		if (!isMatchingBoard(p, { 31, 25, 19, 13 }))
			count--;
		if (!isMatchingBoard(p, { 15, 23, 31, 39 }))
			count--;
		if (!isMatchingBoard(p, { 37, 31, 25, 19 }))
			count--;
		if (!isMatchingBoard(p, { 28, 29, 30, 31 }))
			count--;
		if (!isMatchingBoard(p, { 29, 30, 31, 32 }))
			count--;
		if (!isMatchingBoard(p, { 30, 31, 32, 33 }))
			count--;
		if (!isMatchingBoard(p, { 31, 32, 33, 34 }))
			count--;
		if (!isMatchingBoard(p, { 38, 31, 24, 17 }))
			count--;
		if (!isMatchingBoard(p, { 31, 24, 17, 10 }))
			count--;
		break;
	case 3:
		count = 27;
		if (!isMatchingBoard(p, { 0, 8, 16, 24 }))
			count--;
		if (!isMatchingBoard(p, { 24, 18, 12, 6 }))
			count--;
		if (!isMatchingBoard(p, { 8, 16, 24, 32 }))
			count--;
		if (!isMatchingBoard(p, { 30, 24, 18, 12 }))
			count--;
		if (!isMatchingBoard(p, { 21, 22, 23, 24 }))
			count--;
		if (!isMatchingBoard(p, { 22, 23, 24, 25 }))
			count--;
		if (!isMatchingBoard(p, { 23, 24, 25, 26 }))
			count--;
		if (!isMatchingBoard(p, { 24, 25, 26, 27 }))
			count--;
		if (!isMatchingBoard(p, { 16, 24, 32, 40 }))
			count--;
		if (!isMatchingBoard(p, { 36, 30, 24, 18 }))
			count--;
		if (!isMatchingBoard(p, { 38, 31, 24, 17 }))
			count--;
		if (!isMatchingBoard(p, { 31, 24, 17, 10 }))
			count--;
		if (!isMatchingBoard(p, { 24, 17, 10, 3 }))
			count--;
		break;
	case 2:
		count = 26;
		if (!isMatchingBoard(p, { 1, 9, 17, 25 }))
			count--;
		if (!isMatchingBoard(p, { 23, 17, 11, 5 }))
			count--;
		if (!isMatchingBoard(p, { 14, 15, 16, 17 }))
			count--;
		if (!isMatchingBoard(p, { 15, 16, 17, 18 }))
			count--;
		if (!isMatchingBoard(p, { 16, 17, 18, 19 }))
			count--;
		if (!isMatchingBoard(p, { 17, 18, 19, 20 }))
			count--;
		if (!isMatchingBoard(p, { 9, 17, 25, 33 }))
			count--;
		if (!isMatchingBoard(p, { 29, 23, 17, 11 }))
			count--;
		if (!isMatchingBoard(p, { 17, 25, 33, 41 }))
			count--;
		if (!isMatchingBoard(p, { 35, 29, 23, 17 }))
			count--;
		if (!isMatchingBoard(p, { 38, 31, 24, 17 }))
			count--;
		if (!isMatchingBoard(p, { 31, 24, 17, 10 }))
			count--;
		if (!isMatchingBoard(p, { 24, 17, 10, 3 }))
			count--;
		break;
	case 1:
		count = 20;
		if (!isMatchingBoard(p, { 7, 8, 9, 10 }))
			count--;
		if (!isMatchingBoard(p, { 8, 9, 10, 11 }))
			count--;
		if (!isMatchingBoard(p, { 9, 10, 11, 12 }))
			count--;
		if (!isMatchingBoard(p, { 10, 11, 12, 13 }))
			count--;
		if (!isMatchingBoard(p, { 2, 10, 18, 26 }))
			count--;
		if (!isMatchingBoard(p, { 22, 16, 10, 4 }))
			count--;
		if (!isMatchingBoard(p, { 10, 18, 26, 34 }))
			count--;
		if (!isMatchingBoard(p, { 28, 22, 16, 10 }))
			count--;
		if (!isMatchingBoard(p, { 31, 24, 17, 10 }))
			count--;
		if (!isMatchingBoard(p, { 24, 17, 10, 3 }))
			count--;
		break;
	case 0:
		count = 14;
		if (!isMatchingBoard(p, { 0, 1, 2, 3 }))
			count--;
		if (!isMatchingBoard(p, { 1, 2, 3, 4 }))
			count--;
		if (!isMatchingBoard(p, { 2, 3, 4, 5 }))
			count--;
		if (!isMatchingBoard(p, { 3, 4, 5, 6 }))
			count--;
		if (!isMatchingBoard(p, { 3, 11, 19, 27 }))
			count--;
		if (!isMatchingBoard(p, { 21, 15, 9, 3 }))
			count--;
		if (!isMatchingBoard(p, { 24, 17, 10, 3 }))
			count--;
		break;
	default: count = 0;
	}
	cn[3] = count;

	switch (nextRow(4)) {
	case 5:
		count = 11;
		if (!isMatchingBoard(p, { 15, 23, 31, 39 }))
			count--;
		if (!isMatchingBoard(p, { 36, 37, 38, 39 }))
			count--;
		if (!isMatchingBoard(p, { 37, 38, 39, 40 }))
			count--;
		if (!isMatchingBoard(p, { 38, 39, 40, 41 }))
			count--;
		if (!isMatchingBoard(p, { 39, 32, 25, 18 }))
			count--;
		break;
	case 4:
		count = 17;
		if (!isMatchingBoard(p, { 8, 16, 24, 32 }))
			count--;
		if (!isMatchingBoard(p, { 16, 24, 32, 40 }))
			count--;
		if (!isMatchingBoard(p, { 38, 32, 26, 20 }))
			count--;
		if (!isMatchingBoard(p, { 29, 30, 31, 32 }))
			count--;
		if (!isMatchingBoard(p, { 30, 31, 32, 33 }))
			count--;
		if (!isMatchingBoard(p, { 31, 32, 33, 34 }))
			count--;
		if (!isMatchingBoard(p, { 39, 32, 25, 18 }))
			count--;
		if (!isMatchingBoard(p, { 32, 25, 18, 11 }))
			count--;
		break;
	case 3:
		count = 23;
		if (!isMatchingBoard(p, { 1, 9, 17, 25 }))
			count--;
		if (!isMatchingBoard(p, { 9, 17, 25, 33 }))
			count--;
		if (!isMatchingBoard(p, { 31, 25, 19, 13 }))
			count--;
		if (!isMatchingBoard(p, { 22, 23, 24, 25 }))
			count--;
		if (!isMatchingBoard(p, { 23, 24, 25, 26 }))
			count--;
		if (!isMatchingBoard(p, { 24, 25, 26, 27 }))
			count--;
		if (!isMatchingBoard(p, { 17, 25, 33, 41 }))
			count--;
		if (!isMatchingBoard(p, { 37, 31, 25, 19 }))
			count--;
		if (!isMatchingBoard(p, { 39, 32, 25, 18 }))
			count--;
		if (!isMatchingBoard(p, { 32, 25, 18, 11 }))
			count--;
		if (!isMatchingBoard(p, { 25, 18, 11, 4 }))
			count--;
		break;
	case 2:
		count = 22;
		if (!isMatchingBoard(p, { 2, 10, 18, 26 }))
			count--;
		if (!isMatchingBoard(p, { 24, 18, 12, 6 }))
			count--;
		if (!isMatchingBoard(p, { 15, 16, 17, 18 }))
			count--;
		if (!isMatchingBoard(p, { 16, 17, 18, 19 }))
			count--;
		if (!isMatchingBoard(p, { 17, 18, 19, 20 }))
			count--;
		if (!isMatchingBoard(p, { 10, 18, 26, 34 }))
			count--;
		if (!isMatchingBoard(p, { 30, 24, 18, 12 }))
			count--;
		if (!isMatchingBoard(p, { 36, 30, 24, 18 }))
			count--;
		if (!isMatchingBoard(p, { 39, 32, 25, 18 }))
			count--;
		if (!isMatchingBoard(p, { 32, 25, 18, 11 }))
			count--;
		if (!isMatchingBoard(p, { 25, 18, 11, 4 }))
			count--;
		break;
	case 1:
		count = 16;
		if (!isMatchingBoard(p, { 8, 9, 10, 11 }))
			count--;
		if (!isMatchingBoard(p, { 9, 10, 11, 12 }))
			count--;
		if (!isMatchingBoard(p, { 10, 11, 12, 13 }))
			count--;
		if (!isMatchingBoard(p, { 3, 11, 19, 27 }))
			count--;
		if (!isMatchingBoard(p, { 23, 17, 11, 5 }))
			count--;
		if (!isMatchingBoard(p, { 29, 23, 17, 11 }))
			count--;
		if (!isMatchingBoard(p, { 32, 25, 18, 11 }))
			count--;
		if (!isMatchingBoard(p, { 25, 18, 11, 4 }))
			count--;
		break;
	case 0:
		count = 10;
		if (!isMatchingBoard(p, { 1, 2, 3, 4 }))
			count--;
		if (!isMatchingBoard(p, { 2, 3, 4, 5 }))
			count--;
		if (!isMatchingBoard(p, { 3, 4, 5, 6 }))
			count--;
		if (!isMatchingBoard(p, { 22, 16, 10, 4 }))
			count--;
		if (!isMatchingBoard(p, { 25, 18, 11, 4 }))
			count--;
		break;
	default: count = 0;
	}
	cn[4] = count;

	switch (nextRow(5)) {
	case 5:
		count = 9;
		if (!isMatchingBoard(p, { 16, 24, 32, 40 }))
			count--;
		if (!isMatchingBoard(p, { 37, 38, 39, 40 }))
			count--;
		if (!isMatchingBoard(p, { 38, 39, 40, 41 }))
			count--;
		if (!isMatchingBoard(p, { 40, 33, 26, 19 }))
			count--;
		break;
	case 4:
		count = 13;
		if (!isMatchingBoard(p, { 9, 17, 25, 33 }))
			count--;
		if (!isMatchingBoard(p, { 17, 25, 33, 41 }))
			count--;
		if (!isMatchingBoard(p, { 30, 31, 32, 33 }))
			count--;
		if (!isMatchingBoard(p, { 31, 32, 33, 34 }))
			count--;
		if (!isMatchingBoard(p, { 40, 33, 26, 19 }))
			count--;
		if (!isMatchingBoard(p, { 33, 26, 19, 12 }))
			count--;
		break;
	case 3:
		count = 17;
		if (!isMatchingBoard(p, { 2, 10, 18, 26 }))
			count--;
		if (!isMatchingBoard(p, { 10, 18, 26, 34 }))
			count--;
		if (!isMatchingBoard(p, { 23, 24, 25, 26 }))
			count--;
		if (!isMatchingBoard(p, { 24, 25, 26, 27 }))
			count--;
		if (!isMatchingBoard(p, { 38, 32, 26, 20 }))
			count--;
		if (!isMatchingBoard(p, { 40, 33, 26, 19 }))
			count--;
		if (!isMatchingBoard(p, { 33, 26, 19, 12 }))
			count--;
		if (!isMatchingBoard(p, { 26, 19, 12, 5 }))
			count--;
		break;
	case 2:
		count = 16;
		if (!isMatchingBoard(p, { 3, 11, 19, 27 }))
			count--;
		if (!isMatchingBoard(p, { 16, 17, 18, 19 }))
			count--;
		if (!isMatchingBoard(p, { 17, 18, 19, 20 }))
			count--;
		if (!isMatchingBoard(p, { 31, 25, 19, 13 }))
			count--;
		if (!isMatchingBoard(p, { 37, 31, 25, 19 }))
			count--;
		if (!isMatchingBoard(p, { 40, 33, 26, 19 }))
			count--;
		if (!isMatchingBoard(p, { 33, 26, 19, 12 }))
			count--;
		if (!isMatchingBoard(p, { 26, 19, 12, 5 }))
			count--;
		break;
	case 1:
		count = 12;
		if (!isMatchingBoard(p, { 9, 10, 11, 12 }))
			count--;
		if (!isMatchingBoard(p, { 10, 11, 12, 13 }))
			count--;
		if (!isMatchingBoard(p, { 24, 18, 12, 6 }))
			count--;
		if (!isMatchingBoard(p, { 30, 24, 18, 12 }))
			count--;
		if (!isMatchingBoard(p, { 33, 26, 19, 12 }))
			count--;
		if (!isMatchingBoard(p, { 26, 19, 12, 5 }))
			count--;
		break;
	case 0:
		count = 8;
		if (!isMatchingBoard(p, { 2, 3, 4, 5 }))
			count--;
		if (!isMatchingBoard(p, { 3, 4, 5, 6 }))
			count--;
		if (!isMatchingBoard(p, { 23, 17, 11, 5 }))
			count--;
		if (!isMatchingBoard(p, { 26, 19, 12, 5 }))
			count--;
		break;
	default: count = 0;
	}
	cn[5] = count;

	switch (nextRow(6)) {
	case 5:
		count = 7;
		if (!isMatchingBoard(p, { 17, 25, 33, 41 }))
			count--;
		if (!isMatchingBoard(p, { 38, 39, 40, 41 }))
			count--;
		if (!isMatchingBoard(p, { 41, 34, 27, 20 }))
			count--;
		break;
	case 4:
		count = 9;
		if (!isMatchingBoard(p, { 10, 18, 26, 34 }))
			count--;
		if (!isMatchingBoard(p, { 31, 32, 33, 34 }))
			count--;
		if (!isMatchingBoard(p, { 41, 34, 27, 20 }))
			count--;
		if (!isMatchingBoard(p, { 34, 27, 20, 13 }))
			count--;
		break;
	case 3:
		count = 11;
		if (!isMatchingBoard(p, { 3, 11, 19, 27 }))
			count--;
		if (!isMatchingBoard(p, { 24, 25, 26, 27 }))
			count--;
		if (!isMatchingBoard(p, { 41, 34, 27, 20 }))
			count--;
		if (!isMatchingBoard(p, { 34, 27, 20, 13 }))
			count--;
		if (!isMatchingBoard(p, { 27, 20, 13, 6 }))
			count--;
		break;
	case 2:
		count = 10;
		if (!isMatchingBoard(p, { 17, 18, 19, 20 }))
			count--;
		if (!isMatchingBoard(p, { 38, 32, 26, 20 }))
			count--;
		if (!isMatchingBoard(p, { 41, 34, 27, 20 }))
			count--;
		if (!isMatchingBoard(p, { 34, 27, 20, 13 }))
			count--;
		if (!isMatchingBoard(p, { 27, 20, 13, 6 }))
			count--;
		break;
	case 1:
		count = 8;
		if (!isMatchingBoard(p, { 10, 11, 12, 13 }))
			count--;
		if (!isMatchingBoard(p, { 31, 25, 19, 13 }))
			count--;
		if (!isMatchingBoard(p, { 34, 27, 20, 13 }))
			count--;
		if (!isMatchingBoard(p, { 27, 20, 13, 6 }))
			count--;
		break;
	case 0:
		count = 6;
		if (!isMatchingBoard(p, { 3, 4, 5, 6 }))
			count--;
		if (!isMatchingBoard(p, { 24, 18, 12, 6 }))
			count--;
		if (!isMatchingBoard(p, { 27, 20, 13, 6 }))
			count--;
		break;
	default: count = 0;
	}
	cn[6] = count;

	// Sort
	std::vector<int> moves(8);
	do {
		count = 0;
		for (i = 6; i >= 0; i--) {
			if (cn[i] > count) {
				count = cn[i];
				index = i;
			}
		}
		cn[index] = 0;
		moves[j++] = index;
	} while (count != 0);
	
	// Remove all zero counts
	while (moves.size() > 1 && moves.back() == 0) {
		moves.pop_back();
	}

	return moves;
}

bool Connect4::isMatchingBoard(int player, std::vector<int> winningPositions) {
	for (int position : winningPositions) {
		int row = position / cols;
		int col = position % cols;
		if (board[row][col] != player) {
			return false;
		}
	}
	return true;
}

std::pair<int, int> Connect4::getLastMove() { return lastMove; }
void Connect4::setLastMove(std::pair<int, int> p) { lastMove = p; }