#include "Game.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

using namespace std;

constexpr const char* CorrectAnswerMessage = "Answer was correct!";

Game::Game(ostream& output) :
	places{}, 
	purses{}, 
	currentPlayer(0),
	isGettingOutOfPenaltyBox(false),
	output(output)
{
	for (int i = 0; i < 50; i++)
	{
		popQuestions.push_back(createQuestion("Pop", i));
		scienceQuestions.push_back(createQuestion("Science", i));
		sportsQuestions.push_back(createQuestion("Sports", i));
		rockQuestions.push_back(createQuestion("Rock", i));
	}
}

string Game::createQuestion(const string& category, int index)
{
	return category + std::string (" Question ") + std::to_string(index);
}

bool Game::isPlayable() const
{
	return (howManyPlayers() >= 2);
}

bool Game::add(const string& playerName){
	players.push_back(playerName);
	places.push_back (0);
	purses.push_back (0);
	inPenaltyBox.push_back (false);

	output << playerName << " was added" << endl;
	output << "They are player number " << players.size() << endl;
	return true;
}

int Game::howManyPlayers() const
{
	return static_cast<int>(players.size());
}

void Game::roll(int roll)
{
	output << players[currentPlayer] << " is the current player" << endl;
	output << "They have rolled a " << roll << endl;

	if (inPenaltyBox[currentPlayer])
	{
		if (roll % 2 != 0)
		{
			isGettingOutOfPenaltyBox = true;

			output << players[currentPlayer] << " is getting out of the penalty box" << endl;
			places[currentPlayer] = places[currentPlayer] + roll;
			if (places[currentPlayer] > 11) places[currentPlayer] = places[currentPlayer] - 12;

			output << players[currentPlayer] << "'s new location is " << places[currentPlayer] << endl;
			output << "The category is " << currentCategory() << endl;
			askQuestion();
		}
		else
		{
			output << players[currentPlayer] << " is not getting out of the penalty box" << endl;
			isGettingOutOfPenaltyBox = false;
		}

	}
	else
	{

		places[currentPlayer] = places[currentPlayer] + roll;
		if (places[currentPlayer] > 11) places[currentPlayer] = places[currentPlayer] - 12;

		output << players[currentPlayer] << "'s new location is " << places[currentPlayer] << endl;
		output << "The category is " << currentCategory() << endl;
		askQuestion();
	}

}

void Game::askQuestion()
{
	if (currentCategory() == "Pop")
	{
		output << popQuestions.front() << endl;
		popQuestions.pop_front();
	}
	if (currentCategory() == "Science")
	{
		output << scienceQuestions.front() << endl;
		scienceQuestions.pop_front();
	}
	if (currentCategory() == "Sports")
	{
		output << sportsQuestions.front() << endl;
		sportsQuestions.pop_front();
	}
	if (currentCategory() == "Rock")
	{
		output << rockQuestions.front() << endl;
		rockQuestions.pop_front();
	}
}


string Game::currentCategory() const
{
	if (places[currentPlayer] % 4 == 0) return "Pop";
	if (places[currentPlayer] % 4 == 1) return "Science";
	if (places[currentPlayer] % 4 == 2) return "Sports";
	return "Rock";
}

void Game::chooseNextPlayer()
{
	currentPlayer++;
	if (currentPlayer == players.size()) currentPlayer = 0;
}

bool Game::wasCorrectlyAnswered()
{
	if (inPenaltyBox[currentPlayer])
	{
		if (isGettingOutOfPenaltyBox)
		{
			output << CorrectAnswerMessage << endl;
			purses[currentPlayer]++;
			output << players[currentPlayer]
			     << " now has "
			     << purses[currentPlayer]
				<<  " Gold Coins." << endl;

			bool winner = didPlayerWin();
			chooseNextPlayer();

			return winner;
		}
		else
		{
			chooseNextPlayer();
			return true;
		}
	}
	else
	{

		output << CorrectAnswerMessage << endl;
		purses[currentPlayer]++;
		output << players[currentPlayer]
				<< " now has "
				<< purses[currentPlayer]
			<< " Gold Coins." << endl;

		bool winner = didPlayerWin();
		chooseNextPlayer();

		return winner;
	}
}

bool Game::wrongAnswer()
{
	output << "Question was incorrectly answered" << endl;
	output << players[currentPlayer] + " was sent to the penalty box" << endl;
	inPenaltyBox[currentPlayer] = true;

	chooseNextPlayer();
	return true;
}


bool Game::didPlayerWin() const
{
	return !(purses[currentPlayer] == 6);
}
