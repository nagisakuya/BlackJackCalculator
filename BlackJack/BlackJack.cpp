#include "BlackJack.h"
using namespace nagisakuya;

BlackJack::BlackJack(int Numberofdeck) {
	deck = Deck(Numberofdeck, 0);
}
void BlackJack::Play() {
	Hand dealer = Hand("Dealer's hand");
	Player player;
	dealer.add(deck.DrowRandom());
	dealer.print();
	player.begin(deck.DrowRandom(), deck.DrowRandom());
	player.act(&deck);
	dealer.hituntil17(&deck, Soft17Hit);
	player.judge(dealer);
}
BlackJack::Option BlackJack::AskOption(bool Split_enable, bool DoubleDown_enable, bool Surrender_enable) {
	string temp;
	for (;;) {
		cout << "Hit(H) or Stand(S)";
		if (DoubleDown_enable) cout << " or DoubleDown(D)";
		if (Split_enable) cout << " or Split(P)";
		if (Surrender_enable) cout << " or Surrender(R)";
		cout << endl;
		cin >> temp;
		if (Abblist.count(temp) == 0 || (Split_enable == false && temp == "SP") || (DoubleDown_enable == false && temp == "D") || (Surrender_enable == false && temp == "SU")) {
			cout << "error " << temp << " is out of option" << endl;
		}
		else break;
	}
	return Abblist.at(temp);
}
BlackJack::Result BlackJack::Judge(PlayerHand player, Hand dealer) {
	int dealer_sum, player_sum;
	bool playerBJ, dealerBJ;
	std::tie(player_sum, std::ignore, playerBJ) = player.CheckHand();
	std::tie(dealer_sum, std::ignore, dealerBJ) = dealer.CheckHand();
	if (dealer_sum > 21)dealer_sum = 0;
	if (player_sum > 21)player_sum = -1;
	if (playerBJ == true && dealerBJ == false) {
		return Result::BlackJack;
	}
	else if (player_sum < dealer_sum) {
		if (player.get_doubled() == false) {
			return Result::Lose;
		}
		else {
			return Result::DoubledLose;
		}
	}
	else if (player_sum == dealer_sum) {
		return Result::Tie;
	}
	else {
		if (player.get_doubled() == false) {
			return Result::Win;
		}
		else {
			return Result::DoubledWin;
		}
	}
}

BlackJack::Deck::Deck(int NumberofDeck, int Joker) {
	for (size_t i = 0; i < 13; i++)
	{
		content[i] = 4 * NumberofDeck;
	}
	content[13] = Joker;
}
void BlackJack::Deck::print() {
	for (int i = 0; i < 14; i++)
	{
		cout << "number of " << Translate(i) << "=" << content[i] << endl;
	}
}
int BlackJack::Deck::DrowRandom() {
	int temp = rand() % this->size();
	int sum = 0;
	for (size_t i = 0; i < 14; i++)
	{
		sum += content[i];
		if (sum >= temp) {
			content[i]--;
			return i;
		}
	}
	return 0;
}
int BlackJack::Deck::size() {
	int sum = 0;
	for (size_t i = 0; i < 14; i++)
	{
		sum += content[i];
	}
	return sum;
}


BlackJack::Hand::Hand(string name, vector<int> input) {
	this->name = name;
	hand = input;
}
;
void BlackJack::Hand::print() {
	int sum = 0;
	int AceCount = 0;
	cout << name << " is ";
	for (size_t i = 0; i < hand.size(); i++)
	{
		cout << Translate(hand[i]) << " ";
		if (hand[i] == 0) {
			sum += 11;
			AceCount++;
		}
		else if (hand[i] >= 9) {
			sum += 10;
		}
		else {
			sum += hand[i] + 1;
		}
		if (AceCount > 0 && sum > 21) {
			sum -= 10;
			AceCount--;
		}
	}
	cout << " sum is " << sum << endl;
}
tuple<int, bool, bool> BlackJack::Hand::CheckHand() {
	int sum = 0;
	int AceCount = 0;
	size_t size = hand.size();
	for (size_t i = 0; i < size; i++)
	{
		if (hand[i] == 0) {
			sum += 11;
			AceCount++;
		}
		else if (hand[i] >= 9) {
			sum += 10;
		}
		else {
			sum += hand[i] + 1;
		}
		if (AceCount > 0 && sum > 21) {
			sum -= 10;
			AceCount--;
		}
	}
	return { sum ,AceCount == 0 ? false : true ,(size == 2 && sum == 21) ? true : false };
}
void BlackJack::Hand::hituntil17(BlackJack::Deck* deck, bool Soft17Hit)
{
	int sum;
	bool soft;
	for (;;) {
		this->add(deck->DrowRandom());
		std::tie(sum, soft, std::ignore) = this->CheckHand();
		if (sum >= 17 && !(soft == true && sum == 17 && Soft17Hit == true)) break;
	}
	this->print();
}
void BlackJack::Hand::add(int i) {
	hand.push_back(i);
}

void BlackJack::PlayerHand::hit(BlackJack::Deck* deck)
{
	for (;;) {
		add(deck->DrowRandom());
		print();
		if (std::get<0>(CheckHand()) >= 21 || AskOption(false, false) == Option::Stand) break;
	}
}
BlackJack::PlayerHand::PlayerHand(string name, vector<int> input) :BlackJack::Hand(name , input) {
}
BlackJack::PlayerHand BlackJack::PlayerHand::split()
{
	splitted = true;
	name = "Primal" + name;
	PlayerHand r = PlayerHand("Splitted" + name);
	r.add(hand[1]);
	hand.pop_back();
	return r;
}
bool BlackJack::PlayerHand::splittable() {
	if (splitted == false && hand.size() == 2 && (hand[0] == hand[1] || hand[0] >= 9 && hand[1] >= 9))return true;
	else return false;
}

string BlackJack::Translate(int input) {
	switch (input) {
	case 0:
		return "A";
	case 10:
		return "J";
	case 11:
		return "Q";
	case 12:
		return "K";
	case 13:
		return "Joker";
	default:
		return std::to_string(input + 1);
	}
}

const std::map < BlackJack::Result, std::pair<double, string>> BlackJack::Rate = {
	{Result::Win		,std::make_pair(2,"Win")},
	{Result::Lose		,std::make_pair(0,"Lose")},
	{Result::Tie		,std::make_pair(1,"Tie")},
	{Result::BlackJack	,std::make_pair(2.5,"BlackJack")},
	{Result::Surrender	,std::make_pair(0.5,"Surrender")},
	{Result::DoubledWin	,std::make_pair(4,"DoubledWin")},
	{Result::DoubledLose,std::make_pair(-1,"DoubledLose")}
};
const std::map < string, BlackJack::Option> BlackJack::Abblist = {
	{"H"	,Option::Hit},
	{"S"	,Option::Stand},
	{"D"	,Option::DoubleDown},
	{"P"	,Option::Split},
	{"R"	,Option::Surrender}
};


bool BlackJack::Player::issplitted()
{
	return firsthand.get_splitted();
}

void BlackJack::Player::act(BlackJack::Deck* deck)
{
	switch (std::get<0>(firsthand.CheckHand()) >= 21 ? Option::Stand : AskOption(firsthand.splittable(), true)) {
	case Option::Hit:
		firsthand.hit(deck);
		break;
	case Option::Stand:
		break;
	case Option::DoubleDown:
		firsthand.set_doubled(true);
		firsthand.add(deck->DrowRandom());
		firsthand.print();
		break;
	case Option::Split:
		secondhand = firsthand.split();
		firsthand.hit(deck);
		secondhand.hit(deck);
		break;
	case Option::Surrender:
		firsthand.set_result(Result::Surrender);
		break;
	}
}

BlackJack::Player::Player(string name)
{
	this->name = name;
	firsthand = PlayerHand(name + "'s hand");
}
void BlackJack::Player::begin(int first, int second)
{
	firsthand.add(first);
	firsthand.add(second);
	firsthand.print();
}

void BlackJack::Player::judge(BlackJack::Hand dealer)
{
	if (firsthand.get_result() != Result::undefined) return;
	firsthand.set_result(BlackJack::Judge(firsthand, dealer));
	cout << this->name << " Result:" << Rate.at(firsthand.get_result()).second << endl;
	if (firsthand.get_splitted() == true) {
		secondhand.set_result(BlackJack::Judge(secondhand, dealer));
		cout << this->name << " Result:" << Rate.at(secondhand.get_result()).second << "(split)" << endl;
	}
}
