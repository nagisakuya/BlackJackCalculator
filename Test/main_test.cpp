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
#include <random>
#include <algorithm>
#include <type_traits>
#include <cmath>
#include "../matrix/matrix.h"
#include "../neural network/neuralnetwork.h"
using namespace nagisakuya::BlackJack;
using namespace nagisakuya::Utility;
using namespace nagisakuya::neural;
using namespace std;

void test_strategy1() {
	Strategy strategy;
	cout << strategy.print();
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

void test_dolambda(void(*lambda)()) {
	int a = 0;
	lambda();
}

void bfunc(Rule b) {};

void test_lambda1() {
	auto lambda = [] {cout << "Hello World"; return; };
	test_dolambda(lambda);
}

void test_lambda2() {
	int a = 1;
	auto lambda = [&]()->void {cout << a; };
}

void test_card() {
	cout << ((Card)"T").str();
}

void func1(int i) {};
void func2(Card const& i) {};

void speed_test() {
	const int times = 10000000;
	clock_t start = clock();
	for (int i = 0; i < times; i++)
	{
		func1(1);
	}
	cout << (clock() - start) << endl;
	start = clock();
	Card c(1);
	for (int i = 0; i < times; i++)
	{
		func2(Card(1));
	}
	cout << (clock() - start) << endl;
}

void matrixtest() {
	matrix<int, 2, 3> m({ { {1,2,3},{3,4,5} } });
	matrix<int, 3, 2> n({ { {4,3},{2,1},{3,3} } });
	cout << m.str()<< endl;
	cout << m.transpose().str() << endl;
}



int main() {
	double a = std::exp(-1000);
	cout << a;
}