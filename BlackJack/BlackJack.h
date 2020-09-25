#pragma once
#include <tuple>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::tuple;
using json = nlohmann::json;



class BlackJack {
protected:
	enum class Option { Hit, Stand, DoubleDown, Split, Surrender };
	enum class Result { Win, Lose, Tie, BlackJack, Surrender, DoubledWin, DoubledLose, undefined };
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
		vector<int> hand;
		string name;
	public:
		void add(int i);
		void print();
		Hand(string input);
		tuple<int, bool, bool> CheckHand();
		void hituntil17(BlackJack::Deck* deck,bool Soft17Hit = false);
	};
	class PlayerHand :public BlackJack::Hand {
	protected:
		Result result = Result::undefined;
		bool doubled = false;
	public:
		PlayerHand(string input = "Player's hand");
		void hit(BlackJack::Deck* deck);
		PlayerHand split();
		bool splittable();
		void set_result(Result r) { result = r; }
		Result get_result() { return result; }
		bool get_doubled() { return doubled; }
		void set_doubled(bool i) { doubled = i; }
	};
	Deck deck;
	bool Soft17Hit = false;
	bool SurrenderFlag = false;
	bool DoubleAfterSplit = false;
	const static std::map<Result, std::pair<double, string>>Rate;
	const static std::map< string, BlackJack::Option> Abblist;

	static BlackJack::Option AskOption(bool Split_enable = false, bool DoubleDown_enable = false, bool Surrender_enable = false);
public:
	class Player {
	protected:
		bool splitted = false;
		string name;
		PlayerHand firsthand, secondhand;
	public:
		void split();
		void act(BlackJack::Deck* deck);
		Player(string name = "Player");
		void begin(int first, int second);
		PlayerHand get_firsthand() { return firsthand; }
		PlayerHand get_secondhand() { return secondhand; }
		void Judge(BlackJack::Hand dealer);
	};
	BlackJack(int Numberofdeck = 8);
	void Play();
	static BlackJack::Result Judge(PlayerHand player, Hand dealer);
	static string Translate(int input);
};

