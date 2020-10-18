
#include <iostream>
#include "BlackJackCalculator.h"
#include <fstream>

using namespace nagisakuya::BlackJack;
using namespace nagisakuya::Utilty;
using namespace std;


int main()
{
	cout << "BlackJackCalculator Ver 1.1.2" << endl;
	ifstream inputfile;
	array<int,10> deck;
	array<bool,3> rule;
	double BJrate;
	string temp_s;
	inputfile.open("input.txt", ios::in);
	if (!inputfile) {
		cout << "Error:file open fizzed" << endl;
		return 1;
	}
	for (size_t i = 0; i < 10; i++)
	{
		inputfile >> temp_s;
		inputfile >> deck[i];
	}
	for (size_t i = 0; i < 3; i++)
	{
		inputfile >> temp_s;
		inputfile >> temp_s;
		if (temp_s == "true") rule[i] = true;
		else if (temp_s == "false") rule[i] = false;
		else {
			cout << "Error:can't read " << i + 10 << " Th option" << endl;
			return 1;
		}
	}
	inputfile >> temp_s;
	inputfile >> BJrate;
	inputfile.close();
	cfstream cfout("output.txt", ios::trunc);
	Calculator calculator(Deck(deck), Rule(rule[0], rule[1], rule[2]), Rate(BJrate));
	calculator.calculate(cfout);
	cfout.close();
}