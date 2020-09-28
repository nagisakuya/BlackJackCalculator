#pragma once
#include "../BlackJack/BlackJack.h"
#include <valarray>


namespace nagisakuya {
	namespace BlackJack {
		/*class Strategy {
			static enum class ex_Option { Hit, Stand, DoubleDown, Split, Surrender, DoubleOtherwiseStand, SplitifDoubleafterSplitenable };
		};*/
		class Calculator :public Table {
		public:
			//double Expected_value(Deck const&);
			//std::pair<Option, double> WhattoDo(Deck const& deck, PlayerHand const& playerhand, DealerHand const& dealerhand);
			std::valarray<double> DealerExpection(Deck const& deck, Hand const& dealer); //17,18,19,20,21,bust,BJ‚ÌŠm—¦
		public:
			Calculator(int NumberofDeck = 8, Rule rule = Rule(), double BJRate = 2.5);
		};
	}
}