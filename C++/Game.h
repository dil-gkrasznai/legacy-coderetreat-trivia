#include <iostream>
#include <list>
#include <vector>
using namespace std;

#ifndef GAME_H_
#define GAME_H_

class TestGame;

class Game{
private:
	friend class TestGame;

	vector<string> players;
	vector<int> places;
	vector<int> purses;
	vector<bool> inPenaltyBox;

	list<string> popQuestions;
	list<string> scienceQuestions;
	list<string> sportsQuestions;
	list<string> rockQuestions;

	unsigned int currentPlayer;
	bool isGettingOutOfPenaltyBox;

	ostream& output;

public:
	Game(ostream& output);

	static string createQuestion(const string& category, int index);
	bool isPlayable() const;
	bool add(const string& playerName);

	int howManyPlayers() const;
	void roll(int roll);

private:
	void askQuestion();
	string currentCategory() const;
	void chooseNextPlayer();

public:
	bool wasCorrectlyAnswered();
	bool wrongAnswer();

private:
	bool didPlayerWin() const;
};

#endif /* GAME_H_ */
