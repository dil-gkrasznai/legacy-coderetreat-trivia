#include "pch.h"

#include "../Game.h"
#include "../ApprovalTests.hpp"

TEST(EmtyGameTest, Playabilitytest) {
	Game aGame;
	EXPECT_FALSE(aGame.isPlayable ());
	EXPECT_EQ(0, aGame.howManyPlayers());	
	ApprovalTests::Approvals::verify(aGame.howManyPlayers());
}

TEST(OnePlayerGameTest, Playabilitytest) {
	Game aGame;
	ApprovalTests::Approvals::verify(aGame.add("Chet"));
	EXPECT_FALSE(aGame.isPlayable());
	EXPECT_EQ(1, aGame.howManyPlayers());
}

TEST(TwoPlayerGameTest, Playabilitytest) {
	Game aGame;
	aGame.add("Chet");
	aGame.add("Pat");
	EXPECT_TRUE(aGame.isPlayable());
	EXPECT_EQ(2, aGame.howManyPlayers());
}

TEST(GameTest, AddRockQuestionTest)
{
	Game aGame;
	EXPECT_EQ("Rock Question 0", aGame.createRockQuestion(0));
	EXPECT_EQ("Rock Question 1", aGame.createRockQuestion(1));
	EXPECT_EQ("Rock Question 2", aGame.createRockQuestion(2));
}