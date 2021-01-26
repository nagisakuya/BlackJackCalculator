
#include <iostream>
#include "../BlackJackLib/BlackJackCalculator.h"
#include <fstream>

using namespace nagisakuya::BlackJack;
using namespace nagisakuya::Utility;
using namespace std;


int main()
{
	cout << "BlackJackCalculator" << endl;
	ifstream inputfile;
	Calculator calc;
	inputfile.open("input.txt", ios::in);
	if (!inputfile) {
		cout << "Error:file open fizzed" << endl;
		exit(0);
	}
	calc.import(inputfile);
	inputfile.close();
	cfstream cfout("output.txt", ios::trunc);
	calc.calculate_withoutput(cfout);
	cfout.close();
}