
#include <iostream>
#include "BlackJack.h"
#include "BlackJackCalculator.h"
#include<fstream>

using namespace nagisakuya::BlackJack;
using namespace std;

/*int main()
{
	srand((unsigned int)time(NULL));
	Table test(8, Rule());
	string temp_s;
	test.addplayer(Player(1));
	test.replay();
}*/

int main()
{
	ofstream outputfile;
	outputfile.open("output.txt", ios::trunc);
	Calculator test(Deck(1), Rule(true, false, true), Rate(1.5));
	test.Calculate_Expection(outputfile);
}