//import from WoO
//https://wizardofodds.com/games/blackjack/expected-values/
//ベーシックストラテジーをインポートしようと思ったけど使えないことに気が付く…(´・ω・`)

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
	auto regular = new array<array < array < array<double, 3>, 9>, 10>, 10>;
	auto splittable = new array<array < array<double, 4>, 10>, 10>;
	int temp_int[3];
	string temp_string;
	Rule temp_rule;
	inputfile.open("input-rule.txt", ios::in);
	temp_rule.import(inputfile);
	inputfile.close();
	inputfile.open("input-regular-strategy.txt", ios::in);
	for (size_t i = 0;; i++)
	{
		inputfile >> temp_string;
		if (inputfile.eof()) {
			break;
		}
		temp_int[0] = (int)Card(temp_string);
		inputfile >> temp_string;
		temp_int[1] = (int)Card(temp_string.substr(0, temp_string.find(",")));
		temp_int[2] = (int)Card(temp_string.substr(temp_string.find(",") + 1));
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
		temp_int[0] = (int)Card(temp_string);
		inputfile >> temp_string;
		temp_int[1] = (int)Card(temp_string.substr(0, temp_string.find(",")));
		inputfile >> (*splittable)[temp_int[0]][temp_int[1]][0];
		inputfile >> (*splittable)[temp_int[0]][temp_int[1]][1];
		inputfile >> (*splittable)[temp_int[0]][temp_int[1]][2];
		if (temp_rule.at(Rule::List::DoubleAfterSplit) == false) {
			inputfile >> (*splittable)[temp_int[0]][temp_int[1]][3];
			inputfile >> temp_string;
		}
		else {
			inputfile >> temp_string;
			inputfile >> (*splittable)[temp_int[0]][temp_int[1]][3];
		}
		inputfile >> temp_string;
	}
	inputfile.close();
	/*
	for (int i = 10; i-- != 0;)
	{
		for (int j = 10; j-- != 0;)
		{
			for (int k = j; k-- != 0;)
			{
				auto temp_itr = max_element((*regular)[i][j][k].begin(), (*regular)[i][j][k].end());
				if (*temp_itr == (*regular)[i][j][k][0]) {
					temp_strategy.set({ i,{k,j} }, Option::Stand);
				}
				else if (*temp_itr == (*regular)[i][j][k][1]) {
					temp_strategy.set({ i,{k,j} }, Option::Hit);
				}
				else if (*temp_itr == (*regular)[i][j][k][2]) {
					temp_strategy.set({ i,{k,j} }, Option::Double);
				}
			}
		}
	}
	for (int i = 10; i-- != 0;)
	{
		for (int j = 10; j-- != 0;)
		{
			auto temp_itr = max_element((*splittable)[i][j].begin(), (*splittable)[i][j].end());
			if (*temp_itr == (*splittable)[i][j][0]) {
				temp_strategy.set({ i,{j,j} }, Option::Stand);
			}
			else if (*temp_itr == (*splittable)[i][j][1]) {
				temp_strategy.set({ i,{j,j} }, Option::Hit);
			}
			else if (*temp_itr == (*splittable)[i][j][2]) {
				temp_strategy.set({ i,{j,j} }, Option::Double);
			}
			else if (*temp_itr == (*splittable)[i][j][3]) {
				temp_strategy.set({ i,{j,j} }, Option::Split);
			}
		}
	}
	ofstream outputfile;
	outputfile.open("output-strategy.txt", ios::trunc);
	outputfile << temp_strategy.print() << endl;
	outputfile.close();
	*/
}

