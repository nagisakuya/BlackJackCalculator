#include "BlackJackCalculator.h"
using namespace std;

namespace nagisakuya {
	namespace BlackJack {
		Calculator::Calculator(Deck deck, Rule rule, Rate rate) :Table(deck, rule, rate) {}
		double Calculator::Calculate(std::ofstream& file)
		{
			double sum = 0;
			pair<Option, double> temp_pair;
			Deck temp_deck[3];
			cout << rule.print() << endl;
			cout << rate.print() << endl;
			cout << deck.print() << endl;
			file << rule.print() << endl;
			file << rate.print() << endl;
			file << deck.print() << endl;
			if (rule.at(RuleList::Surrender) == true) file << "Surrender expexted value is always:" << rate.at(Result::Surrender) << endl;
			//file << "Player:(Player 1stcard) (Player 2ndcard) Dealer:(Dealer upcard) Stand:(Ev(Expected value) if stand) Hit:(Ev if hit) Double:(Ev if double) Split:(Ev if split) WhattoDo:(BestOption) ExpectedValue:(Expected value)" << endl;
			clock_t start = clock();
			clock_t temp_clock = clock();
			for (int i = 10; i-- > 0;)
			{
				if (deck.count(i) != 0) {
					temp_deck[0] = deck - i;
					for (int j = 10; j-- > 0;)
					{
						if (temp_deck[0].count(j) != 0) {
							temp_deck[1] = temp_deck[0] - j;
							for (int k = 10; k-- > j; )
							{
								if (temp_deck[1].count(k) != 0) {
									temp_deck[2] = temp_deck[1] - k;
									file << "Dealer: " << Translate(i) << "\tPlayer1st: " << Translate(j) << " \tPlayer2nd: " << Translate(k) << "\t";
									cout << "Dealer: " << Translate(i) << "\tPlayer1st: " << Translate(j) << " \tPlayer2nd: " << Translate(k) << "\t";
									temp_clock = clock();
									temp_pair = WhattoDo(temp_deck[2], PlayerHand({ j,k }), DealerHand({ i }), file);
									cout << "WhattoDo: " << OptiontoString.at(temp_pair.first) << "\tExpectedValue: " << temp_pair.second << "\tTime: " << clock() - temp_clock << endl;
									file << "WhattoDo: " << OptiontoString.at(temp_pair.first) << "\tExpectedValue: " << temp_pair.second << "\tTime: " << clock() - temp_clock << endl;
									sum += temp_pair.second * (i == j ? 1 : 2) * ((double)deck.count(i) * (double)temp_deck[0].count(j) * (double)temp_deck[1].count(k));
								}
							}
						}
					}
				}
			}
			clock_t end = clock();
			double r = sum / ((double)deck.size() * ((double)deck.size() - 1) * ((double)deck.size() - 2));
			std::cout << "TotalExpectedValue:" << r << "\t" << " TotalTime:" << (end - start) / CLOCKS_PER_SEC << "sec" << endl;
			file << "TotalExpectedValue:" << r << "\t" << " TotalTime:" << (end - start) / CLOCKS_PER_SEC << "sec" << endl;
			return r;
		}
		std::pair<Option, double> Calculator::WhattoDo(Deck const& deck, PlayerHand const& player, DealerHand const& dealer, ofstream& file)
		{
			int sum;
			bool BJ;
			double temp = 0;
			valarray<double> temp_d = DealerEV(deck, dealer);
			tie(sum, ignore, BJ) = player.CheckHand();
			unordered_map<Option, double> temp_map;
			bool IsTheFirst = (player.size() == 2);
			Deck temp_deck;
			ofstream null("/dev/null");

			if (BJ == true) {
				temp_map.emplace(Option::Stand, (1 - DealerEV(deck, dealer)[6]) * rate.at(Result::BlackJack));
				goto exit;
			}
			//if stand
			if (21 >= sum && sum >= 17) {
				for (int i = 0; i < sum - 16; i++)
				{
					temp += rate.at(Result::Win) * temp_d[i];
				}
				if (sum != 21) {
					for (int i = sum - 16 + 1; i < 7; i++)
					{
						temp += rate.at(Result::Lose) * temp_d[i];
					}
				}
			}
			else if (sum > 21) {
				temp += rate.at(Result::Lose);
			}
			else {
				temp += rate.at(Result::Win) * temp_d[0];
				temp += rate.at(Result::Lose) * (1 - temp_d[0]);
			}
			temp_map.emplace(Option::Stand, temp);
			if (sum >= 21 || player.get_doubled() == true) {
				goto exit;
			}

			//if hit
			temp = 0;
			for (size_t i = 0; i < 10; i++)
			{
				if (deck.count(i) != 0) {
					temp += deck.count(i) * PlayerEV(deck - i, player + i, dealer);
				}
			}
			temp /= deck.size();
			temp_map.emplace(Option::Hit, temp);

			//if doubledown
			if (IsTheFirst == true && rule.at(RuleList::DoubleAfterSplit) == true ? true : player.get_splitted() == false) {
				temp = 0;
				for (size_t i = 0; i < 10; i++)
				{
					if (deck.count(i) != 0) {
						temp += deck.count(i) * PlayerEV(deck - i, player * i, dealer) * 2;
					}
				}
				temp /= deck.size();
				temp_map.emplace(Option::Double, temp);
			}

			//if split
			if (IsTheFirst == true && player.splittable()) {
				temp = 0;
				for (int i = 10; i-- > 0;)
				{
					if (deck.count(i) != 0) {
						temp_deck = deck - i;
						for (int j = 10; j-- > i;)
						{
							if (temp_deck.count(j) != 0) {
								temp += (i == j ? 1 : 2) * ((double)deck.count(i) * (double)temp_deck.count(j) *
									(WhattoDo(temp_deck - j, player / i, dealer, null).second + WhattoDo(temp_deck - j, player / j, dealer, null).second));
							}
						}
					}
				}
				temp /= (double)deck.size() * ((double)deck.size() - 1);
				temp_map.emplace(Option::Split, temp);
			}

			//if surrender
			if (rule.at(RuleList::Surrender) == true) temp_map.emplace(Option::Surrender, rate.at(Result::Surrender));
		exit:
			Option best = Option::Stand;
			file << "Stand: " << temp_map.at(Option::Stand) << "\t";
			if (temp_map.count(Option::Hit) == 1) { file << "Hit: " << temp_map.at(Option::Hit) << "\t"; if (temp_map.at(best) < temp_map.at(Option::Hit))best = Option::Hit; }
			else file << "Hit: undefined" << "\t";
			if (temp_map.count(Option::Double) == 1) { file << "Double: " << temp_map.at(Option::Double) << "\t"; if (temp_map.at(best) < temp_map.at(Option::Double))best = Option::Double; }
			else file << "Douoble: undefined" << "\t";
			if (temp_map.count(Option::Split) == 1) { file << "Split: " << temp_map.at(Option::Split) << "\t"; if (temp_map.at(best) < temp_map.at(Option::Split))best = Option::Split; }
			else file << "Split: undefined" << "\t";
			if (temp_map.count(Option::Surrender) == 1) if (temp_map.at(best) < temp_map.at(Option::Surrender))best = Option::Surrender;
			return *temp_map.find(best);

		}
		double Calculator::PlayerEV(Deck const& deck, PlayerHand const& player, DealerHand const& dealer)
		{
			const tuple<int, bool, bool > temp_tuple = player.CheckHand();
			if (get<2>(temp_tuple) == true) return  (1 - DealerEV(deck, dealer)[6]) * rate.at(Result::BlackJack);
			double best = -2;
			double stand = 0;
			//if stand
			if (get<0>(temp_tuple) > 11 || player.get_doubled() == true) {
				valarray<double> temp_d = DealerEV(deck, dealer); //bust,17,18,19,20,21,BJ
				if (get<0>(temp_tuple) > 21) {
					stand += rate.at(Result::Lose);
				}
				else if (get<0>(temp_tuple) < 17) {
					stand += rate.at(Result::Win) * temp_d[0];
					stand += rate.at(Result::Lose) * (1 - temp_d[0]);
				}
				else{ //if (21 >= get<0>(temp_tuple) && get<0>(temp_tuple) >= 17) 
					for (int i = 0; i < get<0>(temp_tuple) - 16; i++)
					{
						stand += rate.at(Result::Win) * temp_d[i];
					}
					if (get<0>(temp_tuple) != 21) {
						for (int i = get<0>(temp_tuple) - 16 + 1; i < 7; i++)
						{
							stand += rate.at(Result::Lose) * temp_d[i];
						}
					}
				}
				if (get<0>(temp_tuple) >= 21 || player.get_doubled() == true) return stand;
			}

			double hit = 0;
			bool IsTheFirst = (player.size() == 2);

			//if hit
			for (size_t i = 0; i < 10; i++)
			{
				if (deck.count(i) != 0) {
					hit += deck.count(i) * PlayerEV(deck - i, player + i, dealer);
				}
			}
			hit /= deck.size();
			return hit > stand ? hit : stand;

		}
		std::valarray<double> Calculator::DealerEV(Deck const& deck, DealerHand const& dealer)
		{
			//tie(sum,ignore,BJ) = dealer.CheckHand();
			//’x‚¢‚»‚Ì1 ‘½•ª’l‚ğ‚í‚´‚í‚´•Ê‚È•Ï”‚É‘ã“ü‚µ‚Ä‚é‚Ì‚ª’x‚¢
			//pair‚æ‚è‚Í’x‚¢‚ªtuple‚à‚‘¬
			tuple<int, bool, bool> temp = dealer.CheckHand();//pair‚É‚µ‚Ä‚‘¬‰» 
			valarray<double> r(0.0, 7);//={0,0,0,0,0,0,0}; ’x‚¢‚»‚Ì2 initilizer_list‚Í’x‚¢
			//valarray’x‚¢‚©‚à
			if (get<0>(temp) >= 17) {
				if (get<2>(temp) == true) {
					r[6] = 1;
					return r;
				}
				if (21 >= get<0>(temp) && !(get<0>(temp) == 17 && get<1>(temp) == true && rule.at(RuleList::Soft17Hit) == true)) {
					r[get<0>(temp) - 16] = 1;
					return r;
				}
				if (get<0>(temp) > 21) {
					r[0] = 1;
					return r;
				}
			}
			for (size_t i = 0; i < 10; i++)
			{
				if (deck.count(i) != 0) {
					r += deck.count(i) * DealerEV(deck - i, dealer + i);
				}
			}
			r = r / deck.size();
			return r;
		}
	}
}