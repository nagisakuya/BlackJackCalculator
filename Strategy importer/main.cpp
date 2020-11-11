#include "../BlackJackLib/BlackJack.h"
#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>
using namespace std;
using namespace nagisakuya::BlackJack;

int main()
{
	ifstream inputfile;
	auto regular = new array<array < array < array<double, 3>, 10>, 10>, 10>;
	auto splittable = new array<array < array<double, 4>, 10>, 10>;
	inputfile.open("input-regular-strategy.txt", ios::in);
	int temp_int[3];
	string temp_string;
	for (size_t i = 0;; i++)
	{
		inputfile >> temp_string;
		if (inputfile.eof()) {
			break;
		}
		temp_int[0] = Translate(temp_string);
		inputfile >> temp_string;
		temp_int[1] = Translate(temp_string.substr(0, temp_string.find(",")));
		temp_int[2] = Translate(temp_string.substr(temp_string.find(",") + 1));
		inputfile >> (*regular)[temp_int[0]][temp_int[1]][temp_int[2]][0];
		inputfile >> (*regular)[temp_int[0]][temp_int[1]][temp_int[2]][1];
		inputfile >> (*regular)[temp_int[0]][temp_int[1]][temp_int[2]][2];
		inputfile >> temp_string;
	}
	inputfile.close();
	inputfile.open("input-splittable-strategy.txt", ios::in);
	for (size_t i = 0;; i++)
	{
		inputfile >> temp_string;
		if (inputfile.eof()) {
			break;
		}
		temp_int[0] = Translate(temp_string);
		inputfile >> temp_string;
		temp_int[1] = Translate(temp_string.substr(0, temp_string.find(",")));
		inputfile >> (*splittable)[temp_int[0]][temp_int[1]][0];
		inputfile >> (*splittable)[temp_int[0]][temp_int[1]][1];
		inputfile >> (*splittable)[temp_int[0]][temp_int[1]][2];
		inputfile >> (*splittable)[temp_int[0]][temp_int[1]][3];
		inputfile >> temp_string;
		inputfile >> temp_string;
	}
	inputfile.close();
	ofstream outputfile;
	outputfile.open("output-strategy.txt", ios::trunc);
	Rule temp_rule;
	for (Rule::List i = (Rule::List)0; i < Rule::List::EoE; i = (Rule::List)((int)i + 1))
	{
		while (true) {
			cout << Rule::RuleandString.at_ftos(i) << "? true or false" << endl;
			cin >> temp_string;
			if (temp_string == "true") {
				temp_rule.set(i, true);
				break;
			}
			else if (temp_string == "false") {
				temp_rule.set(i, false);
				break;
			}
		}
	}
	outputfile << temp_rule.print() << endl;
	for (size_t i = 10; i-- != 0;)
	{
		for (size_t j = 10; j-- != 0;)
		{
			for (size_t k = j; k-- != 0;)
			{
				outputfile << i << " " << j << " " << k << " ";
				auto temp_itr = max_element((*regular)[i][j][k].begin(), (*regular)[i][j][k].end());
				if (*temp_itr == (*regular)[i][j][k][0]) {
					outputfile << OptiontoString.at(Option::Stand) << endl;
				}
				else if (*temp_itr == (*regular)[i][j][k][1]) {
					outputfile << OptiontoString.at(Option::Hit) << endl;
				}
				else if (*temp_itr == (*regular)[i][j][k][2]) {
					outputfile << OptiontoString.at(Option::Double) << endl;
				}
			}
		}
	}
	for (size_t i = 10; i-- != 0;)
	{
		for (size_t j = 10; j-- != 0;)
		{
			outputfile << i << " " << j << " ";
			auto temp_itr = max_element((*splittable)[i][j].begin(), (*splittable)[i][j].end());
			if (*temp_itr == (*splittable)[i][j][0]) {
				outputfile << OptiontoString.at(Option::Stand) << endl;
			}
			else if (*temp_itr == (*splittable)[i][j][1]) {
				outputfile << OptiontoString.at(Option::Hit) << endl;
			}
			else if (*temp_itr == (*splittable)[i][j][2]) {
				outputfile << OptiontoString.at(Option::Double) << endl;
			}
			else if (*temp_itr == (*splittable)[i][j][3]) {
				outputfile << OptiontoString.at(Option::Split) << endl;
			}
		}
	}
	outputfile.close();
}

