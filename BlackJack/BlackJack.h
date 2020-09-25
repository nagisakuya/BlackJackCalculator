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
			void print();
			std::tuple<int, bool, bool> CheckHand();
			void hituntil17(Deck* deck, bool Soft17Hit = false);
		};
		class PlayerHand :public Hand {
		protected:
			Result result = Result::undefined;
			bool splitted = false;
			bool doubled = false;
		public:
			PlayerHand(std::string name = "Player's hand", std::vector<int> input = {});
			void hit(Deck* deck);
			PlayerHand split();
			bool splittable();
			void set_result(Result r) { result = r; }
			Result get_result() { return result; }
			bool get_splitted() { return splitted; }
			bool get_doubled() { return doubled; }
			void set_doubled(bool i) { doubled = i; }
			std::string get_name() { return name; }
		};
		class Player {
		protected:
			std::string name;
			int ID;
		public:
			PlayerHand firsthand, secondhand;
			Player(int ID, std::string name = "Player");
			bool issplitted();
			void act(Deck* deck);
			void begin(int first, int second);
			int get_ID() { return ID; }
			std::string get_name() { return name; }
		};
		class Table {
		protected:
			Deck deck;
			Hand dealer;
			std::map<int, Player> PlayerList;
			bool Soft17Hit = false;
			bool SurrenderFlag = false;
			bool DoubleAfterSplit = false;
			std::map<Result, double>Rate;
			const static std::map< Result, std::string> ResulttoString;
		public:
			void addplayer(Player input);
			static Result Judge(PlayerHand player, Hand dealer);
			Table(int Numberofdeck = 8, double BlackJackrate = 2.5);
			void Play();
		};
		std::string Translate(int input);
		Option AskOption(bool Split_enable = false, bool DoubleDown_enable = false, bool Surrender_enable = false);
	}
	
}