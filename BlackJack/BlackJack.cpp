#include "BlackJack.h"

using namespace nagisakuya::BlackJack;
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::tuple;
using std::pair;
using std::map;
using json = nlohmann::json;

Table::Table(int Numberofdeck, double BlackJackrate) {
	deck = Deck(Numberofdeck, 0);
	Rate = {
	{Result::Win		,2},
	{Result::Lose		,0},
	{Result::Tie		,1},
	{Result::BlackJack	,BlackJackrate},
	{Result::Surrender	,0.5},
	{Result::DoubledWin	,4},
	{Result::DoubledLose,-1}
	};
}
void Table::Play() {
	if (PlayerList.size() == 0) {
		cout << "there is no player" << endl;
		return;
	}
	Hand dealer = Hand("Dealer's hand");
	dealer.add(deck.DrowRandom());
	dealer.print();
	//関数ポインタでplayereach関数をを作ろうと思った
	for (auto i = PlayerList.begin(); i != PlayerList.end(); ++i)
	{
		i->second.begin(deck.DrowRandom(), deck.DrowRandom());
	}
	for (auto i = PlayerList.begin(); i != PlayerList.end(); ++i)
	{
		i->second.act(&deck);
	}
	dealer.hituntil17(&deck, Soft17Hit);
	for (auto i = PlayerList.begin(); i != PlayerList.end(); ++i)
	{
		i->second.act(&deck);
	}
	for (auto i = PlayerList.begin(); i != PlayerList.end(); ++i)
	{
		i->second.firsthand.set_result(Table::Judge(i->second.firsthand, dealer));
		cout << i->second.firsthand.get_name() << " Result:" << ResulttoString.at(i->second.firsthand.get_result()) << endl;
		if (i->second.firsthand.get_splitted() == true) {
			i->second.secondhand.set_result(Table::Judge(i->second.secondhand, dealer));
			cout << i->second.secondhand.get_name() << " Result:" << ResulttoString.at(i->second.secondhand.get_result()) << endl;
		}
	}
}
Option nagisakuya::BlackJack::AskOption(bool Split_enable, bool DoubleDown_enable, bool Surrender_enable) {
	string temp;
	for (;;) {
		cout << "Hit(H) or Stand(S)";
		if (DoubleDown_enable) cout << " or DoubleDown(D)";
		if (Split_enable) cout << " or Split(P)";
		if (Surrender_enable) cout << " or Surrender(R)";
		cout << endl;
		cin >> temp;
		if (temp == "H") return Option::Hit;
		else if (temp == "S") return Option::Stand;
		else if (temp == "D" && DoubleDown_enable == true) return Option::DoubleDown;
		else if (temp == "P" && Split_enable == true) return Option::Split;
		else if (temp == "R" && Surrender_enable == true) return Option::Surrender;
		else cout << "error " << temp << " is out of option" << endl;;
	}
}
void nagisakuya::BlackJack::Table::addplayer(Player input)
{
	PlayerList.emplace(input.get_ID(), input);
}
Result Table::Judge(PlayerHand playerhand, Hand dealer) {
	if (playerhand.get_result() != Result::undefined)return playerhand.get_result();
	int dealer_sum, player_sum;
	bool playerBJ, dealerBJ;
	std::tie(player_sum, std::ignore, playerBJ) = playerhand.CheckHand();
	std::tie(dealer_sum, std::ignore, dealerBJ) = dealer.CheckHand();
	if (dealer_sum > 21)dealer_sum = 0;
	if (player_sum > 21)player_sum = -1;
	if (playerBJ == true && dealerBJ == false) {
		return Result::BlackJack;
	}
	else if (player_sum < dealer_sum) {
		if (playerhand.get_doubled() == false) {
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
		if (playerhand.get_doubled() == false) {
			return Result::Win;
		}
		else {
			return Result::DoubledWin;
		}
	}
}

Deck::Deck(int NumberofDeck, int Joker) {
	for (size_t i = 0; i < 13; i++)
	{
		content[i] = 4 * NumberofDeck;
	}
	content[13] = Joker;
}
void Deck::print() {
	for (int i = 0; i < 14; i++)
	{
		cout << "number of " << Translate(i) << "=" << content[i] << endl;
	}
}
int Deck::DrowRandom() {
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
int Deck::size() {
	int sum = 0;
	for (size_t i = 0; i < 14; i++)
	{
		sum += content[i];
	}
	return sum;
}


Hand::Hand(string name, vector<int> input) {
	this->name = name;
	hand = input;
}
void Hand::print() {
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
tuple<int, bool, bool> Hand::CheckHand() {
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
void Hand::hituntil17(Deck* deck, bool Soft17Hit)
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
void Hand::add(int i) {
	hand.push_back(i);
}

void PlayerHand::hit(Deck* deck)
{
	for (;;) {
		add(deck->DrowRandom());
		print();
		if (std::get<0>(CheckHand()) >= 21 || AskOption(false, false) == Option::Stand) break;
	}
}
PlayerHand::PlayerHand(string name, vector<int> input) :Hand(name, input) {
}
PlayerHand PlayerHand::split()
{
	splitted = true;
	name = "Primal" + name;
	PlayerHand r = PlayerHand("Splitted" + name);
	r.add(hand[1]);
	hand.pop_back();
	return r;
}
bool PlayerHand::splittable() {
	if (splitted == false && hand.size() == 2 && (hand[0] == hand[1] || hand[0] >= 9 && hand[1] >= 9))return true;
	else return false;
}

string nagisakuya::BlackJack::Translate(int input) {
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
const map< Result, string> Table::ResulttoString = {
	{Result::Win		,"Win"},
	{Result::Lose		,"Lose"},
	{Result::Tie		,"Tie"},
	{Result::BlackJack	,"BlackJack"},
	{Result::Surrender	,"Surrender"},
	{Result::DoubledWin	,"DoubledWin"},
	{Result::DoubledLose,"DoubledLose"}
};


bool Player::issplitted()
{
	return firsthand.get_splitted();
}

void Player::act(Deck* deck)
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

Player::Player(int ID, string name)
{
	this->name = name;
	this->ID = ID;
	firsthand = PlayerHand(name + "'s hand");
}
void Player::begin(int first, int second)
{
	firsthand.add(first);
	firsthand.add(second);
	firsthand.print();
}
