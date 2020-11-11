#include "pch.h"
#include "BlackJack.h"

using namespace std;

namespace nagisakuya {
	namespace BlackJack {

		Rule::Rule(bool Soft17Hit, bool Surrender, bool DoubleAfterSplit)
		{
			element.emplace(Rule::List::Soft17Hit, Soft17Hit);
			element.emplace(Rule::List::Surrender, Surrender);
			element.emplace(Rule::List::DoubleAfterSplit, DoubleAfterSplit);
		}
		Rule::Rule(std::istream& input)
		{
			import(input);
		}
		const nagisakuya::Utility::bijection<Rule::List, std::string> Rule::RuleandString = {
			{ List::Soft17Hit,"Soft17Hit"},
			{ List::Surrender,"Surrender" },
			{ List::DoubleAfterSplit,"DoubleAfterSplit" }
		};
		std::string Rule::print() const
		{
			string re;
			for (List i = (List)0; i < List::EoE; i = (List)((int)i + 1)) {
				re += RuleandString.at_ftos(i) + ": ";
				if (element.at(i) == true) re += "true\t";
				else re += "false\t";
			}
			return re;
		}
		void Rule::import(std::istream& input)
		{
			for (List i = (List)0; i < List::EoE; i = (List)((int)i + 1))
			{
				string temp_str, temp_rulename;
				input >> temp_rulename;
				if (RuleandString.at_ftos(i) + ":" == temp_rulename) {
					input >> temp_str;
					if (temp_str == "true") {
						element[i] = true;
					}
					else if (temp_str == "false") {
						element[i] = false;
					}
					else {
						cout << "[ERROR] Wrong text in rule string";
						exit(0);
					}
				}
				else {
					cout << "[ERROR] Wrong text in rule name";
					exit(0);
				}
			}
		}
		Player::Player(int ID, string name)
		{
			this->name = name;
			this->id = ID;
			hand.first = PlayerHand();
		}
		void Player::play(Deck* deck, Rule const& rule)
		{
			cout << name << ": ";
			if (hand.first.play(deck, rule) == Option::Split) {
				hand.second = hand.first.split(deck);
				cout << "Primal hand:" << endl;
				hand.first.play(deck, rule);
				cout << "Secondaly hand:" << endl;
				hand.second.play(deck, rule);
			}

		}

		Result Judge(PlayerHand const& playerhand, DealerHand const& dealer) {
			if (playerhand.get_result() != Result::undefined)return playerhand.get_result();
			int dealer_sum, player_sum;
			bool playerBJ, dealerBJ;
			std::tie(player_sum, std::ignore, playerBJ) = playerhand.CheckHand();
			std::tie(dealer_sum, std::ignore, dealerBJ) = dealer.CheckHand();
			if (dealer_sum > 21)dealer_sum = 0;
			if (player_sum > 21)player_sum = -1;
			if (playerBJ == true && dealerBJ == false) {
				return Result::BlackJack;
			}
			else if (player_sum < dealer_sum) {
				if (playerhand.get_doubled() == false) {
					return Result::Lose;
				}
				else {
					return Result::DoubledLose;
				}
			}
			else if (player_sum == dealer_sum) {
				return Result::Tie;
			}
			else {
				if (playerhand.get_doubled() == false) {
					return Result::Win;
				}
				else {
					return Result::DoubledWin;
				}
			}
		}

		string Translate(int input) {
			switch (input) {
			case 0:
				return "A";
			case 9:
				return "T";
			case 10:
				return "J";
			case 11:
				return "Q";
			case 12:
				return "K";
			default:
				return std::to_string(input + 1);
			}
		}
		int Translate(std::string input)
		{
			if (input == ",") {
				cout << "";
			}
			if (input == "A") {
				return 0;
			}
			else if (input == "T") {
				return 9;
			}
			else if (input == "J") {
				return 10;
			}
			else if (input == "Q") {
				return 11;
			}
			else if (input == "K") {
				return 12;
			}
			else {
				return stoi(input) - 1;
			}
		}
		Rate::Rate(double BlackJackRate)
		{
			element.emplace(Result::Win, 1);
			element.emplace(Result::Lose, -1);
			element.emplace(Result::Tie, 0);
			element.emplace(Result::BlackJack, BlackJackRate);
			element.emplace(Result::Surrender, at(Result::Lose) / 2);
			element.emplace(Result::DoubledWin, at(Result::Win) * 2);
			element.emplace(Result::DoubledLose, at(Result::Lose) * 2);
		}
		std::string Rate::print() const
		{
			string r;
			r += "Win: " + to_string(element.at(Result::Win)) + "\t";
			r += "Tie: " + to_string(element.at(Result::Tie)) + "\t";
			r += "Lose: " + to_string(element.at(Result::Lose)) + "\t";
			r += "BlackJack: " + to_string(element.at(Result::BlackJack)) + "\t";
			return r;
		}
	}
}