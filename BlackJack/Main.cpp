#include <ctime>
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
	pair<Option, double> temp;
	double sum = 0;
	outputfile << "Player:(Player 1stcard) (Player 2ndcard) Dealer:(Dealer upcard) Stand:(Ev(Expected value) if stand) Hit:(Ev if hit) Double:(Ev if double) Split:(Ev if split) WhattoDo:(BestOption) ExpectedValue:(Expected value)" << endl;
	clock_t start = clock();
	Deck deck(1);
	double size = deck.size();
	for (int i = 10; i-- > 0;)
	{
		for (int j = 10; j-- > i;)
		{
			for (int k = 10; k-- > 0; )
			{
				outputfile << "Player:" << Translate(i) << " " << Translate(j) << " \tDealer:" << Translate(k) << "\t";
				cout << "Player:" << Translate(i) << " " << Translate(j) << "\tDealer:" << Translate(k) << "\t";
				temp = test.WhattoDo_writedown(((deck - i) - j) - k, PlayerHand({ i,j }), DealerHand({ k }),outputfile);
				cout << "WhattoDo:" << OptiontoString.at(temp.first);
				cout << "\tExpectedValue:" << temp.second << endl;
				outputfile << "WhattoDo:" << OptiontoString.at(temp.first) << "\t" << "ExpectedValue:" << temp.second << endl;
				sum += temp.second * (i == j ? 1 : 2) * (double)(deck.count(i) * deck.count(j) * deck.count(k));
			}
		}
	}
	clock_t end = clock();
	cout << "TotalExpectedValue:" << sum / (size * (size - 1) * (size - 2)) << endl;
	cout << " Time:" << (end - start) / CLOCKS_PER_SEC << endl;
	outputfile << "TotalExpectedValue:" << sum / (size * (size - 1) * (size - 2)) << "\t" << " Time:" << (end - start) / CLOCKS_PER_SEC << "sec" << endl;
}