#include "pch.h"
#include "BlackJackCalculator.h"
using namespace std;

namespace nagisakuya {
	using namespace Utility;
	namespace BlackJack {
		Calculator::Calculator(Deck deck, Rule rule, Rate rate, DealerHand dealer) :Table(deck, rule, rate, dealer) {}
		double Calculator::calculate(std::function<void()> do_before, std::function<double(Calculator, PlayerHand)> do_while, std::function<void(double, double)> do_after)
		{
			double sum = 0;
			do_before();
			clock_t start = clock();
			for (int i = 10; i-- > 0;)
			{
				int l = deck.count(i);
				if (l != 0) {
					for (int j = 10; j-- > 0;)
					{
						int m = (deck - Card(i)).count(j);
						if (m != 0) {
							for (int k = 10; k-- > j; )
							{
								int n = (deck - Card(i) - Card(j)).count(k);
								if (n != 0) {
									sum += do_while(Calculator(deck - Card(i) - Card(j) - Card(k), rule, rate, DealerHand({ Card(i) })), PlayerHand({ Card(j),Card(k) }))* (i == j ? 1 : 2)* ((double)l * m * n);
								}
							}
						}
					}
				}
			}
			double r = sum / ((double)deck.size() * ((double)deck.size() - 1) * ((double)deck.size() - 2));
			do_after(r, (double)clock() - start);
			return r;
		}
		double Calculator::calculate_withoutput(cfstream& cfout)
		{
			auto before = [&] {
				cfout << PrintStatus();
				if (rule.at(Rule::List::Surrender) == true) cfout << "Surrender expexted value is always:" << rate.get(Result::Surrender) << endl;
				//file << "Player:(Player 1stcard) (Player 2ndcard) Dealer:(Dealer upcard) Stand:(Ev(Expected value) if stand) Hit:(Ev if hit) Double:(Ev if double) Split:(Ev if split) WhattoDo:(BestOption) ExpectedValue:(Expected value)" << endl;
			};
			auto while_ = [&](Calculator table, PlayerHand player) {
				cfout << "Dealer: " << table.getdealer().get_upcard().str() << "\tPlayer1st: " << player[0].str() << " \tPlayer2nd: " << player[1].str() << "\t";
				clock_t temp_clock = clock();
				unordered_map<Option, double> temp_map = table.PlayerEV_all(player);
				Option best = Option::Stand;
				cfout.fonly() << "Stand: " << temp_map.at(Option::Stand) << "\t";
				if (temp_map.count(Option::Hit) == 1) { cfout.fonly() << "Hit: " << temp_map.at(Option::Hit) << "\t"; if (temp_map.at(best) < temp_map.at(Option::Hit))best = Option::Hit; }
				else cfout.fonly() << "Hit: undefined" << "\t";
				if (temp_map.count(Option::Double) == 1) { cfout.fonly() << "Double: " << temp_map.at(Option::Double) << "\t"; if (temp_map.at(best) < temp_map.at(Option::Double))best = Option::Double; }
				else cfout.fonly() << "Douoble: undefined" << "\t";
				if (temp_map.count(Option::Split) == 1) { cfout.fonly() << "Split: " << temp_map.at(Option::Split) << "\t"; if (temp_map.at(best) < temp_map.at(Option::Split))best = Option::Split; }
				else cfout.fonly() << "Split: undefined" << "\t";
				if (temp_map.count(Option::Surrender) == 1) if (temp_map.at(best) < temp_map.at(Option::Surrender))best = Option::Surrender;
				cfout << "WhattoDo: " << OptionandString.at((*temp_map.find(best)).first) << "\tExpectedValue: " << (*temp_map.find(best)).second << "\tTime: " << ((double)clock() - (double)temp_clock) / (double)CLOCKS_PER_SEC << endl;
				return (*temp_map.find(best)).second;
			};
			auto after = [&](double d, double clock) {
				cfout << "TotalExpectedValue:" << d << "\t" << " TotalTime:" << clock / CLOCKS_PER_SEC << "sec" << endl;
			};
			return calculate(before, while_, after);
		}
		double Calculator::calculate_onstrategy(Utility::cfstream& cfout, Strategy strategy)
		{
			auto before = [&] {
				cfout << "Calculating on strategy" << endl;
				cfout << strategy.print();
				cfout << PrintStatus();
			};
			auto while_ = [&](Calculator table, PlayerHand player) {
				cfout << "Dealer: " << table.getdealer().get_upcard().str() << "\tPlayer1st: " << player[0].str() << " \tPlayer2nd: " << player[1].str() << "\t";
				clock_t temp_clock = clock();
				double ev = table.If_onstrategy(deck, player);
				cfout << "ExpectedValue: " << ev << "\tTime: " << ((double)clock() - (double)temp_clock) / (double)CLOCKS_PER_SEC << endl;
				return ev;
			};
			auto after = [&](double d, double clock) {
				cfout << "TotalExpectedValue:" << d << "\t" << " TotalTime:" << clock / CLOCKS_PER_SEC << "sec" << endl;
			};
			return calculate(before, while_, after);
		}
		std::pair<Option, double> Calculator::WhattoDo(PlayerHand const& player)
		{
			unordered_map<Option, double> temp_map = Calculator(deck, rule, rate, dealer).PlayerEV_all(player);
			Option best = Option::Stand;
			if (temp_map.count(Option::Hit) == 1) { if (temp_map.at(best) < temp_map.at(Option::Hit))best = Option::Hit; }
			if (temp_map.count(Option::Double) == 1) { if (temp_map.at(best) < temp_map.at(Option::Double))best = Option::Double; }
			if (temp_map.count(Option::Split) == 1) { if (temp_map.at(best) < temp_map.at(Option::Split))best = Option::Split; }
			if (temp_map.count(Option::Surrender) == 1) if (temp_map.at(best) < temp_map.at(Option::Surrender))best = Option::Surrender;
			return *temp_map.find(best);
		}
		std::unordered_map<Option, double> Calculator::PlayerEV_all(PlayerHand const& player)
		{
			tuple<int, bool, bool> temp_tuple = player.CheckHand();
			unordered_map<Option, double> temp_map;
			bool IsTheFirst = (player.size() == 2);
			temp_map.emplace(Option::Stand, If_stand(deck, player));
			if (!(get<0>(temp_tuple) == 21 || get<2>(temp_tuple) == true || player.get_doubled() == true)) {
				temp_map.emplace(Option::Hit, If_hit(deck, player));
				if (IsTheFirst == true && rule.at(Rule::List::DoubleAfterSplit) == true ? true : player.get_splitted() == false) temp_map.emplace(Option::Double, If_double(deck, player));
				if (IsTheFirst == true && player.splittable()) 	temp_map.emplace(Option::Split, If_split(deck, player));
				if (rule.at(Rule::List::Surrender) == true) temp_map.emplace(Option::Surrender, rate.get(Result::Surrender));
			}
			return temp_map;
		}
		double Calculator::PlayerEV(Deck const& deck, PlayerHand const& player)
		{
			const tuple<int, bool, bool > temp_tuple = player.CheckHand();
			if (get<0>(temp_tuple) > 11) {
				double stand = If_stand(deck, player);
				if (get<0>(temp_tuple) >= 21 || player.get_doubled() == true) return stand;
				double hit = If_hit(deck, player);
				return hit > stand ? hit : stand;
			}
			else return If_hit(deck, player);

		}
		double Calculator::PlayerEV_afterP(Deck const& deck, PlayerHand const& player)
		{
			const tuple<int, bool, bool > temp_tuple = player.CheckHand();
			if (rule.at(Rule::List::DoubleAfterSplit) == true) {
				if (get<0>(temp_tuple) > 11) {
					double stand = If_stand(deck, player);
					if (get<0>(temp_tuple) >= 21 || player.get_doubled() == true) return stand;
					double hit = If_hit(deck, player);
					double Double = If_double(deck, player);
					return hit > stand ? (hit > Double ? hit : Double) : (stand > Double ? stand : Double);
				}
				else {
					double hit = If_hit(deck, player);
					double Double = If_double(deck, player);
					return hit > Double ? hit : Double;
				}
			}
			else return PlayerEV(deck, player);
		}
		double Calculator::If_stand(Deck const& deck, PlayerHand const& player)
		{
			const tuple<int, bool, bool > temp_tuple = player.CheckHand();
			if (get<2>(temp_tuple) == true) return  (1 - DealerEV(deck, dealer)[6]) * rate.get(Result::BlackJack);
			valarray<double> temp_d = DealerEV(deck, dealer); //bust,17,18,19,20,21,BJ
			if (get<0>(temp_tuple) > 21) {
				return rate.get(Result::Lose);
			}
			else if (get<0>(temp_tuple) < 17) {
				return (rate.get(Result::Win) * temp_d[0])
					+ (rate.get(Result::Lose) * (1 - temp_d[0]));
			}
			else {
				double r = 0;
				for (int i = 0; i < get<0>(temp_tuple) - 16; i++)
				{
					r += rate.get(Result::Win) * temp_d[i];
				}
				if (get<0>(temp_tuple) != 21) {
					for (int i = get<0>(temp_tuple) - 16 + 1; i < 7; i++)
					{
						r += rate.get(Result::Lose) * temp_d[i];
					}
				}
				return r;
			}
		}
		double Calculator::If_hit(Deck const& deck, PlayerHand const& player)
		{
			double r = 0;
			for (size_t i = 0; i < 10; i++)
			{
				if (deck.count(i) != 0) {
					r += deck.count(i) * PlayerEV(deck - Card(i), player.Ifhit(i));
				}
			}
			r /= deck.size();
			return r;
		}
		double Calculator::If_double(Deck const& deck, PlayerHand const& player)
		{
			double r = 0;
			for (size_t i = 0; i < 10; i++)
			{
				if (deck.count(i) != 0) {
					r += deck.count(i) * If_stand(deck - Card(i), player.Ifdouble(i)) * 2;
				}
			}
			r /= deck.size();
			return r;
		}
		double Calculator::If_split(Deck const& deck, PlayerHand const& player)
		{
			double r = 0;
			Deck temp_deck;
			for (int i = 10; i-- > 0;)
			{
				if (deck.count(i) != 0) {
					temp_deck = deck - Card(i);
					for (int j = 10; j-- > i;)
					{
						if (temp_deck.count(j) != 0) {
							r += (i == j ? 1 : 2) * ((double)deck.count(i) * (double)temp_deck.count(j) *
								(PlayerEV_afterP(temp_deck - Card(i), player.Ifsplit(i)) + PlayerEV_afterP(temp_deck- Card(i), player.Ifsplit(j))));
						}
					}
				}
			}
			return r / ((double)deck.size() * ((double)deck.size() - 1));
		}
		double Calculator::If_onstrategy(Deck const& deck, PlayerHand const& player, Strategy strategy)
		{
			auto temp_option = strategy.find(dealer, player, rule);
			if (temp_option == Option::Stand) {
				return If_stand(deck, player);
			}
			else if (temp_option == Option::Hit) {
				double sum = 0;
				for (size_t i = 0; i < 10; i++)
				{
					if (deck.count(i) != 0) {
						sum += deck.count(i) * If_onstrategy(deck - Card(i), player.Ifhit(i));
					}
				}
				return sum / deck.size();
			}
			else if (temp_option == Option::Double) {
				double sum = 0;
				for (size_t i = 0; i < 10; i++)
				{
					if (deck.count(i) != 0) {
						sum += deck.count(i) * If_stand(deck - Card(i), player.Ifdouble(i)) * 2;
					}
				}
				return sum / deck.size();
			}
			else if (temp_option == Option::Split) {
				double sum = 0;
				Deck temp_deck;
				for (int i = 10; i-- > 0;)
				{
					if (deck.count(i) != 0) {
						temp_deck = deck- Card(i);
						for (int j = 10; j-- > i;)
						{
							if (temp_deck.count(j) != 0) {
								sum += (i == j ? 1 : 2) * ((double)deck.count(i) * (double)temp_deck.count(j) *
									(If_onstrategy(temp_deck- Card(i), player.Ifsplit(i)) + If_onstrategy(temp_deck- Card(i), player.Ifsplit(j))));
							}
						}
					}
				}
				return sum / ((double)deck.size() * ((double)deck.size() - 1));
			}
			else if (temp_option == Option::Surrender) {
				return rate.get(Result::Surrender);
			}
			else {
				cout << "something wrong in If_onstrategy may be in Option enum";
				exit(0);
			};
		}
		std::valarray<double> Calculator::DealerEV(Deck const& deck, DealerHand const& dealer)
		{
			tuple<int, bool, bool> temp = dealer.CheckHand();
			valarray<double> r(0.0, 7);
			if (get<0>(temp) >= 17) {
				if (get<2>(temp) == true) {
					r[6] = 1;
					return r;
				}
				if (21 >= get<0>(temp) && !(get<0>(temp) == 17 && get<1>(temp) == true && rule.at(Rule::List::Soft17Hit) == true)) {
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
					r += deck.count(i) * DealerEV(deck- Card(i), dealer.Ifhit(i));
				}
			}
			return r / deck.size();
		}
	}
}