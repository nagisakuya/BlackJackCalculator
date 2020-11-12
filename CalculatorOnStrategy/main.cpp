
#include <iostream>
#include "../BlackJackLib/BlackJackCalculator.h"
#include <fstream>

using namespace nagisakuya::BlackJack;
using namespace nagisakuya::Utility;
using namespace std;


int main()
{
	cout << "BlackJackCalculator on strategy ver" << endl;
	ifstream inputfile;
	array<int,10> deck;
	double BJrate;
	string temp_s;
	inputfile.open("input.txt", ios::in);
	if (!inputfile) {
		cout << "Error:file open fizzed" << endl;
		exit(0);
	}
	for (size_t i = 0; i < 10; i++)
	{
		inputfile >> temp_s;
		inputfile >> deck[i];
	}
	Rule rule(inputfile);
	inputfile >> temp_s;
	inputfile >> BJrate;
	inputfile.close();
	cfstream cfout("output_onstrategy.txt", ios::trunc);
	Calculator calculator(Deck(deck), rule, Rate(BJrate));
	calculator.calculate_onstrategy(cfout);
	cfout.close();
}