#include "pch.h"

#include "../Game.h"

TEST(TestCaseName, TestName)
{
	Game game;
	EXPECT_FALSE(game.isPlayable ());
}