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
	NeuralNetwork<2, 30, 1> logic(0.03);
	array<double, 1> result;
	/*
	result = XOR.calculate({ {1,1} });
	std::cout << result[0] << endl;
	result = XOR.calculate({ {1,0} });
	std::cout << result[0] << endl;
	result = XOR.calculate({ {0,1} });
	std::cout << result[0] << endl;
	result = XOR.calculate({ {0,0} });
	std::cout << result[0] << endl;
	*/
	for (size_t i = 0; i < 1000; i++)
	{
		logic.study({ {1,1} }, { {0} });
		logic.study({ {1,0} }, { {1} });
		logic.study({ {0,1} }, { {1} });
		logic.study({ {0,0} }, { {0} });
	}
	result = logic.calculate({ {1,1} });
	std::cout << "(1,1) = " << result[0] << endl;
	result = logic.calculate({ {1,0} });
	std::cout << "(1,0) = " << result[0] << endl;
	result = logic.calculate({ {0,1} });
	std::cout << "(0,1) = " << result[0] << endl;
	result = logic.calculate({ {0,0} });
	std::cout << "(0,0) = " << result[0] << endl;
}