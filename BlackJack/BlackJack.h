#pragma once
#include <tuple>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <array>


namespace nagisakuya {
	namespace BlackJack {
		enum class Result { Win, Lose, Tie, BlackJack, Surrender, DoubledWin, DoubledLose, undefined };
		enum class Option { Hit, Stand, DoubleDown, Split, Surrender, Best };
		const std::map<Option, std::string> OptiontoString{
			{Option::Hit	,"Hit   "},
			{Option::Stand	,"Stand "},
			{Option::DoubleDown,"Double"},
			{Option::Split,"Split "},
			{Option::Surrender,"Surrender"}
		};
		enum class RuleList { Soft17Hit, Surrender, DoubleAfterSplit };
		class Rule :public std::map<RuleList, bool> {
		public:
			Rule(bool Soft17Hit = false, bool Surrender = false, bool DoubleAfterSplit = false);
		};
		class Deck {
		protected:
			std::array<int, 10> content;
		public:
			Deck(int NumberofDeck = 8);
			Deck(std::array<int, 10> input);
			void print() const;
			int size() const;
			int count(int i) const{ return content[i]; }
			int DrowRandom();
			Deck operator - (int i) const { std::array<int, 10> r = content; r[i]--; return Deck(r); }
		};
		class Hand {
		protected:
			std::vector<int> content;
		public:
			Hand(std::vector<int> input = {});
			void add(int input);
			virtual void print() const;
			size_t size() const;
			std::tuple<int, bool, bool> CheckHand() const;//sum,issoft,isBJ
			std::pair<int, bool> CheckHand_speed() const;//sum,isBJ
		};
		class DealerHand :public Hand {
		public:
			DealerHand(std::vector<int> input = {});
			void print() const;
			void hituntil17(Deck& deck, Rule const& rule);
			int get_upcard() const { return content[0]; }
			DealerHand operator + (int i) const { std::vector<int> r = content; r.emplace_back(i); return DealerHand(r); }
		};
		class PlayerHand :public Hand {
		private:
			Result result = Result::undefined;
			bool splitted = false;
			bool doubled = false;
			const static std::map< Result, std::string> ResulttoString;
			static Option AskOption(bool Split_enable = false, bool DoubleDown_enable = false, bool Surrender_enable = false);
		public:
			PlayerHand(std::vector<int> input = {}, bool splitted = false, bool doubled = false);
			bool splittable() const;
			PlayerHand split(Deck* deck);
			Option play(Deck* deck, Rule const& rule);
			void judge(DealerHand const& dealer);
			bool get_splitted() const { return splitted; }
			bool get_doubled() const { return doubled; }
			Result get_result() const { return result; }
			PlayerHand operator + (int i) const { std::vector<int> r = content; r.emplace_back(i); return PlayerHand(r, splitted, doubled); }//i‚ðˆø‚¢‚½Œ‹‰Ê‚ð•Ô‚·
			PlayerHand operator * (int i) const { std::vector<int> r = content; r.emplace_back(i); return PlayerHand(r, splitted, true); }//Double‚µ‚Äi‚ðˆø‚¢‚½Œ‹‰Ê‚ð•Ô‚·
			PlayerHand operator / (int i) const { std::vector<int> r = content; r.emplace_back(i); return PlayerHand(r, true, false); }//Split‚µ‚½‚ ‚Æi‚ðˆø‚¢‚½Œ‹‰Ê‚ð•Ô‚·
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
		private:
			std::vector<Player> PlayerList;
			DealerHand dealer;
			Deck deck;
		protected:
			Rule rule;
			std::map<Result, double>Rate;
		public:
			Table(int Numberofdeck = 8, Rule rule = Rule(), double BlackJackRate = 2.5);
			bool addplayer(Player input);
			void play();
			void replay();
		};
		Result Judge(PlayerHand const& playerhand, DealerHand const& dealer);
		std::string Translate(int input);
	}

}