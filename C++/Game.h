#include <iostream>
#include <list>
#include <vector>
#include <map>
using namespace std;

#ifndef GAME_H_
#define GAME_H_

class TestGame;

class Game{
public:
	enum Langugage {
		ENGLISH,
		HUNGARIAN,
		GERMAN
	};
private:
	friend class TestGame;

	vector<string> players;
	vector<int> places;
	vector<int> purses;
	vector<bool> inPenaltyBox;

	map<string, list<string>> questions;

	unsigned int currentPlayer;
	bool isGettingOutOfPenaltyBox;

	ostream& output;
	Langugage language;

public:
	Game(ostream& output, Langugage language);

	static string createQuestion(const string& category, int index);
	bool isPlayable() const;
	bool add(const string& playerName);

	int howManyPlayers() const;
	void roll(int roll);

private:
	void askQuestion();
	string currentCategory() const;
	void chooseNextPlayer();
	string getMessage(const char* messageId) const;
	int getCategoryNum() const;

public:
	bool wasCorrectlyAnswered();
	bool wrongAnswer();

private:
	bool didPlayerWin() const;
};

#endif /* GAME_H_ */
