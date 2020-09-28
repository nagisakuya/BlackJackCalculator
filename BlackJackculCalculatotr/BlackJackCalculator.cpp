#include "BlackJackCalculator.h"
using namespace std;

namespace nagisakuya {
	namespace BlackJack {
		Calculator::Calculator(int NumberofDeck, Rule rule, double BJRate):Table(NumberofDeck,rule, BJRate){}
		/*std::pair<Option, double> Calculator::WhattoDo(Deck const& deck, PlayerHand const& player, DealerHand const& dealer)
		{
			if (get<0>(player.CheckHand()) == 21) return make_pair(Option::Stand, Rate.at(Result::BlackJack));
			bool IsTheFirst = (player.size() == 2);
			pair<Option, double> best;

			return std::pair<Option, double>();
		}*/
		std::valarray<double> Calculator::DealerExpection(Deck const& deck, Hand const& dealer)
		{
			int temp;
			bool BJ, soft;
			tie(temp, soft, BJ) = dealer.CheckHand();
			valarray<double> r;
			if (BJ) {
				r[6]++;
				return r;
			}
			if (21>= temp &&temp >= 17) {
				r[temp - 17]++;
				return r;
			}
			if ( temp > 21) {
				r[5]++;
				return r;
			}

			for (size_t i = 0; i < 10; i++)
			{
				if (deck.count(i) != 0) {
					r += deck.count(i) * DealerExpection(deck - i, dealer + i);
				}
			}
			r = r / deck.size();
			return r;
		}
	}
}