#include "pch.h"
#include <gtest/gtest.h>
#include <optional>

#include "../Game.h"


class TestGame : public Game
{
public:
	TestGame(ostream& output, Localization::Language language = Localization::ENGLISH) :
		Game(output, language)
	{
	}

	int getPlayerPlace(int player) const
	{
		return places[player];
	}

	int getPlayerPurse(int player) const
	{
		return purses[player];
	}

	bool isPlayerInPenaltyBox(int player) const
	{
		return inPenaltyBox[player];
	}

	const string& getPlayerName(int player) const
	{
		return players[player];
	}

	unsigned int getCurrentPlayer() const
	{
		return currentPlayer;
	}

	bool isPlayerGettingOutOfPenaltyBox() const
	{
		return isGettingOutOfPenaltyBox;
	}
};

TEST(EmtyGameTest, Playabilitytest) {
	TestGame aGame (std::cout);
	EXPECT_FALSE(aGame.isPlayable ());
	EXPECT_EQ(0, aGame.howManyPlayers());	
}

TEST(OnePlayerGameTest, Playabilitytest) {
	TestGame aGame(std::cout);
	aGame.add("Chet");
	EXPECT_FALSE(aGame.isPlayable());
	EXPECT_EQ(1, aGame.howManyPlayers());
}

TEST(TwoPlayerGameTest, Playabilitytest) {
	TestGame aGame(std::cout);
	aGame.add("Chet");
	aGame.add("Pat");
	EXPECT_TRUE(aGame.isPlayable());
	EXPECT_EQ(2, aGame.howManyPlayers());
}

TEST(ManyPlayerTest, LotsOfPlayerAddTesf)
{
	TestGame aGame(std::cout);

	for (int i = 0; i < 100; i++)
	{
		aGame.add("Player" + std::to_string(i));
	}

	EXPECT_EQ(100, aGame.howManyPlayers());
}


class GameTest : public ::testing::Test
{
public:
	std::unique_ptr<TestGame> aGame;
	std::unique_ptr<std::ostringstream> output;

	GameTest()
	{
	}

	virtual void SetUp()
	{
		output = std::make_unique<std::ostringstream>();
		aGame = std::make_unique<TestGame>(*output);
		aGame->add("Chet");
		aGame->add("Pat");
		aGame->add("Sue");
	}

	virtual void TearDown()
	{
		aGame.reset();
		output.reset();
	}
};


TEST_F(GameTest, RollTest)
{
	aGame->roll(1);
	EXPECT_EQ(0, aGame->getCurrentPlayer());
	EXPECT_EQ(1, aGame->getPlayerPlace(0));
	EXPECT_FALSE(aGame->isPlayerInPenaltyBox(0));
}


TEST_F(GameTest, WrongAnswerTest)
{
	aGame->roll(1);
	aGame->wrongAnswer();
	EXPECT_TRUE(aGame->isPlayerInPenaltyBox(0));
}


TEST_F(GameTest, CorrectAnswerTest)
{
	aGame->roll(1);
	aGame->wasCorrectlyAnswered();
	EXPECT_EQ(1, aGame->getPlayerPurse(0));
	EXPECT_FALSE(aGame->isPlayerInPenaltyBox(0));
}


TEST_F(GameTest, CorrectAnswerMessageCheckTest)
{
	aGame->roll(1);
	output->str("");
	output->clear();

	aGame->wasCorrectlyAnswered();
	std::string result = output->str();
	EXPECT_EQ(1, aGame->getPlayerPurse(0));
	EXPECT_FALSE(aGame->isPlayerInPenaltyBox(0));
	EXPECT_EQ("Answer was correct!\nChet now has 1 Gold Coins.\n", result);
}





