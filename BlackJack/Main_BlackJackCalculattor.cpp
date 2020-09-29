
#include <iostream>
#include "BlackJackCalculator.h"
#include <fstream>

using namespace nagisakuya::BlackJack;
using namespace std;



int main()
{
	ifstream inputfile;
	array<int, 10> deck;
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
		inputfile >> deck[i];
	}
	for (size_t i = 0; i < 3; i++)
	{
		inputfile >> temp_s;
		if (temp_s == "true") rule[i] = true;
		else if (temp_s == "false") rule[i] = false;
		else {
			cout << "Error:can't read " << i + 10 << " Th" << endl;
			return 1;
		}
	}
	inputfile >> BJrate;
	inputfile.close();
	ofstream outputfile;
	outputfile.open("output.txt", ios::trunc);
	Calculator test(Deck(deck), Rule(rule[0], rule[1], rule[2]), Rate(BJrate));
	test.Calculate(outputfile);
}