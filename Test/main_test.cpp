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
	strategy.import(file);
}

int main(){
	test_strategy1();
}