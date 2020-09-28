#include "BlackJackCalculator.h"
using namespace std;

namespace nagisakuya {
	namespace BlackJack {
		Calculator::Calculator(int NumberofDeck) :Table(NumberofDeck, Rule(),2.5) {}
		double Calculator::Calculate_Expection(Deck const& deck)
		{
			pair<Option, double> temp;
			double sum = 0;
			double size = deck.size();
			for (int i = 10; i-- > 0;)
			{
				for (int j = 10; j-- > i;)
				{
					for (int k = 10; k-- > 0; )
					{
						temp = WhattoDo(((Deck(1) - i) - j) - k, PlayerHand({ i,j }), DealerHand({ k }));
						sum += temp.second * (i == j ? 1 : 2) * ((double)deck.count(i) * ((double)deck.count(j) - (i == j ? 1 : 0)) * ((double)deck.count(k) - (j == k ? 1 : 0) - (i == k ? 1 : 0)));
					}
				}
			}
			return sum / (size * (size - 1) * (size - 2));
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
					temp += deck.count(i) * WhattoDo_speed(deck - i, player + i, dealer);
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
						temp += deck.count(i) * (WhattoDo_speed(deck - i, player * i, dealer) * 2 - 1);
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
						temp += deck.count(i) * (WhattoDo_speed(deck - i, player / i, dealer) * 2 - 1);
					}
				}
				temp /= deck.size();
				if (temp > best.second) best = { Option::Split,temp };
				temp = 0;
			}
			return best;

		}
		std::pair<Option, double> Calculator::WhattoDo_writedown(Deck const& deck, PlayerHand const& player, DealerHand const& dealer, ofstream& file)
		{
			int sum;
			bool BJ;
			tie(sum, ignore, BJ) = player.CheckHand();
			if (BJ == true) {
				file << "Stand:" << (1 - DealerExpection(deck, dealer)[6]) * Rate.at(Result::BlackJack) << "\t";
				file << "Hit:undefined" << "\t";
				file << "Double:undefined" << "\t";
				file << "Split:undefined" << "\t";
				return { Option::Stand, (1 - DealerExpection(deck,dealer)[6]) * Rate.at(Result::BlackJack) };
			}
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
			file << "Stand:" << temp << "\t";
			if (sum >= 21 || player.get_doubled() == true) {
				file << "Hit:undefined" << "\t";
				file << "Double:undefined" << "\t";
				file << "Split:undefined" << "\t";
				return best;
			}

			bool IsTheFirst = (player.size() == 2);

			//if hit
			temp = 0;
			for (size_t i = 0; i < 10; i++)
			{
				if (deck.count(i) != 0) {
					temp += deck.count(i) * WhattoDo_speed(deck - i, player + i, dealer);
				}
			}
			temp /= deck.size();
			file << "Hit:" << temp << "  \t";
			if (temp > best.second) best = { Option::Hit,temp };
			//if doubledown
			if (IsTheFirst == true && player.get_splitted() == false) {
				temp = 0;
				for (size_t i = 0; i < 10; i++)
				{
					if (deck.count(i) != 0) {
						temp += deck.count(i) * (WhattoDo_speed(deck - i, player * i, dealer) * 2 - 1);
					}
				}
				temp /= deck.size();
				file << "Doule:" << temp << "\t";
				if (temp > best.second) best = { Option::DoubleDown,temp };
			}else
				file << "Double:undefined" << "\t";
			//if split
			if (IsTheFirst == true && player.splittable()) {
				temp = 0;
				for (size_t i = 0; i < 10; i++)
				{
					if (deck.count(i) != 0) {
						temp += deck.count(i) * (WhattoDo_speed(deck - i, player / i, dealer) * 2 - 1);
					}
				}
				temp /= deck.size();
				file << "Split:" << temp << "\t";
				if (temp > best.second) best = { Option::Split,temp };
				temp = 0;
			}else 
				file << "Split:undefined" << "\t";
			return best;

		}
		double Calculator::WhattoDo_speed(Deck const& deck, PlayerHand const& player, DealerHand const& dealer)
		{
			int sum;
			bool BJ;
			tie(sum, ignore, BJ) = player.CheckHand();
			if (BJ == true) return  (1 - DealerExpection(deck, dealer)[6]) * Rate.at(Result::BlackJack);
			double best;
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
			best = temp;
			if (sum >= 21 || player.get_doubled() == true) return best;

			bool IsTheFirst = (player.size() == 2);

			//if hit
			temp = 0;
			for (size_t i = 0; i < 10; i++)
			{
				if (deck.count(i) != 0) {
					temp += deck.count(i) * WhattoDo_speed(deck - i, player + i, dealer);
				}
			}
			temp /= deck.size();
			if (temp > best) best = temp;
			//if doubledown
			if (IsTheFirst == true && player.get_splitted() == false) {
				temp = 0;
				for (size_t i = 0; i < 10; i++)
				{
					if (deck.count(i) != 0) {
						temp += deck.count(i) * (WhattoDo_speed(deck - i, player * i, dealer)* 2 - 1);
					}
				}
				temp /= deck.size();
				if (temp > best) best = temp;
			}
			//if split
			if (IsTheFirst == true && player.splittable()) {
				temp = 0;
				for (size_t i = 0; i < 10; i++)
				{
					if (deck.count(i) != 0) {
						temp += deck.count(i) * (WhattoDo_speed(deck - i, player / i, dealer) * 2 - 1);
					}
				}
				temp /= deck.size();
				if (temp > best) best = temp ;
				temp = 0;
			}
			return best;

		}
		std::valarray<double> Calculator::DealerExpection(Deck const& deck, DealerHand const& dealer)
		{
			//tie(sum,ignore,BJ) = dealer.CheckHand();
			//íxÇ¢ÇªÇÃ1 ëΩï™ílÇÇÌÇ¥ÇÌÇ¥ï Ç»ïœêîÇ…ë„ì¸ÇµÇƒÇÈÇÃÇ∆tupleÇ™íxÇ¢
			pair<int,bool> temp= dealer.CheckHand_speed();//pairÇ…ÇµÇƒçÇë¨âª 
			valarray<double> r(0.0, 7);//={0,0,0,0,0,0,0}; íxÇ¢ÇªÇÃ2 initilizer_listÇÕíxÇ¢
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