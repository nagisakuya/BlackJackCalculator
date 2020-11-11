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
	ifstream file("BasicStrategy.txt", ios::in);
	Strategy strategy;
	//strategy.import(file);
	cout <<  strategy.print();
	DealerHand dealer({ 7 - 1 });
	PlayerHand player({ 1 - 1, 5 - 1 , 2 - 1 });
	dealer.print();
	player.print();
	cout << OptionandString.at(strategy.find(dealer, player, Rule())) << endl;
}

int main(){
	test_strategy1();
}