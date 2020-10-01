#include "BlackJack.h"

using namespace std;

namespace nagisakuya {
	namespace BlackJack {

		Rule::Rule(bool Soft17Hit, bool Surrender, bool DoubleAfterSplit)
		{
			emplace(RuleList::Soft17Hit, Soft17Hit);
			emplace(RuleList::Surrender, Surrender);
			emplace(RuleList::DoubleAfterSplit, DoubleAfterSplit);
		}

		std::string Rule::print() const
		{
			string r;
			r += "Soft17Hit: ";
			if (at(RuleList::Soft17Hit) == true) r += "true\t";
			else r += "false\t";
			r += "Surrender: ";
			if (at(RuleList::Surrender) == true) r += "true\t";
			else r += "false\t";
			r += "DoubleAfterSplit: ";
			if (at(RuleList::DoubleAfterSplit) == true) r += "true\t";
			else r += "false\t";
			return r;
		}

		Player::Player(int ID, string name)
		{
			this->name = name;
			this->id = ID;
			hand.first = PlayerHand();
		}
		void Player::play(Deck* deck, Rule const& rule)
		{
			cout << name << ":" << endl;
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
			case 13:
				return "Joker";
			default:
				return std::to_string(input + 1);
			}
		}
		Rate::Rate(double BlackJackRate)
		{
			emplace(Result::Win, 1);
			emplace(Result::Lose,-1);
			emplace(Result::Tie, 0);
			emplace(Result::BlackJack, BlackJackRate);
			emplace(Result::Surrender, at(Result::Lose) / 2);
			emplace(Result::DoubledWin, at(Result::Win) * 2);
			emplace(Result::DoubledLose, at(Result::Lose) * 2);
		}
		std::string Rate::print() const
		{
			string r;
			r += "Win: " + to_string(at(Result::Win)) + "\t";
			r += "Tie: " + to_string(at(Result::Tie)) + "\t";
			r += "Lose: " + to_string(at(Result::Lose)) + "\t";
			r += "BlackJack: " + to_string(at(Result::BlackJack)) + "\t";
			return r;
		}
	}
}