#include <ctime>
#include <iostream>
#include "BlackJack.h"
using namespace nagisakuya::BlackJack;
using namespace std;

int main()
{
    srand((unsigned int)time(NULL));
	Table test(8, Rule());
	string temp_s;
	test.addplayer(Player(1));
	for (size_t i = 0; ; i++)
	{
		test.play();
		cout << "Press c to continue. press e to exit." << endl;
		cin >> temp_s;
		if (temp_s == "e") break;
	}

}