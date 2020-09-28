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
		enum class RuleList { Soft17Hit, Surrender, DoubleAfterSplit };
		class Rule;
		class Rule :public std::map<RuleList, bool> {
		public:
			Rule(bool Soft17Hit = false, bool Surrender = false, bool DoubleAfterSplit = false);
		};
		class Deck {
		private:
			int content[14];
			int first_size;
		public:
			Deck(int NumberofDeck = 1, int Joker = 2);
			void print() const;
			int size() const;
			int DrowRandom();
		};
		class Hand {
		protected:
			std::vector<int> hand;
			std::string name;
		public:
			Hand(std::string name = "hand", std::vector<int> input = {});
			void add(int input);
			virtual void print() const;
			size_t size() const;
			std::tuple<int, bool, bool> CheckHand() const;
		};
		class DealerHand :public Hand {
		public:
			DealerHand(std::string name = "Dealer hand", std::vector<int> input = {});
			void print() const;
			void hituntil17(Deck& deck, Rule const& rule);
		};
		class PlayerHand :public Hand {
		protected:
			Result result = Result::undefined;
			bool splitted = false;
			bool doubled = false;
			const static std::map< Result, std::string> ResulttoString;
			static Option AskOption(bool Split_enable = false, bool DoubleDown_enable = false, bool Surrender_enable = false);
		public:
			PlayerHand(std::string name = "Player's hand", std::vector<int> input = {}, bool splitted = false);
			bool splittable() const;
			PlayerHand split(Deck* deck);
			Option play(Deck* deck, Rule const& rule);
			void judge(DealerHand const& dealer);
			bool get_splitted() const { return splitted; }
			bool get_doubled() const { return doubled; }
			Result get_result() const { return result; }
		};
		class Player {
		protected:
			std::string name;
			int id;
		public:
			std::pair<PlayerHand, PlayerHand> hand;
			Player(int ID, std::string name = "Player");
			void play(Deck* deck, Rule const& rule);
			int get_ID() const { return id; }
			std::string get_name() const { return name; }
		};
		class Table {
		protected:
			Deck deck;
			DealerHand dealer;
			std::vector<Player> PlayerList;
			Rule rule;
			std::map<Result, double>Rate;
		public:
			Table(int Numberofdeck = 8, Rule rule = Rule(), double BlackJackRate = 2.5);
			bool addplayer(Player input);
			void play();
		};
		Result Judge(PlayerHand const& playerhand, DealerHand const& dealer);
		std::string Translate(int input);
	}

}