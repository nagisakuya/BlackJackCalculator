#include "BlackJackCalculator.h"
using namespace std;

namespace nagisakuya {
	namespace BlackJack {
		Calculator::Calculator(int NumberofDeck,Rule rule,Rate rate) :Table(NumberofDeck,rule,rate) {}
		double Calculator::Calculate_Expection(std::ofstream& file)
		{
			clock_t start = clock();
			double size = deck.size();
			double sum = 0;
			pair<Option, double> temp;
			for (int i = 10; i-- > 0;)
			{
				for (int j = 10; j-- > i;)
				{
					for (int k = 10; k-- > 0; )
					{
						file << "Player:" << Translate(i) << " " << Translate(j) << " \tDealer:" << Translate(k) << "\t";
						cout << "Player:" << Translate(i) << " " << Translate(j) << "\tDealer:" << Translate(k) << "\t";
						temp = WhattoDo(((deck - i) - j) - k, PlayerHand({ i,j }), DealerHand({ k }), file);
						cout << "WhattoDo:" << OptiontoString.at(temp.first);
						cout << "\tExpectedValue:" << temp.second << endl;
						file << "WhattoDo:" << OptiontoString.at(temp.first) << "\t" << "ExpectedValue:" << temp.second << endl;
						sum += temp.second * (i == j ? 1 : 2) * ((double)deck.count(i) * ((double)deck.count(j) - (i == j ? 1 : 0)) * ((double)deck.count(k) - (j == k ? 1 : 0) - (i == k ? 1 : 0)));
					}
				}
			}
			clock_t end = clock();
			cout << "TotalExpectedValue:" << sum / (size * (size - 1) * (size - 2)) << endl;
			cout << " Time:" << (end - start) / CLOCKS_PER_SEC << endl;
			file << "TotalExpectedValue:" << sum / (size * (size - 1) * (size - 2)) << "\t" << " Time:" << (end - start) / CLOCKS_PER_SEC << "sec" << endl;
		}
		std::pair<Option, double> Calculator::WhattoDo(Deck const& deck, PlayerHand const& player, DealerHand const& dealer, ofstream& file)
		{
			int sum;
			bool BJ;
			tie(sum, ignore, BJ) = player.CheckHand();
			if (BJ == true) {
				file << "Stand:" << (1 - DealerExpection(deck, dealer)[6]) * rate.at(Result::BlackJack) << "\t";
				file << "Hit:undefined" << "\t";
				file << "Double:undefined" << "\t";
				file << "Split:undefined" << "\t";
				return { Option::Stand, (1 - DealerExpection(deck,dealer)[6]) * rate.at(Result::BlackJack) };
			}
			pair<Option, double> best;
			double temp = 0;
			//if stand
			valarray<double> temp_d = DealerExpection(deck, dealer);
			if (21 >= sum && sum >= 17) {
				for (int i = 0; i < sum - 16; i++)
				{
					temp += rate.at(Result::Win) * temp_d[i];
				}
				//temp += rate.at(Result::Tie) * temp_d[sum - 16];
				if (sum != 21) {
					for (int i = sum - 16 + 1; i < 7; i++)
					{
						temp += rate.at(Result::Lose) * temp_d[i];
					}
				}
				//if (sum == 21)temp += rate.at(Result::Tie) * temp_d[6];
			}
			else {
				temp += rate.at(Result::Win) * temp_d[0];
				temp += rate.at(Result::Lose) * (1 - temp_d[0]);
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
					temp += deck.count(i) * PlayerExpection(deck - i, player + i, dealer);
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
						temp += deck.count(i) * PlayerExpection(deck - i, player * i, dealer) * 2;
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
						temp += deck.count(i) * PlayerExpection(deck - i, player / i, dealer) * 2;
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
		double Calculator::PlayerExpection(Deck const& deck, PlayerHand const& player, DealerHand const& dealer)
		{
			int sum;
			bool BJ;
			tie(sum, ignore, BJ) = player.CheckHand();//—vC³
			if (BJ == true) return  (1 - DealerExpection(deck, dealer)[6]) * rate.at(Result::BlackJack);
			double best;
			double temp = 0;
			//if stand
			valarray<double> temp_d = DealerExpection(deck, dealer);
			//bust,17,18,19,20,21,BJ
			if (21 >= sum && sum >= 17) {
				for (int i = 0; i < sum - 16; i++)
				{
					temp += rate.at(Result::Win) * temp_d[i];
				}
				//temp += rate.at(Result::Tie) * temp_d[sum - 16]; Result::Tie==0‚È‚Ì‚ÅˆÓ–¡‚È‚µ
				if (sum != 21) {
					for (int i = sum - 16 + 1; i < 7; i++)
					{
						temp += rate.at(Result::Lose) * temp_d[i];
					}
				}
				//if (sum == 21)temp += rate.at(Result::Tie) * temp_d[6];
			}
			else {
				temp += rate.at(Result::Win) * temp_d[0];
				temp += rate.at(Result::Lose) * (1 - temp_d[0]);
			}
			best = temp;
			if (sum >= 21 || player.get_doubled() == true) return best;

			bool IsTheFirst = (player.size() == 2);

			//if hit
			temp = 0;
			for (size_t i = 0; i < 10; i++)
			{
				if (deck.count(i) != 0) {
					temp += deck.count(i) * PlayerExpection(deck - i, player + i, dealer);
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
						temp += deck.count(i) * PlayerExpection(deck - i, player * i, dealer) * 2;
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
						temp += deck.count(i) * PlayerExpection(deck - i, player / i, dealer) * 2 ;
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