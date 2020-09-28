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

		Player::Player(int ID, string name)
		{
			this->name = name;
			this->id = ID;
			hand.first = PlayerHand();
		}
		void Player::play(Deck* deck, Rule const& rule)
		{
			if (hand.first.play(deck, rule) == Option::Split) {
				hand.second = hand.first.split(deck);
				hand.first.play(deck, rule);
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
	}
}