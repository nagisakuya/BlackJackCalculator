#pragma once
#include "BlackJack.h"
#include "Utility.h"
#include <valarray>
#include <fstream>
#include <ctime>


namespace nagisakuya {
	namespace BlackJack {
		class Calculator :public Table {
		private:
			double PlayerEV(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
			double PlayerEV_afterP(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
			std::valarray<double> DealerEV(Deck const& deck, DealerHand const& dealer); //bust,17,18,19,20,21,BJの確率を返す
			inline double If_stand(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
			inline double If_hit(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
			inline double If_double(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
			inline double If_split(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
		public:
			Calculator(Deck deck, Rule rule = Rule(), Rate rate = Rate());
			double calculate();
			double calculate(cfstream& cfout);
			std::pair<Option, double> WhattoDo(Deck const& deck, PlayerHand const& playerhand, DealerHand const& dealerhand);
			std::pair<Option, double> WhattoDo(Deck const& deck, PlayerHand const& playerhand, DealerHand const& dealerhand, cfstream& cfout);
			std::string PrintStatus();
		};

	}
}