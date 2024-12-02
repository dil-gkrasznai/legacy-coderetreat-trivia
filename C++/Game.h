#include <iostream>
#include <list>
#include <vector>
#include <map>
using namespace std;

#ifndef GAME_H_
#define GAME_H_

class Localization {
public:
	enum Language {
		ENGLISH,
		HUNGARIAN,
		GERMAN
	};

	static string getMessage(Language language, const char* messageId);
};


class Categories {
public:
	Categories(Localization::Language language);

	const std::vector<std::string>& getCategories() const;
	size_t getCategoryNum() const;
private:
	std::vector<std::string> categories;
};


class Questions {
public:
	Questions(const Categories& categories, Localization::Language language);
	string getNextQuestion(const string& category);

private:
	static string createQuestion(const string& category, Localization::Language language, int index);
	map<string, list<string>> questions;
};

class TestGame;

class Game{
public:
	
private:
	friend class TestGame;

	vector<string> players;
	vector<int> places;
	vector<int> purses;
	vector<bool> inPenaltyBox;

	unsigned int currentPlayer;
	bool isGettingOutOfPenaltyBox;

	ostream& output;
	Localization::Language language;

	Categories categories;
	Questions questions;

public:
	Game(ostream& output, Localization::Language language);

	
	bool isPlayable() const;
	bool add(const string& playerName);

	int howManyPlayers() const;
	void roll(int roll);

private:
	void askQuestion();
	string currentCategory() const;
	void moveCurrentPlayer(int roll);
	void chooseNextPlayer();
	string getMessage(const char* messageId) const;
	int getFieldNum() const;

public:
	bool wasCorrectlyAnswered();
	bool wrongAnswer();

private:
	bool didPlayerWin() const;
};

#endif /* GAME_H_ */
