#include "Game.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

constexpr const char* CorrectAnswerMessage = "Answer was correct!";

std::string Localization::getMessage(Localization::Language language, const char* messageId)
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
	case Localization::HUNGARIAN:
		return hungarian_messages.at(messageId);
	case Localization::GERMAN:
		return german_messages.at(messageId);
	case Localization::ENGLISH:
	default:
		return english_messages.at(messageId);
	}
}


Categories::Categories(Localization::Language language)
{
	if (language == Localization::HUNGARIAN)
	{
		categories = { "Pop", "Science", "Sports", "Rock", "Money", "Politics" };
	}
	else
	{
		categories = { "Pop", "Science", "Sports", "Rock" };
	}
}

const std::vector<std::string>& Categories::getCategories() const
{
	return categories;
}


size_t Categories::getCategoryNum() const
{
	return categories.size();
}


Questions::Questions(const Categories& categories, Localization::Language language)
{
	for (const auto& category : categories.getCategories())
	{
		for (int i = 0; i < 50; i++)
		{
			questions[category].push_back(createQuestion(category, language, i));
		}
	}
}

string Questions::getNextQuestion(const string& category)
{
	string question = questions[category].front();
	questions[category].pop_front();
	return question;
}

string Questions::createQuestion(const string& category, Localization::Language /*language*/, int index)
{
	return category + std::string(" Question ") + std::to_string(index);
}



Game::Game(ostream& output, Localization::Language language) :
	places{}, 
	purses{}, 
	currentPlayer(0),
	isGettingOutOfPenaltyBox(false),
	output(output),
	language(language),
	categories(language),
	questions(categories, language)
{
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
	output << questions.getNextQuestion (currentCategory()) << endl;
}


string Game::currentCategory() const
{
	return categories.getCategories()[places[currentPlayer] % categories.getCategoryNum()];
}


void Game::moveCurrentPlayer(int roll)
{
	places[currentPlayer] = places[currentPlayer] + roll;
	if (places[currentPlayer] >= getFieldNum ()) 
		places[currentPlayer] = places[currentPlayer] - getFieldNum();
}


void Game::chooseNextPlayer()
{
	currentPlayer++;
	if (currentPlayer == players.size())
		currentPlayer = 0;
}


string Game::getMessage(const char* messageId) const
{
	return Localization::getMessage(language, messageId);
}


int Game::getFieldNum() const
{
	if (language == Localization::GERMAN)
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
