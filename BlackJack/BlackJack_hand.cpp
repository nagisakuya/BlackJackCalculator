#include "BlackJack.h"
using namespace std;
namespace nagisakuya {
	namespace BlackJack {
		Hand::Hand(vector<int> input) {
			content = input;
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
			content.push_back(input);
		}

		void Hand::print() const {
			int sum = 0;
			int AceCount = 0;
			for (size_t i = 0; i < content.size(); i++)
			{
				cout << Translate(content[i]) << " ";
				if (content[i] == 0) {
					sum += 11;
					AceCount++;
				}
				else if (content[i] >= 9) {
					sum += 10;
				}
				else {
					sum += content[i] + 1;
				}
				if (AceCount > 0 && sum > 21) {
					sum -= 10;
					AceCount--;
				}
			}
			cout << " sum is " << sum << endl;
		}

		size_t Hand::size() const
		{
			return content.size();
		}

		tuple<int, bool, bool> Hand::CheckHand() const {
			int sum = 0;
			int AceCount = 0;
			size_t size = content.size();
			for (size_t i = 0; i < size; i++)
			{
				if (content[i] == 0) {
					sum += 11;
					AceCount++;
				}
				else if (content[i] >= 9) {
					sum += 10;
				}
				else {
					sum += content[i] + 1;
				}
				if (AceCount > 0 && sum > 21) {
					sum -= 10;
					AceCount--;
				}
			}
			return { sum ,AceCount == 0 ? false : true ,(size == 2 && sum == 21) ? true : false };
		}

		std::pair<int, bool> Hand::CheckHand_speed() const
		{
			//initializer_listÇÕíxÇ¢ÇÃÇ≈éñëOÇ…pairÇçÏÇ¡ÇƒÇ®Ç≠ÅB(pairÇÕçÇë¨ÇæÇ∆évÇ§)
			pair<int, bool> r;
			r.first = 0;
			int AceCount = 0;
			size_t size = content.size();
			for (size_t i = 0; i < size; i++)
			{
				if (content[i] == 0) {
					r.first += 11;
					AceCount++;
				}
				else {
					r.first += content[i] + 1;
				}
				if (AceCount > 0 && r.first > 21) {
					r.first -= 10;
					AceCount--;
				}
			}
			r.second = (size == 2 && r.first == 21) ? true : false;
			return r;
		}


		DealerHand::DealerHand(vector<int> input) :Hand(input) {
		}

		void DealerHand::print() const
		{
			if (content.size() == 1) {
				cout << "Dealer up card is " << Translate(content[0]) << endl;
			}
			else {
				cout << "Dealer hand is ";
				Hand::print();
			}
		}

		void DealerHand::hituntil17(Deck& deck, Rule const& rule)
		{
			int sum;
			bool soft;
			for (;;) {
				add(deck.DrowRandom());
				std::tie(sum, soft, std::ignore) = CheckHand();
				if (sum >= 17 && !(soft == true && sum == 17 && rule.at(RuleList::Soft17Hit) == true)) break;
			}
			print();
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

		PlayerHand::PlayerHand(vector<int> input, bool splitted , bool doubled) :Hand(input) {
			this->splitted = splitted;
			this->doubled = doubled;
		}

		bool PlayerHand::splittable() const {
			if (splitted == false && content.size() == 2 && (content[0] == content[1] || content[0] >= 9 && content[1] >= 9))return true;
			else return false;
		}

		PlayerHand PlayerHand::split(Deck* deck)
		{
			splitted = true;
			PlayerHand r = PlayerHand({ content[1] ,deck->DrowRandom() }, true);
			content.pop_back();
			add(deck->DrowRandom());
			return r;
		}

		Option PlayerHand::play(Deck* deck, Rule const& rule)
		{
			bool IsTheFirst = (size() == 2);
			print();
			switch (std::get<0>(CheckHand()) >= 21 ? Option::Stand : AskOption(
				IsTheFirst && splittable(),
				splitted == true ? IsTheFirst && rule.at(RuleList::DoubleAfterSplit) : IsTheFirst,
				rule.at(RuleList::Surrender) && IsTheFirst && splitted == false)
				) {
			case Option::Hit:
				add(deck->DrowRandom());
				play(deck, rule);
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

		void PlayerHand::judge(DealerHand const& dealer)
		{
			if (result == Result::undefined) {
				result = Judge(*this, dealer);
			}
			cout << " Result:" << ResulttoString.at(get_result()) << endl;
		}

	}
}