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
			inline std::unordered_map<Option, double> PlayerEV_all(PlayerHand const& player);
			double PlayerEV(Deck const& deck, PlayerHand const& player);
			double PlayerEV_afterP(Deck const& deck, PlayerHand const& player);
			std::valarray<double> DealerEV(Deck const& deck, DealerHand const& dealer); //bust,17,18,19,20,21,BJの確率を返す
			inline double If_stand(Deck const& deck, PlayerHand const& player);
			inline double If_hit(Deck const& deck, PlayerHand const& player);
			inline double If_double(Deck const& deck, PlayerHand const& player);
			inline double If_split(Deck const& deck, PlayerHand const& player);
		public:
			Calculator(Deck deck, Rule rule = Rule(), Rate rate = Rate(),DealerHand dealer = DealerHand());
			double calculate();
			double calculate(cfstream& cfout);
			std::pair<Option, double> WhattoDo(PlayerHand const& playerhand);
			std::string PrintStatus();
		};

	}
}