#pragma once
#include <tuple>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>


namespace nagisakuya {
	namespace BlackJack {
		enum class Result { Win, Lose, Tie, BlackJack, Surrender, DoubledWin, DoubledLose, undefined };
		enum class Option { Hit, Stand, DoubleDown, Split, Surrender };
		class Rule {
			std::map<std::string, bool> list;
		public:
			Rule(bool Soft17Hit = false, bool Surrender = false, bool DoubleAfterSplit = false);
			bool get(std::string input) { return list.at(input); }
		};
		class Deck {
		private:
			int content[14];
		public:
			Deck(int NumberofDeck = 1, int Joker = 2);
			void print();
			int DrowRandom();
			int size();
		};
		class Hand {
		protected:
			std::vector<int> hand;
			std::string name;
		public:
			Hand(std::string name = "hand", std::vector<int> input = {});
			void add(int i);
			virtual void print();
			std::tuple<int, bool, bool> CheckHand();
		};
		class DealerHand :public Hand{
		public:
			DealerHand(std::string name = "Dealer hand", std::vector<int> input = {});
			void hituntil17(Deck* deck, Rule rule);
			void print();
		};
		class PlayerHand :public Hand {
		protected:
			Result result = Result::undefined;
			bool splitted = false;
			bool doubled = false;
			const static std::map< Result, std::string> ResulttoString;
			static Option AskOption(bool Split_enable = false, bool DoubleDown_enable = false, bool Surrender_enable = false);
		public:
			PlayerHand(std::string name = "Player's hand", std::vector<int> input = {});
			void hit(Deck* deck);
			PlayerHand split(Deck* deck);
			bool splittable();
			void judge(DealerHand dealer);
			Option play(Deck* deck, Rule rule, bool IsTheFirst);
			Result get_result() { return result; }
			bool get_splitted() { return splitted; }
			bool get_doubled() { return doubled; }
			std::string get_name() { return name; }
		};
		class Player {
		protected:
			std::string name;
			int id;
		public:
			std::pair<PlayerHand, PlayerHand> hand;
			Player(int ID, std::string name = "Player");
			bool issplitted();
			void play(Deck* deck,Rule rule);
			int get_ID() { return id; }
			std::string get_name() { return name; }
		};
		class Table {
		protected:
			Deck deck;
			DealerHand dealer;
			std::vector< Player> PlayerList;
			Rule rule;
			std::map<Result, double>Rate;
		public:
			Table(int Numberofdeck = 8, Rule rule = Rule() ,double BlackJackRate = 2.5);
			void Test();
			bool addplayer(Player input);
			void play();
		};
		Result Judge(PlayerHand player, DealerHand dealer);
		std::string Translate(int input);
	}

}