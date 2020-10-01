#pragma once
#include <tuple>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <array>


namespace nagisakuya {
	namespace BlackJack {
		enum class Result { Win, Lose, Tie, BlackJack, Surrender, DoubledWin, DoubledLose, undefined };
		enum class Option { Hit, Stand, Double, Split, Surrender, Best };
		const std::unordered_map<Option, std::string> OptiontoString{
			{Option::Hit	,"Hit   "},
			{Option::Stand	,"Stand "},
			{Option::Double,"Double"},
			{Option::Split,"Split "},
			{Option::Surrender,"Surrender"}
		};
		enum class RuleList { Soft17Hit, Surrender, DoubleAfterSplit };
		class Rule :public std::unordered_map<RuleList, bool> {
		public:
			Rule(bool Soft17Hit = false, bool Surrender = false, bool DoubleAfterSplit = false);
			std::string print() const;
		};
		class Rate :public std::unordered_map<Result, double> {
		public:
			Rate(double BlackJackRate = 1.5);
			std::string print() const;
		};
		class Deck {
		protected:
			std::array<int, 10> content;
		public:
			Deck(int NumberofDeck = 8);
			Deck(std::array<int, 10> input);
			std::string print() const;
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
			virtual std::tuple<int, bool, bool> CheckHand() const;//sum,issoft,isBJ
			std::pair<int, bool> CheckHand_speed() const;//sum,isBJ
		};
		class DealerHand :public Hand {
		public:
			DealerHand(std::vector<int> input = {});
			void print() const;
			void hituntil17(Deck& deck, Rule const& rule);
			int get_upcard() const { return content[0]; }
			DealerHand operator + (int i) const { DealerHand r(content); r.add(i); return r; }
		};
		class PlayerHand :public Hand {
		private:
			Result result = Result::undefined;
			bool splitted = false;
			bool doubled = false;
			const static std::unordered_map< Result, std::string> ResulttoString;
			static Option AskOption(bool Split_enable = false, bool DoubleDown_enable = false, bool Surrender_enable = false);
		public:
			PlayerHand(std::vector<int> input = {}, bool splitted = false, bool doubled = false);
			bool splittable() const;
			PlayerHand split(Deck* deck);
			Option play(Deck* deck, Rule const& rule);
			void judge(DealerHand const& dealer);
			std::tuple<int, bool, bool> CheckHand() const;
			bool get_splitted() const { return splitted; }
			bool get_doubled() const { return doubled; }
			Result get_result() const { return result; }
			PlayerHand operator + (int i) const { PlayerHand r(content, splitted, doubled); r.add(i); return r; }//i‚ð‰Á‚¦‚½Œ‹‰Ê‚ð•Ô‚·
			PlayerHand operator * (int i) const { PlayerHand r(content, splitted, true); r.add(i); return r; }//Double‚µ‚Äi‚ð‰Á‚¦‚½Œ‹‰Ê‚ð•Ô‚·
			PlayerHand operator / (int i) const { PlayerHand r({ content[0] }, true, false); r.add(i); return r; }//Split‚µ‚½‚ ‚Æi‚ð‰Á‚¦‚½Œ‹‰Ê‚ð•Ô‚·
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
		protected:
			DealerHand dealer;
			Deck deck;
			const Rule rule;
			const Rate rate;
		public:
			Table(Deck deck = Deck(8), Rule rule = Rule(), Rate rate = Rate(1.5));
			bool addplayer(Player input);
			void play();
			void replay();
		};
		Result Judge(PlayerHand const& playerhand, DealerHand const& dealer);
		std::string Translate(int input);
	}

}