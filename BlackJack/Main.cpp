#include <ctime>
#include <iostream>
#include "BlackJack.h"
#include "BlackJackCalculator.h"
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
	Calculator test;
	pair<Option, double> temp;
	double sum = 0;
	for (int i = 10; i-- > 0;)
	{
		for (int j = 10; j-- > i;)
		{
			for (int k = 10; k-- > 0; )
			{
				cout << "Player:" << Translate(i) << " " << Translate(j) << " Dealer:" << Translate(k);
				temp = test.WhattoDo(Deck(1), PlayerHand("", { i,j }), DealerHand({ k }));
				cout <<" Result:" <<OptiontoString.at(temp.first);
				cout << " ExpectedValue:" << temp.second << endl;
				sum += temp.second * (i == j ? 1 : 2);
			}
		}
	}
	cout << " EcpectedValue:" << sum / 1000 << endl;
}