#include "BlackJack.h"
using namespace std;
namespace nagisakuya {
	namespace BlackJack {
		Hand::Hand(string name, vector<int> input) {
			this->name = name;
			hand = input;
		}

		Option PlayerHand::AskOption(bool Split_enable, bool DoubleDown_enable, bool Surrender_enable) {
			string temp;
			while (true) {
				cout << "Hit(H) or Stand(S)";
				if (DoubleDown_enable) cout << " or DoubleDown(D)";
				if (Split_enable) cout << " or Split(P)";
				if (Surrender_enable) cout << " or Surrender(R)";
				cout << endl;
				cin >> temp;
				if (temp == "H") return Option::Hit;
				else if (temp == "S") return Option::Stand;
				else if (temp == "D" && DoubleDown_enable == true) return Option::DoubleDown;
				else if (temp == "P" && Split_enable == true) return Option::Split;
				else if (temp == "R" && Surrender_enable == true) return Option::Surrender;
				else cout << "error " << temp << " is out of option" << endl;;
			}
		}

		void Hand::add(int input) {
			hand.push_back(input);
		}

		void Hand::print() {
			int sum = 0;
			int AceCount = 0;
			cout << name << " is ";
			for (size_t i = 0; i < hand.size(); i++)
			{
				cout << Translate(hand[i]) << " ";
				if (hand[i] == 0) {
					sum += 11;
					AceCount++;
				}
				else if (hand[i] >= 9) {
					sum += 10;
				}
				else {
					sum += hand[i] + 1;
				}
				if (AceCount > 0 && sum > 21) {
					sum -= 10;
					AceCount--;
				}
			}
			cout << " sum is " << sum << endl;
		}

		tuple<int, bool, bool> Hand::CheckHand() {
			int sum = 0;
			int AceCount = 0;
			size_t size = hand.size();
			for (size_t i = 0; i < size; i++)
			{
				if (hand[i] == 0) {
					sum += 11;
					AceCount++;
				}
				else if (hand[i] >= 9) {
					sum += 10;
				}
				else {
					sum += hand[i] + 1;
				}
				if (AceCount > 0 && sum > 21) {
					sum -= 10;
					AceCount--;
				}
			}
			return { sum ,AceCount == 0 ? false : true ,(size == 2 && sum == 21) ? true : false };
		}


		DealerHand::DealerHand(string name, vector<int> input) :Hand(name, input) {
		}

		void DealerHand::print()
		{
			if (hand.size() == 1) {
				cout << name << " up card is " << Translate(hand[0]) << endl;
			}
			else Hand::print();
		}

		void DealerHand::hituntil17(Deck* deck, Rule rule)
		{
			int sum;
			bool soft;
			for (;;) {
				this->add(deck->DrowRandom());
				std::tie(sum, soft, std::ignore) = this->CheckHand();
				if (sum >= 17 && !(soft == true && sum == 17 && rule.get("Soft17Hit") == true)) break;
			}
			this->print();
		}


		const map< Result, string> PlayerHand::ResulttoString = {
			{Result::Win		,"Win"},
			{Result::Lose		,"Lose"},
			{Result::Tie		,"Tie"},
			{Result::BlackJack	,"BlackJack"},
			{Result::Surrender	,"Surrender"},
			{Result::DoubledWin	,"DoubledWin"},
			{Result::DoubledLose,"DoubledLose"}
		};

		PlayerHand::PlayerHand(string name, vector<int> input) :Hand(name, input) {
		}

		bool PlayerHand::splittable() {
			if (splitted == false && hand.size() == 2 && (hand[0] == hand[1] || hand[0] >= 9 && hand[1] >= 9))return true;
			else return false;
		}

		PlayerHand PlayerHand::split(Deck* deck)
		{
			splitted = true;
			name = "Primal" + name;
			PlayerHand r = PlayerHand("Splitted" + name, { hand[1] ,deck->DrowRandom() });
			hand.pop_back();
			add(deck->DrowRandom());
			return r;
		}

		Option PlayerHand::play(Deck* deck, Rule rule, bool IsTheFirst)
		{
			print();
			switch (std::get<0>(CheckHand()) >= 21 ? Option::Stand : AskOption(IsTheFirst && splittable(), IsTheFirst, rule.get("Surrender"))) {
			case Option::Hit:
				add(deck->DrowRandom());
				play(deck, rule, false);
				return Option::Hit;
			case Option::Stand:
				return Option::Stand;
			case Option::DoubleDown:
				doubled = true;
				add(deck->DrowRandom());
				print();
				return Option::DoubleDown;
			case Option::Split:
				return Option::Split;
			case Option::Surrender:
				result = Result::Surrender;
				return Option::Surrender;
			}
			return Option();
		}

		void PlayerHand::judge(DealerHand dealer)
		{
			if (result == Result::undefined) {
				result = Judge(*this, dealer);
				cout << name << " Result:" << ResulttoString.at(get_result()) << endl;
			}
		}

	}
}