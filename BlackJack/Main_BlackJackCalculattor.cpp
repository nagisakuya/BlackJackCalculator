
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
	outputfile.open("output.txt",ios::trunc);
	Calculator test;
	outputfile << "Player:(Player 1stcard) (Player 2ndcard) Dealer:(Dealer upcard) Stand:(Ev(Expected value) if stand) Hit:(Ev if hit) Double:(Ev if double) Split:(Ev if split) WhattoDo:(BestOption) ExpectedValue:(Expected value)" << endl;
	test.Calculate_Expection(outputfile);
	Deck deck(1);
}