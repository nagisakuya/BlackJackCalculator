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
			//(����)���x�ɉe���o��̂Ŏ����֐������ǕʂŐ錾
			std::pair<Option, double> WhattoDo(Deck const& deck, PlayerHand const& playerhand, DealerHand const& dealerhand, std::ofstream& file);
			double PlayerExpection(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
			std::valarray<double> DealerExpection(Deck const& deck, DealerHand const& dealer); //bust,17,18,19,20,21,BJ�̊m��
			Calculator(Deck deck, Rule rule = Rule(), Rate rate = Rate());
		};
	}
}