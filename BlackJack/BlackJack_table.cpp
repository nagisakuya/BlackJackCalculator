#include "BlackJack.h"

using namespace std;

namespace nagisakuya {
	namespace BlackJack {
		Table::Table(int Numberofdeck, Rule rule, double BlackJackRate) {
			deck = Deck(Numberofdeck, 0);
			this->rule = rule;
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
			//関数ポインタでplayereach関数をを作ろうと思ったが、内部で1関数以上処理したりするししなくていいか？
			for (Player& i : PlayerList)
			{
				i.hand.first = PlayerHand(i.get_name() + " hand", { deck.DrowRandom() ,deck.DrowRandom() });
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
	}
}