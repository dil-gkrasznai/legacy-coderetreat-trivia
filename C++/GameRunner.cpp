#include <stdlib.h>
#include "Game.h"

static bool notAWinner;

int main()
{

	srand(static_cast<unsigned int>(time(nullptr)));
	Game aGame(std::cout);

	aGame.add("Chet");
	aGame.add("Pat");
	aGame.add("Sue");

	do
	{

		aGame.roll(rand() % 5 + 1);

		if (rand() % 9 == 7)
		{
			notAWinner = aGame.wrongAnswer();
		}
		else
		{
			notAWinner = aGame.wasCorrectlyAnswered();
		}
	} while (notAWinner);

}
