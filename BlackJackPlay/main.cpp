
#include <iostream>
#include <ctime>
#include "../BlackJackLib/BlackJack.h"

using namespace nagisakuya::BlackJack;
using namespace std;

int main()
{
	srand((unsigned int)time(NULL));
	Table test(8, Rule());
	string temp_s;
	test.addplayer(Player(1));
	test.replay();
}