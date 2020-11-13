#include "../BlackJackLib/BlackJackCalculator.h"
#include <tuple>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <array>
#include <valarray>
#include <ctime>
#include <sstream>
#include <map>
#include <algorithm>
using namespace nagisakuya::BlackJack;
using namespace nagisakuya::Utility;
using namespace std;

void test_strategy1() {
	Strategy strategy;
	cout <<  strategy.print();
	DealerHand dealer({ 7 - 1 });
	PlayerHand player({ 1 - 1, 5 - 1 , 5 - 1 });
	dealer.print();
	player.print();
	cout << OptionandString.at(strategy.find(dealer, player, Rule())) << endl;
}

void test_strategy2() {
	cfstream cf("/dev/null");
	cout << Calculator(Deck(1)).calculate_onstrategy(cf);
}

void test_strategy3() {
	ifstream file("BasicStrategy.txt", ios::in);
	Strategy strategy;
	strategy.import(file);
	cout << strategy.print();
	cout << strategy.splittable.print_initializer_list();
}

int main(){
	test_strategy3();
}