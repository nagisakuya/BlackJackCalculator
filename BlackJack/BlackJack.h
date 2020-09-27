#pragma once
#include <tuple>
#include <map>
#include <string>
#include <vector>
#include <iostream>


namespace nagisakuya {
	namespace BlackJack {
		enum class Result { Win, Lose, Tie, BlackJack, Surrender, DoubledWin, DoubledLose, undefined };
		enum class Option { Hit, Stand, DoubleDown, Split, Surrender };
		class Rule : std::map<std::string, bool> {
		public:
			Rule(bool Soft17Hit = false, bool Surrender = false, bool DoubleAfterSplit = false);
			bool get(std::string input) { return at(input); }
		};
		class Deck {
		private:
			int content[14];
			int first_size;
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
			void add(int input);
			virtual void print();
			std::tuple<int, bool, bool> CheckHand();
		};
		class DealerHand :public Hand{
		public:
			DealerHand(std::string name = "Dealer hand", std::vector<int> input = {});
			void print();
			void hituntil17(Deck* deck, Rule rule);
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
			bool splittable();
			PlayerHand split(Deck* deck);
			Option play(Deck* deck, Rule rule, bool IsTheFirst);
			void judge(DealerHand dealer);
			bool get_splitted() { return splitted; }
			bool get_doubled() { return doubled; }
			Result get_result() { return result; }
		};
		class Player {
		protected:
			std::string name;
			int id;
		public:
			std::pair<PlayerHand, PlayerHand> hand;
			Player(int ID, std::string name = "Player");
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
			bool addplayer(Player input);
			void play();
		};
		Result Judge(PlayerHand player, DealerHand dealer);
		std::string Translate(int input);
	}

}