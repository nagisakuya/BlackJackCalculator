#include "pch.h"
#include "BlackJack.h"

using namespace std;

namespace nagisakuya {
	namespace BlackJack {
		Table::Table(Deck deck,Rule rule,Rate rate, DealerHand dealer) : rule(rule),rate(rate)
		{
			this->deck = deck;
			this->dealer = dealer;
		}

		void Table::play() {
			if (PlayerList.size() == 0) {
				cout << "there is no player" << endl;
				return;
			}
			DealerHand dealer = DealerHand({ deck.DrowRandom() });
			dealer.print();
			//関数ポインタでplayereach関数をを作ろうと思ったが、内部で1関数以上処理したりするししなくていいか？
			for (Player& i : PlayerList)
			{
				i.hand.first = PlayerHand({ deck.DrowRandom() ,deck.DrowRandom() });
			}
			for (Player& i : PlayerList)
			{
				i.play(&deck, rule);
			}
			dealer.hituntil17(deck, rule);
			for (Player& i : PlayerList)
			{
				i.hand.first.judge(dealer);
				if (i.hand.first.get_splitted() == true)i.hand.second.judge(dealer);
			}
		}

		void Table::replay()
		{
			for (size_t i = 0; ; i++)
			{
				string temp_s;
				play();
				cout << "Press c to continue. press e to exit." << endl;
				cin >> temp_s;
				if (temp_s == "e") break;
			}
		}
		string Table::PrintStatus()
		{
			return rule.print() + "\n" + rate.print() + "\n" + deck.print() + "\n";
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
			PlayerList.emplace_back(input);
			return true;
		}
	}
}