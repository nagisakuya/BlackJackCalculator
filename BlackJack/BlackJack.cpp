#include "BlackJack.h"

using namespace std;
using json = nlohmann::json;

namespace nagisakuya {
	namespace BlackJack {
		Table::Table(int Numberofdeck, Rule rule,double BlackJackRate) {
			deck = Deck(Numberofdeck, 0);
			Rate = {
			{Result::Win		,2},
			{Result::Lose		,0},
			{Result::Tie		,1},
			{Result::BlackJack	,BlackJackRate},
			{Result::Surrender	,0.5},
			{Result::DoubledWin	,4},
			{Result::DoubledLose,-1}
			};
		}
		void Table::play() {
			if (PlayerList.size() == 0) {
				cout << "there is no player" << endl;
				return;
			}
			DealerHand dealer = DealerHand("Dealer hand", { deck.DrowRandom() });
			dealer.print();
			//関数ポインタでplayereach関数をを作ろうと思った
			for (Player& i : PlayerList)
			{
				i.hand.first = PlayerHand(i.get_name() + " hand", { deck.DrowRandom() ,deck.DrowRandom() });
			}
			for (Player& i : PlayerList)
			{
				i.play(&deck, rule);
			}
			dealer.hituntil17(&deck, rule);
			for (Player& i : PlayerList)
			{
				i.hand.first.judge(dealer);
				if (i.hand.first.get_splitted() == true)i.hand.second.judge(dealer);
			}
		}
		void Table::Test() {}
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
		bool Table::addplayer(Player input)
		{
			int input_id = input.get_ID();
			for (auto i : PlayerList) {
				if (i.get_ID() == input_id) {
					cout << "The player is already on This table" << endl;
					return false;
				}
			}
			PlayerList.push_back(input);
			return true;
		}
		Result Judge(PlayerHand playerhand, DealerHand dealer) {
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

		Deck::Deck(int NumberofDeck, int Joker) {
			for (size_t i = 0; i < 13; i++)
			{
				content[i] = 4 * NumberofDeck;
			}
			content[13] = Joker;
		}
		void Deck::print() {
			for (int i = 0; i < 14; i++)
			{
				cout << "number of " << Translate(i) << "=" << content[i] << endl;
			}
		}
		int Deck::DrowRandom() {
			int temp = rand() % this->size();
			int sum = 0;
			for (size_t i = 0; i < 14; i++)
			{
				sum += content[i];
				if (sum >= temp) {
					content[i]--;
					return i;
				}
			}
			return 0;
		}
		int Deck::size() {
			int sum = 0;
			for (int i : content)
			{
				sum += i;
			}
			return sum;
		}


		Hand::Hand(string name, vector<int> input) {
			this->name = name;
			hand = input;
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
		DealerHand::DealerHand(string name, vector<int> input) :Hand(name,input){
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
		void DealerHand::print()
		{
			if (hand.size() == 1) {
				cout << name << " up card is " << Translate(hand[0]) << endl;
			}
			else Hand::print();
		}
		void Hand::add(int i) {
			hand.push_back(i);
		}

		void PlayerHand::hit(Deck* deck)
		{
			for (;;) {
				add(deck->DrowRandom());
				print();
				if (std::get<0>(CheckHand()) >= 21 || AskOption(false, false) == Option::Stand) break;
			}
		}
		PlayerHand::PlayerHand(string name, vector<int> input) :Hand(name, input) {
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
		bool PlayerHand::splittable() {
			if (splitted == false && hand.size() == 2 && (hand[0] == hand[1] || hand[0] >= 9 && hand[1] >= 9))return true;
			else return false;
		}

		void PlayerHand::judge(DealerHand dealer)
		{
			if (result == Result::undefined) {
				result = Judge(*this, dealer);
				cout << name << " Result:" << ResulttoString.at(get_result()) << endl;
			}
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

		string Translate(int input) {
			switch (input) {
			case 0:
				return "A";
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
		const map< Result, string> PlayerHand::ResulttoString = {
			{Result::Win		,"Win"},
			{Result::Lose		,"Lose"},
			{Result::Tie		,"Tie"},
			{Result::BlackJack	,"BlackJack"},
			{Result::Surrender	,"Surrender"},
			{Result::DoubledWin	,"DoubledWin"},
			{Result::DoubledLose,"DoubledLose"}
		};

		Player::Player(int ID, string name)
		{
			this->name = name;
			this->id = ID;
			hand.first = PlayerHand(name + "'s hand");
		}

		bool Player::issplitted()
		{
			return hand.first.get_splitted();
		}

		void Player::play(Deck* deck, Rule rule)
		{
			if (hand.first.play(deck, rule, true) == Option::Split) {
				hand.second = hand.first.split(deck);
				hand.first.play(deck, rule, false);
				hand.second.play(deck, rule, false);
			}

		}
		Rule::Rule(bool Soft17Hit, bool Surrender, bool DoubleAfterSplit)
		{
			list.emplace("Soft17Hit", Soft17Hit);
			list.emplace("Surrender", Surrender);
			list.emplace("DoubleAfterSplit", DoubleAfterSplit);
		}
	}
}