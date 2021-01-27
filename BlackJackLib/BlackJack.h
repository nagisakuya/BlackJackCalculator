#pragma once
#include "pch.h"
#include "../Utility/Utility.h"
#define GENERATE_ENUM_ITERATOR(T) \
inline T operator++(T& x) { return x = (T)(std::underlying_type<T>::type(x) + 1); } \
inline T operator*(T c) { return c; } \
inline T begin(T r) { return static_cast<T>(0); } \
inline T end(T r) { T l = T::LAST; return l; }


namespace nagisakuya {
	namespace BlackJack {
		enum class Result { Win, Lose, Tie, BlackJack, Surrender, DoubledWin, DoubledLose, undefined };
		enum class Option { Hit, Stand, Double, Split, Surrender , null};
		const Utility::bijection<Option, std::string> OptionandString{
			{Option::Hit	,"Hit"},
			{Option::Stand	,"Stand "},
			{Option::Double,"Double"},
			{Option::Split,"Split "},
			{Option::Surrender,"Surrender"}
		};
		class Rule {
		public:
			enum class List { Soft17Hit, Surrender, DoubleAfterSplit, EoE };//Rule::List i = (Rule::List)0; i < Rule::List::EoE; i = (Rule::List)((int)i + 1)
			const static Utility::bijection<List, std::string> RuleandString;
		private:
			std::unordered_map<List, bool> element;
		public:
			bool at(List input) const { return element.at(input); }
			explicit Rule(bool Soft17Hit = false, bool Surrender = false, bool DoubleAfterSplit = false);
			explicit Rule(std::istream&);
			void set(List i, bool j) { element.at(i) = j; }
			std::string print() const;
			void import(std::istream&);
			void ask();
		};
		class Rate {
		private:
			std::unordered_map<Result, double> element;
		public:
			explicit Rate(double BlackJackRate = 1.5);
			std::unordered_map<Result, double>& get_ref() { return element; };
			double get(Result input)const { return element.at(input); }
			double& at(Result input) { return element.at(input); }
			std::string print() const;
			double& operator[](Result i) { return element[i]; }
		};
		class Card {
		private:
			int suit;
		public:
			Card(int i) { suit = i; };
			explicit Card(std::string);
			int num() const { return suit + 1; };
			explicit operator int() const { return suit; }
			bool operator ==(Card c) const { return suit == c.suit; }
			bool operator <=(Card c) const { return suit <= c.suit; }
			bool operator >=(Card c) const { return suit >= c.suit; }
			//Hand operator +(Card c) const { return Hand({ *this , c }); }
			std::string str() const;
			static Card numtocard(int i) { return Card(i - 1); }
		};
		class Hand;
		class Deck {
		protected:
			std::array<int, 10> content;
		public:
			explicit Deck(int NumberofDeck = 8);
			explicit Deck(std::array<int, 10> input) { content = input; }
			int count(Card c) const { return content[(int)c]; }
			std::string print() const;
			inline int size() const;
			void Drow(Card c) { content[(int)c]--; }
			void Drow(Hand);
			Card DrowRandom();
			//Deck copy() const { return *this; }
			//Deck IfDrow(Card c) const { return copy().Drow(c); };
			Deck operator - (Card c) const { Deck r = *this; r.Drow(c); return r; }
			//Deck IfDrow(std::vector<Card> remlist) const { return copy().Drow(remlist); };
			//Deck& operator - (Hand h) const { return copy().Drow(h); }
			void import(std::istream&);
		};
		class Hand {
		protected:
			std::vector<Card> cards;
		public:
			Hand(std::vector<Card> input = {}) { cards = input; }
			void add(Card input) { cards.emplace_back(input); }
			size_t size() const { return cards.size(); }
			/// <returns>tuple&lt;sum,issoft,isBJ&gt;</returns>
			inline std::tuple<int, bool, bool> CheckHand() const;
			virtual void print() const;
			Card const& operator [](int i) const { return cards[i]; }
			Hand operator +(Card c) const { Hand r = *this; r.add(c); return r; }
		};
		class DealerHand :public Hand {
		public:
			explicit DealerHand(std::vector<Card> input = {}) :Hand(input) {};
			void print() const;
			void hituntil17(Deck& deck, Rule const& rule);
			Card get_upcard() const { return cards[0]; }
			DealerHand Ifhit(Card c) const { DealerHand r = *this; r.add(c); return r; }
		};
		class PlayerHand :public Hand {
		private:
			Result result = Result::undefined;
			bool splitted = false;
			bool doubled = false;
			const static std::unordered_map< Result, std::string> ResulttoString;
			static Option AskOption(bool Split_enable = false, bool DoubleDown_enable = false, bool Surrender_enable = false);
		public:
			explicit PlayerHand(std::vector<Card> input = {}, bool splitted = false, bool doubled = false) :Hand(input) { this->splitted = splitted; this->doubled = doubled; }
			inline bool splittable() const;
			inline bool doubleble(bool DoubleafterSplit) const {
				if (DoubleafterSplit == false) {
					return size() == 2 && splitted == false;
				}
				return size() == 2;
			}
			PlayerHand split(Deck* deck);
			Option play(Deck* deck, Rule const& rule);
			void judge(DealerHand const& dealer);
			bool get_splitted() const { return splitted; }
			bool get_doubled() const { return doubled; }
			Result get_result() const { return result; }
			PlayerHand Ifhit(Card c) const { PlayerHand r(cards, splitted, doubled); r.add(c); return r; }
			PlayerHand Ifdouble(Card c) const { PlayerHand r(cards, splitted, true); r.add(c); return r; }
			PlayerHand Ifsplit(Card c) const { PlayerHand r({ cards[0] }, true, false); r.add(c); return r; }
			//PlayerHand operator + (int i) const { PlayerHand r(content, splitted, doubled); r.add(i); return r; }//i‚ð‰Á‚¦‚½Œ‹‰Ê‚ð•Ô‚·
			//PlayerHand operator * (int i) const { PlayerHand r(content, splitted, true); r.add(i); return r; }//Double‚µ‚Äi‚ð‰Á‚¦‚½Œ‹‰Ê‚ð•Ô‚·
			//PlayerHand operator / (int i) const { PlayerHand r({ content[0] }, true, false); r.add(i); return r; }//Split‚µ‚½‚ ‚Æi‚ð‰Á‚¦‚½Œ‹‰Ê‚ð•Ô‚·
		};
		class Player {
		protected:
			std::string name;
			int id;
		public:
			std::pair<PlayerHand, PlayerHand> hand;
			explicit Player(int ID, std::string name = "Player");
			void play(Deck* deck, Rule const& rule);
			int get_ID() const { return id; }
			std::string get_name() const { return name; }
		};
		class Strategy {
		public:
			enum class Option { Hit, Stand, Double, Split, Doublestand, Splithit, Surrenderhit ,Notsplit };
			const static Utility::bijection<Strategy::Option, std::string> OptionandString;
		private:
			const static Utility::bijection<Strategy::Option, std::string> OptionandOption;
			class Splittable {
			private:
				std::array < std::array<Strategy::Option, 10>, 10> list;
				Strategy::Option& get_ref(Card dealer, Card player) { return list[(int)dealer][(int)player]; }
			public:
				Splittable();
				Strategy::Option get(Card dealer, Card player) const { return list[(int)dealer][(int)player]; }
				void import(std::istream&);
				std::string print();
				std::string print_initializer_list();
			};
			class Soft {
			private:
				std::array < std::array<Strategy::Option, 9>, 10> list;
				Strategy::Option& get_ref(Card dealer, Card player_nonAcard) { return list[(int)dealer][(int)player_nonAcard - 1]; }
			public:
				Soft();
				Strategy::Option get(Card dealer, int player_sum) const { return list[(int)dealer][player_sum - 11 - 1 - 1]; }
				void import(std::istream&);
				std::string print();
				std::string print_initializer_list();
			};
			class Hard {
			private:
				std::array < std::array<Strategy::Option, 10>, 10> list;
				Strategy::Option& get_ref(Card dealer, int player_sum) { if (player_sum >= 17)return list[(int)dealer][9]; if (player_sum <= 8)return list[(int)dealer][0]; return list[(int)dealer][player_sum - 8]; }
			public:
				Hard();
				Strategy::Option get(Card dealer, int player_sum) const { if (player_sum >= 17)return list[(int)dealer][9]; if (player_sum <= 8)return list[(int)dealer][0]; return list[(int)dealer][player_sum - 8]; }
				void import(std::istream&);
				std::string print();
				std::string print_initializer_list();
			};
		public:
			Splittable splittable;
			Soft soft;
			Hard hard;
			void import(std::istream&);
			std::string print();
			BlackJack::Option find(DealerHand const& dealer, PlayerHand const& player, Rule const& rule) const;
		};
		class Table {
		private:
			std::vector<Player> PlayerList;
		protected:
			DealerHand dealer;
			Deck deck;
			Rule rule;
			Rate rate;
		public:
			explicit Table(Deck = Deck(8), Rule = Rule(), Rate = Rate(1.5), DealerHand = DealerHand());
			bool addplayer(Player);
			void play();
			void replay();
			bool getrule(Rule::List input) const { return rule.at(input); }
			double getrate(Result input) const { return rate.get(input); }
			DealerHand getdealer() const { return dealer; }
			Deck getdeck() const { return deck; }
			std::string PrintStatus();
			void import(std::istream&);
		};
	}
}