#include "BlackJackCalculator.h"
using namespace std;

namespace nagisakuya {
	namespace BlackJack {
		Calculator::Calculator(int NumberofDeck) :Table(NumberofDeck, Rule(),2.5) {}
		double Calculator::Calc_Expection()
		{
			pair<Option, double> temp;
			double sum = 0;
			for (int i = 10; i-- > 0;)
			{
				for (int j = 10; j-- > i;)
				{
					for (int k = 10; k-- > 0; )
					{
						temp = WhattoDo(Deck(1), PlayerHand("", { i,j }), DealerHand({ k }));
						sum += temp.second * (i == j ? 1 : 2);
					}
				}
			}
			return sum / 1000;
		}
		std::pair<Option, double> Calculator::WhattoDo(Deck const& deck, PlayerHand const& player, DealerHand const& dealer)
		{
			int sum;
			bool BJ;
			tie(sum, ignore, BJ) = player.CheckHand();
			if (BJ==true) return { Option::Stand, (1 - DealerExpection(deck,dealer)[6]) * Rate.at(Result::BlackJack) };
			pair<Option, double> best;
			double temp = 0;
			//if stand
			valarray<double> temp_d = DealerExpection(deck, dealer);
			if (21 >= sum && sum >= 17) {
				for (int i = 0; i < sum - 16; i++)
				{
					temp += Rate.at(Result::Win) * temp_d[i];
				}
				temp += Rate.at(Result::Tie) * temp_d[sum - 16];
				if (sum == 21)temp += Rate.at(Result::Tie) * temp_d[6];
			}
			else {
				temp += Rate.at(Result::Win) * temp_d[0];
			}
			best = { Option::Stand,temp };
			if (sum >= 21||player.get_doubled() == true ) return best;

			bool IsTheFirst = (player.size() == 2);

			//if hit
			temp = 0;
			for (size_t i = 0; i < 10; i++)
			{
				if (deck.count(i) != 0) {
					temp += deck.count(i) * WhattoDo(deck - i, player + i, dealer).second;
				}
			}
			temp /= deck.size();
			if (temp > best.second) best = { Option::Hit,temp };
			//if doubledown
			if (IsTheFirst == true && player.get_splitted() == false) {
				temp = 0;
				for (size_t i = 0; i < 10; i++)
				{
					if (deck.count(i) != 0) {
						temp += deck.count(i) * (WhattoDo(deck - i, player * i, dealer).second * 2 - 1);
					}
				}
				temp /= deck.size();
				if (temp > best.second) best = { Option::DoubleDown,temp };
			}
			//if split
			if (IsTheFirst == true && player.splittable()) {
				temp = 0;
				for (size_t i = 0; i < 10; i++)
				{
					if (deck.count(i) != 0) {
						temp += deck.count(i) * (WhattoDo(deck - i, player / i, dealer).second * 2 - 1);
					}
				}
				temp /= deck.size();
				if (temp > best.second) best = { Option::Split,temp };
				temp = 0;
			}
			return best;

		}
		std::valarray<double> Calculator::DealerExpection(Deck const& deck, DealerHand const& dealer)
		{
			//tie(sum,ignore,BJ) = dealer.CheckHand();
			//’x‚¢‚»‚Ì1 ‘½•ª’l‚ğ‚í‚´‚í‚´•Ê‚È•Ï”‚É‘ã“ü‚µ‚Ä‚é‚Ì‚Ætuple‚ª’x‚¢
			pair<int,bool> temp= dealer.CheckHand_speed();//pair‚É‚µ‚Ä‚‘¬‰» 
			valarray<double> r(0.0, 7);//={0,0,0,0,0,0,0}; ’x‚¢‚»‚Ì2 initilizer_list‚Í’x‚¢
			if (temp.second == true) {
				r[6] = 1;
				return r;
			}
			if (21 >= temp.first && temp.first >= 17) {
				r[temp.first - 16] = 1;
				return r;
			}
			if (temp.first > 21) {
				r[0] = 1;
				return r;
			}

			for (size_t i = 0; i < 10; i++)
			{
				if (deck.count(i) != 0) {
					r += deck.count(i) * DealerExpection(deck - i, dealer + i);
				}
			}
			r = r / deck.size();
			return r;
		}
	}
}