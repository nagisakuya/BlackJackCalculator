#pragma once
#include "BlackJack.h"
#include <valarray>
#include <fstream>
#include <ctime>


namespace nagisakuya {
	namespace BlackJack {
		class Calculator :public Table {
		private:
			double PlayerEV(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
			double If_stand(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
			double If_hit(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
			double If_double(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
			double If_split(Deck const& deck, PlayerHand const& player, DealerHand const& dealer);
		public:
			Calculator(Deck deck, Rule rule = Rule(), Rate rate = Rate());
			//EV = Expected Value
			double Calculate(std::ofstream& file);
			std::pair<Option, double> WhattoDo(Deck const& deck, PlayerHand const& playerhand, DealerHand const& dealerhand, std::ofstream &file);
			std::valarray<double> DealerEV(Deck const& deck, DealerHand const& dealer); //bust,17,18,19,20,21,BJ‚ÌŠm—¦
		};
	}
}