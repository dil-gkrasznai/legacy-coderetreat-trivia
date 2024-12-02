#include "Game.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

constexpr const char* CorrectAnswerMessage = "Answer was correct!";

static std::string GetMessage(Game::Langugage language, const char* messageId)
{
	using language_map = std::map<std::string, std::string>;
	static const language_map english_messages = { 
			{ CorrectAnswerMessage, "Answer was correct!" } 
	};
	static const language_map hungarian_messages = {
			{ CorrectAnswerMessage, "A valasz helyes!" }
	};
	static const language_map german_messages = {
			{ CorrectAnswerMessage, "Ja, wohl!" }
	};

	switch (language)
	{
	case Game::HUNGARIAN:
		return hungarian_messages.at(messageId);
	case Game::GERMAN:
		return german_messages.at(messageId);
	case Game::ENGLISH:
	default:
		return english_messages.at(messageId);
	}
}


Game::Game(ostream& output, Langugage language) :
	places{}, 
	purses{}, 
	currentPlayer(0),
	isGettingOutOfPenaltyBox(false),
	output(output),
	language(language)
{
	for (int i = 0; i < 50; i++)
	{
		questions["Pop"].push_back(createQuestion("Pop", i));
		questions["Science"].push_back(createQuestion("Science", i));
		questions["Sports"].push_back(createQuestion("Sports", i));
		questions["Rock"].push_back(createQuestion("Rock", i));
		questions["Money"].push_back(createQuestion("Money", i));
		questions["Politics"].push_back(createQuestion("Politics", i));
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
			moveCurrentPlayer(roll);

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
		moveCurrentPlayer(roll);

		output << players[currentPlayer] << "'s new location is " << places[currentPlayer] << endl;
		output << "The category is " << currentCategory() << endl;
		askQuestion();
	}

}

void Game::askQuestion()
{
	output << questions[currentCategory()].front() << endl;
	questions[currentCategory()].pop_front();
}


string Game::currentCategory() const
{
	if (places[currentPlayer] % getCategoryNum() == 0) return "Pop";
	if (places[currentPlayer] % getCategoryNum() == 1) return "Science";
	if (places[currentPlayer] % getCategoryNum() == 2) return "Sports";
	if (places[currentPlayer] % getCategoryNum() == 3) return "Rock";
	if (places[currentPlayer] % getCategoryNum() == 4) return "Money";
	if (places[currentPlayer] % getCategoryNum() == 5) return "Politics";
	throw std::runtime_error("Invalid category");	
}


void Game::moveCurrentPlayer(int roll)
{
	places[currentPlayer] = places[currentPlayer] + roll;
	if (places[currentPlayer] >= getFieldNum ()) places[currentPlayer] = places[currentPlayer] - getFieldNum();
}


void Game::chooseNextPlayer()
{
	currentPlayer++;
	if (currentPlayer == players.size())
		currentPlayer = 0;
}


string Game::getMessage(const char* messageId) const
{
	return GetMessage(language, messageId);
}


int Game::getCategoryNum() const
{
	if (language == HUNGARIAN)
		return 6;
	return 4;
}

int Game::getFieldNum() const
{
	if (language == GERMAN)
		return 16;
	return 12;
}


bool Game::wasCorrectlyAnswered()
{
	if (inPenaltyBox[currentPlayer])
	{
		if (isGettingOutOfPenaltyBox)
		{
			output << getMessage (CorrectAnswerMessage) << endl;
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
		output << getMessage(CorrectAnswerMessage) << endl;
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
