#include "pch.h"
#include "BlackJack.h"

using namespace std;

namespace nagisakuya {
	namespace BlackJack {
		Strategy::Splittable::Splittable()
		{
			list = { {
				{Option::Split,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Split,Option::Stand,Option::Stand,},
				{Option::Split,Option::Splithit,Option::Splithit,Option::Hit,Option::Double,Option::Splithit,Option::Split,Option::Split,Option::Split,Option::Stand,},
				{Option::Split,Option::Splithit,Option::Splithit,Option::Hit,Option::Double,Option::Split,Option::Split,Option::Split,Option::Split,Option::Stand,},
				{Option::Split,Option::Split,Option::Split,Option::Hit,Option::Double,Option::Split,Option::Split,Option::Split,Option::Split,Option::Stand,},
				{Option::Split,Option::Split,Option::Split,Option::Splithit,Option::Double,Option::Split,Option::Split,Option::Split,Option::Split,Option::Stand,},
				{Option::Split,Option::Split,Option::Split,Option::Splithit,Option::Double,Option::Split,Option::Split,Option::Split,Option::Split,Option::Stand,},
				{Option::Split,Option::Split,Option::Split,Option::Hit,Option::Double,Option::Hit,Option::Split,Option::Split,Option::Stand,Option::Stand,},
				{Option::Split,Option::Hit,Option::Hit,Option::Hit,Option::Double,Option::Hit,Option::Hit,Option::Split,Option::Split,Option::Stand,},
				{Option::Split,Option::Hit,Option::Hit,Option::Hit,Option::Double,Option::Hit,Option::Hit,Option::Split,Option::Split,Option::Stand,},
				{Option::Split,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Split,Option::Stand,Option::Stand,},
			} };
		}
		void Strategy::Splittable::import(std::istream& input)
		{
			string temp_str;
			for (size_t i = 0; i < 10; i++)
			{
				for (size_t j = 0; j < 10; j++)
				{
					input >> temp_str;
					list[j][i] = OptionandString.at_reverse(temp_str);
				}
			}
		}
		std::string Strategy::Splittable::print()
		{
			stringstream ss;
			ss << "\t";
			for (size_t i = 0; i < 10; i++)
			{
				ss << Translate(i) << "\t";
			}
			ss << endl;
			for (size_t i = 0; i < 10; i++)
			{
				ss << Translate(i) << "," << Translate(i) << ":\t";
				for (size_t j = 0; j < 10; j++)
				{
					ss << OptionandString.at(list[j][i]) << "\t";
				}
				ss << endl;
			}
			ss << endl;
			return ss.str();
		}
		std::string Strategy::Splittable::print_initializer_list()
		{
			stringstream ss;
			ss << "{";
			for (size_t i = 0; i < 10; i++)
			{
				ss << "{";
				for (size_t j = 0; j < 10; j++)
				{
					ss << OptionandOption.at(list[i][j]) << ",";
				}
				ss << "}," << endl;
			}
			ss << "}" << endl;
			return ss.str();
		}
		Strategy::Soft::Soft()
		{
			list = { {
				{Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Stand,Option::Stand,Option::Stand},
				{Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Doublestand,Option::Stand,Option::Stand,Option::Stand},
				{Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Double,Option::Doublestand,Option::Stand,Option::Stand,Option::Stand},
				{Option::Hit,Option::Hit,Option::Double,Option::Double,Option::Double,Option::Doublestand,Option::Stand,Option::Stand,Option::Stand},
				{Option::Double,Option::Double,Option::Double,Option::Double,Option::Double,Option::Doublestand,Option::Stand,Option::Stand,Option::Stand},
				{Option::Double,Option::Double,Option::Double,Option::Double,Option::Double,Option::Doublestand,Option::Doublestand,Option::Stand,Option::Stand},
				{Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Stand,Option::Stand,Option::Stand,Option::Stand},
				{Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Stand,Option::Stand,Option::Stand,Option::Stand},
				{Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Stand,Option::Stand,Option::Stand},
				{Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Stand,Option::Stand,Option::Stand},
			} };
		}
		void Strategy::Soft::import(std::istream& input)
		{
			string temp_str;
			for (size_t i = 0; i < 9; i++)
			{
				for (size_t j = 0; j < 10; j++)
				{
					input >> temp_str;
					list[j][i] = OptionandString.at_reverse(temp_str);
				}
			}
		}
		std::string Strategy::Soft::print()
		{
			stringstream ss;
			ss << "\t";
			for (size_t i = 0; i < 10; i++)
			{
				ss << Translate(i) << "\t";
			}
			ss << endl;
			for (size_t i = 0; i < 9; i++)
			{
				ss << "A," << Translate(i + 1) << ":\t";
				for (size_t j = 0; j < 10; j++)
				{
					ss << OptionandString.at(list[j][i]) << "\t";
				}
				ss << endl;
			}
			ss << endl;
			return ss.str();
		}
		std::string Strategy::Soft::print_initializer_list()
		{
			stringstream ss;
			ss << "{";
			for (size_t i = 0; i < 10; i++)
			{
				ss << "{";
				for (size_t j = 0; j < 9; j++)
				{
					ss << OptionandOption.at(list[i][j]) << ",";
				}
				ss << "}," << endl;
			}
			ss << "}" << endl;
			return ss.str();
		}
		Strategy::Hard::Hard()
		{
			list = { {
				{Option::Hit,Option::Hit,Option::Hit,Option::Double,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Surrenderhit,Option::Stand,},
				{Option::Hit,Option::Hit,Option::Double,Option::Double,Option::Hit,Option::Stand,Option::Stand,Option::Stand,Option::Stand,Option::Stand,},
				{Option::Hit,Option::Double,Option::Double,Option::Double,Option::Hit,Option::Stand,Option::Stand,Option::Stand,Option::Stand,Option::Stand,},
				{Option::Hit,Option::Double,Option::Double,Option::Double,Option::Stand,Option::Stand,Option::Stand,Option::Stand,Option::Stand,Option::Stand,},
				{Option::Hit,Option::Double,Option::Double,Option::Double,Option::Stand,Option::Stand,Option::Stand,Option::Stand,Option::Stand,Option::Stand,},
				{Option::Hit,Option::Double,Option::Double,Option::Double,Option::Stand,Option::Stand,Option::Stand,Option::Stand,Option::Stand,Option::Stand,},
				{Option::Hit,Option::Hit,Option::Double,Option::Double,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Stand,},
				{Option::Hit,Option::Hit,Option::Double,Option::Double,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Stand,},
				{Option::Hit,Option::Hit,Option::Double,Option::Double,Option::Hit,Option::Hit,Option::Hit,Option::Hit,Option::Surrenderhit,Option::Stand,},
				{Option::Hit,Option::Hit,Option::Hit,Option::Double,Option::Hit,Option::Hit,Option::Hit,Option::Surrenderhit,Option::Surrenderhit,Option::Stand,},
			} };
		}
		void Strategy::Hard::import(std::istream& input)
		{
			string temp_str;
			for (size_t i = 0; i < 10; i++)
			{
				for (size_t j = 0; j < 10; j++)
				{
					input >> temp_str;
					list[j][i] = OptionandString.at_reverse(temp_str);
				}
			}
		}
		std::string Strategy::Hard::print()
		{
			stringstream ss;
			ss << "\t";
			for (size_t i = 0; i < 10; i++)
			{
				ss << Translate(i) << "\t";
			}
			ss << endl;
			for (size_t i = 0; i < 10; i++)
			{
				ss << i + 8 << ":\t";
				for (size_t j = 0; j < 10; j++)
				{
					ss << OptionandString.at(list[j][i]) << "\t";
				}
				ss << endl;
			}
			ss << endl;
			return ss.str();
		}
		std::string Strategy::Hard::print_initializer_list()
		{
			stringstream ss;
			ss << "{";
			for (size_t i = 0; i < 10; i++)
			{
				ss << "{";
				for (size_t j = 0; j < 10; j++)
				{
					ss << OptionandOption.at(list[i][j]) << ",";
				}
				ss << "}," << endl;
			}
			ss << "}" << endl;
			return ss.str();
		}
		void Strategy::import(std::istream& input)
		{
			splittable.import(input);
			soft.import(input);
			hard.import(input);
		}
		std::string Strategy::print()
		{
			stringstream ss;
			ss << splittable.print();
			ss << soft.print();
			ss << hard.print();
			return ss.str();
		}
		BlackJack::Option Strategy::find(DealerHand const& dealer, PlayerHand const& player, Rule const& rule) const
		{
			auto OptiontoOption = [=](Strategy::Option input) {
				if (input == Strategy::Option::Hit) {
					return BlackJack::Option::Hit;
				}
				else if (input == Strategy::Option::Stand) {
					return BlackJack::Option::Stand;
				}
				else if (input == Strategy::Option::Double) {
					if (player.doubleble(rule.at(Rule::List::DoubleAfterSplit)) == true) return BlackJack::Option::Double;
					else return BlackJack::Option::Hit;
				}
				else if (input == Strategy::Option::Doublestand) {
					if (player.doubleble(rule.at(Rule::List::DoubleAfterSplit)) == true) return BlackJack::Option::Double;
					else return BlackJack::Option::Stand;
				}
				else if (input == Strategy::Option::Split) {
					if (player.splittable() == true) return BlackJack::Option::Split;
					else return BlackJack::Option::Stand;
				}
				else if (input == Strategy::Option::Splithit) {
					if (player.splittable() == true) return BlackJack::Option::Split;
					else return BlackJack::Option::Hit;
				}
				else if (input == Strategy::Option::Surrenderhit) {
					if (rule.at(Rule::List::Surrender) == true) { return BlackJack::Option::Surrender; }
					else { return BlackJack::Option::Hit; }
				}
				return BlackJack::Option();
			};
			auto temp_tuple = player.CheckHand();
			if (player.splittable() == true) {
				return OptiontoOption(splittable.get(dealer.get_upcard(), player.get()[0]));
			}
			if (get<1>(temp_tuple) == true) {
				return OptiontoOption(soft.get(dealer.get_upcard(), get<0>(temp_tuple)));
			}
			return OptiontoOption(hard.get(dealer.get_upcard(), get<0>(temp_tuple)));
		}
	}
}