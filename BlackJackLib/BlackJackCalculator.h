#pragma once
#include "pch.h"
#include "BlackJack.h"


namespace nagisakuya {
	namespace BlackJack {
		class Calculator :public Table {
		private:
			inline std::unordered_map<Option, double> PlayerEV_all(PlayerHand const&);
			double PlayerEV(Deck const&, PlayerHand const&);
			double PlayerEV_afterP(Deck const&, PlayerHand const&);
			std::valarray<double> DealerEV(Deck const&, DealerHand const&); //bust,17,18,19,20,21,BJ‚ÌŠm—¦‚ð•Ô‚·
			inline double If_stand(Deck const&, PlayerHand const&);
			inline double If_hit(Deck const&, PlayerHand const&);
			inline double If_double(Deck const&, PlayerHand const&);
			inline double If_split(Deck const&, PlayerHand const&);
			double If_onstrategy(Deck const&, PlayerHand const&, Strategy = Strategy());
		public:
			Calculator() {};
			Calculator(Deck, Rule = Rule(), Rate = Rate(), DealerHand = DealerHand());
			double calculate(std::function<void()> do_before = []{}, std::function<double(Calculator, PlayerHand)> do_while = [](Calculator calculator, PlayerHand hand) -> double {return calculator.WhattoDo(hand).second; }, std::function<void(double,double)> do_after = [](double d,double c) {});
			double calculate_withoutput(Utility::cfstream&);
			double calculate_onstrategy(Utility::cfstream&, Strategy = Strategy());
			std::pair<Option, double> WhattoDo(PlayerHand const& playerhand);
		};

	}
}