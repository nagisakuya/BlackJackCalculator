#pragma once
#include "BlackJack.h"
#include <valarray>
#include <fstream>
#include <ctime>


namespace nagisakuya {
	namespace BlackJack {
		/*class Strategy {
			static enum class ex_Option { Hit, Stand, DoubleDown, Split, Surrender, DoubleOtherwiseStand, SplitifDoubleafterSplitenable };
		};*/
		class Calculator :public Table {
		private:
		public:
			double Calculate_Expection(std::ofstream& file);
			//(多分)速度に影響出るので似た関数だけど別で宣言
			std::pair<Option, double> WhattoDo(Deck const& deck, PlayerHand const& playerhand, DealerHand const& dealerhand, std::ofstream& file);
			double PlayerExpection(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
			std::valarray<double> DealerExpection(Deck const& deck, DealerHand const& dealer); //bust,17,18,19,20,21,BJの確率
			Calculator(int NumberofDeck = 8, Rule rule = Rule(), Rate rate = Rate());
		};
	}
}