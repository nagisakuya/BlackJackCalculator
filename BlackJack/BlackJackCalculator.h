#pragma once
#include "BlackJack.h"
#include <valarray>
#include <fstream>



namespace nagisakuya {
	namespace BlackJack {
		/*class Strategy {
			static enum class ex_Option { Hit, Stand, DoubleDown, Split, Surrender, DoubleOtherwiseStand, SplitifDoubleafterSplitenable };
		};*/
		class Calculator :public Table {
		private:
		public:
			double Calculate_Expection(Deck const& deck);
			//(多分)速度に影響出るので似た関数だけど別で宣言
			std::pair<Option, double> WhattoDo(Deck const& deck, PlayerHand const& playerhand, DealerHand const& dealerhand);
			std::pair<Option, double> WhattoDo_writedown(Deck const& deck, PlayerHand const& playerhand, DealerHand const& dealerhand, std::ofstream & file);
			double WhattoDo_speed(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
			std::valarray<double> DealerExpection(Deck const& deck, DealerHand const& dealer); //bust,17,18,19,20,21,BJの確率
			Calculator(int NumberofDeck = 8);
		};
	}
}