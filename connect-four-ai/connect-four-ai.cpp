// 
// connect-four-ai.cpp
// Jake Buhite and Nick Abegg
// 10/26/2023
//
#include "connect-four.h"
#include "minimax.h"
#include "tdl-agent.h"

void beginPvP(Connect4* game);
void beginPvA(Connect4* game, MiniMax* agent);
void beginAvA(Connect4* game, MiniMax* agent);
int beginTvT(Connect4* game, TDLAgent* agent1, TDLAgent* agent2);
int beginMvT(Connect4* game, TDLAgent* agent1, Agent* ref);
void trainTDL();

int main()
{
	int rows = -1;
	int cols = -1;
	int depth = 12;
	
	std::cout << "Please enter the number of rows: ";
	std::cin >> rows;

	std::cout << "Please enter the number of cols: ";
	std::cin >> cols;


	Connect4* game = new Connect4(rows, cols);
	MiniMax* agent = new MiniMax(game, depth);

	//beginPvP(game);
	//beginPvA(game, agent);
	beginAvA(game, agent);
	//trainTDL();

	//delete game;
	//delete agent;
}

void beginPvP(Connect4* game) {
	std::string actors[] = { "NONE", "PLAYER 1", "PLAYER 2" };
	int choice = 0;

	// Main game loop
	game->printBoard();
	while (!game->isGoalState()) {
		game->setCurrentTurn((game->getCurrentTurn() == PLAYER1) ? PLAYER2 : PLAYER1);
		std::cout << "It is now " << actors[game->getCurrentTurn()] << " turn. Please select a column: ";
		std::cin >> choice;
		while (game->isDominateMove(choice)) {
			std::cout << "ERROR: Please pick a non-dominate move for the first round: ";
			std::cin >> choice;
		}
		int row = game->nextRow(choice);
		game->addDisc(row, choice);
		game->printBoard();
		if (game->getCurrentTurn()  == PLAYER2)
			game->incrementRound();
	}
	game->setWinner(game->getCurrentTurn());
	std::cout << "And the winner is... " << actors[game->getWinner()] << "!" << std::endl;
}

void beginPvA(Connect4* game, MiniMax* agent) {
	std::string actors[] = { "NONE", "PLAYER 1", "AI" };
	int choice = 0;

	// Main game loop
	game->printBoard();
	while (!game->isGoalState()) {
		game->setCurrentTurn((game->getCurrentTurn() == PLAYER1) ? PLAYER2 : PLAYER1);
		std::cout << "It is now " << actors[game->getCurrentTurn()] << " turn. Please select a column: ";
		if (game->getCurrentTurn() == PLAYER1) {
			std::cin >> choice;
			while (game->isDominateMove(choice)) {
				std::cout << "ERROR: Please pick a non-dominate move for the first round: ";
				std::cin >> choice;
			}
		}
		else {
			std::cout << std::endl;
			choice = agent->getAgentMove();
		}
		int row = game->nextRow(choice);
		game->addDisc(row, choice);
		game->printBoard();
		if (game->getCurrentTurn() == PLAYER2)
			game->incrementRound();
	}
	game->setWinner(game->getCurrentTurn());
	std::cout << "And the winner is... " << actors[game->getWinner()] << "!" << std::endl;
}

void beginAvA(Connect4* game, MiniMax* agent) {
	std::string actors[] = { "NONE", "PLAYER 1", "PLAYER 2" };
	int choice = 0;

	// Main game loop
	game->printBoard();
	while (!game->isGoalState()) {
		game->setCurrentTurn((game->getCurrentTurn() == PLAYER1) ? PLAYER2 : PLAYER1);
		std::cout << "It is now " << actors[game->getCurrentTurn()] << " turn." << std::endl;
		choice = agent->getAgentMove();
		int row = game->nextRow(choice);
		game->addDisc(row, choice);
		game->printBoard();
		if (game->getCurrentTurn() == PLAYER2)
			game->incrementRound();
	}
	game->setWinner(game->getCurrentTurn());
	std::cout << "And the winner is... " << actors[game->getWinner()] << "!" << std::endl;
}

int beginTvT(Connect4* game, TDLAgent* agent1, TDLAgent* agent2) {
	Actor curPlayer = PLAYER1;
	int winner = 0;
	bool gameOver = false;
	
	while (true) {
		if (!gameOver) {
			int x = (curPlayer == PLAYER1) ? agent1->getBestMove(game->getBoard()) : agent2->getBestMove(game->getBoard());
			int colHeight = game->nextRow(x);
			if (!gameOver && colHeight != -1) {
				if (game->canWin(curPlayer, x, colHeight)) {
					winner = curPlayer;
					gameOver = true;
				}
				game->addDisc(colHeight, x, curPlayer);
				game->setLastMove({colHeight, x});
				curPlayer = (curPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
				if (game->isDraw() && !gameOver) {
					int col = game->getLastMove().second;
					if (!game->canWin(2, col, game->nextRow(col) - 1)) {
						gameOver = true;
					}
				}
			}
		}
		else {
			if (!agent1->isTraining()) game->printBoard();
			return winner;
		}
	}
}

int beginMvT(Connect4* game, TDLAgent* agent1, Agent* ref) {
	Actor curPlayer = PLAYER1;
	int winner = 0;
	bool gameOver = false;

	while (true) {
		if (!gameOver) {
			int x = (curPlayer == PLAYER1) ? agent1->getBestMove(game->getBoard()) : ref->getAgentMove();
			int colHeight = game->nextRow(x);
			if (!gameOver && colHeight != -1) {
				if (game->canWin(curPlayer, x, colHeight)) {
					winner = curPlayer;
					gameOver = true;
				}
				game->addDisc(colHeight, x, curPlayer);
				game->setLastMove({ colHeight, x });
				curPlayer = (curPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
				if (game->isDraw() && !gameOver) {
					int col = game->getLastMove().second;
					if (!game->canWin(2, col, game->nextRow(col) - 1)) {
						gameOver = true;
					}
				}
				if (game->getCurrentTurn() == PLAYER2)
					game->incrementRound();
			}
		}
		else {
			return winner;
		}
	}
}

void trainTDL() {
	// Initial alpha and epsilon
	double initialAlpha = 0.004;
	double initialEpsilon = 0.1;

	std::string FILENAME = "results.csv";

	std::string trainingState = "TRAIN"; // "EVAL"
	int trainingGames = 0;
	double prevScore1 = 0;
	double prevScore2 = 0;

	int winner = -1;
	Connect4* game = new Connect4();

	// Initialize first and second agent
	TDLAgent* agent1 = new TDLAgent(false, 1, initialAlpha, initialEpsilon);
	TDLAgent* agent2 = new TDLAgent(false, 2, initialAlpha, initialEpsilon);

	// Assign agents as their others
	agent1->setOther(agent2);
	agent2->setOther(agent1);

	// Initialize game
	std::cout <<  "Started training!" << std::endl;
	agent1->toggleTraining();

	// Load agent LUT
	agent1->loadAgent("weights1.txt");
	agent2->loadAgent("weights2.txt");
	while (true) {
		trainingState = "TRAIN";
		trainingGames += 1;
		int evalInterval = 20000;
		while (trainingGames % evalInterval != 0) {
			if (trainingGames % 1000 == 0)
				std::cout << "Game #" << trainingGames << std::endl;

			// Reset and begin new game
			delete game;
			game = new Connect4();

			// Get new winner
			winner = beginTvT(game, agent1, agent2);

			agent1->computeAlpha();
			agent2->computeAlpha();
			trainingGames += 1;
		}

		if (trainingGames % 1000000 == 0) {
			agent1->saveAgent("weights1.txt");
		}

		std::cout << "Evaluating after game " << trainingGames << std::endl;
		game->printBoard();
		std::cout << "Current alpha " << agent1->getAlpha() << std::endl;
		std::cout << "Current epsilon " << agent1->getEpsilon() << std::endl;

		trainingState = "EVAL";

		// EVALUATE TDL AGENT
		double score = 0;
		agent1->toggleTraining();
		int evalGames = 100;
		for (int i = 0; i < evalGames; ++i) {
			winner = -1;
			// Reset
			delete game;
			game = new Connect4();

			// Get winner
			int winner = beginTvT(game, agent1, agent2);
			switch (winner) {
			case 1:
				score += 1;
				break;
			case 2:
				// Agent lost
				break;
			default:
				score += 0.5;
				break;
			}
		}

		// Resume training
		agent1->toggleTraining();

		std::cout << "Agent achieved a score of " << score << std::endl;

		// write the output of each evaluation to a file
		std::ofstream out;
		out.open(FILENAME, std::ios_base::app);
		out << trainingGames << "," << score << '\n';
		out.close();

		int targetScore = 80;
		if (score >= targetScore && score <= prevScore1 && score <= prevScore2) {
			std::cout << "Finished training!" << std::endl;
			agent1->saveAgent("weights1.txt");
			agent2->saveAgent("weights2.txt");

			// Delete game and agents
			delete game;
			delete agent1;
			delete agent2;
			break;
		}
		prevScore1 = prevScore2;
		prevScore2 = score;
	}
}