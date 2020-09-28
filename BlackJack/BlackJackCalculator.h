#pragma once
#include "../BlackJack/BlackJack.h"
#include <valarray>



namespace nagisakuya {
	namespace BlackJack {
		/*class Strategy {
			static enum class ex_Option { Hit, Stand, DoubleDown, Split, Surrender, DoubleOtherwiseStand, SplitifDoubleafterSplitenable };
		};*/
		class Calculator :public Table {
		private:
		public:
			double Calc_Expection();
			std::pair<Option, double> WhattoDo(Deck const& deck, PlayerHand const& playerhand, DealerHand const& dealerhand);
			std::valarray<double> DealerExpection(Deck const& deck, DealerHand const& dealer); //bust,17,18,19,20,21,BJ‚ÌŠm—¦
			Calculator(int NumberofDeck = 8);
		};
	}
}