// Fix.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include "BlackJack.h"
using namespace std;
using namespace nagisakuya::BlackJack;

int main()
{
    string temp_s;
    ifstream file;
    array<int, 10> temp_array;
	double temp;
	double sum = 0;
    istringstream temp_ss;
    file.open("output.txt", ios::in);
    cout << "Fixer Ver 1.0.0" << endl;
    for (size_t i = 0; i < 7; i++)
    {
        file >> temp_s;
    }
    for (size_t i = 0; i < 10; i++)
    {
        file >> temp_s;
        temp_ss = istringstream(temp_s.substr(2));
        temp_ss >> temp_array[i];
    }
    Deck deck(temp_array);
	Deck temp_deck[3];
	for (int i = 10; i-- > 0;)
	{
		if (deck.count(i) != 0) {
			temp_deck[0] = deck - i;
			for (int j = 10; j-- > i;)
			{
				if (temp_deck[0].count(j) != 0) {
					temp_deck[1] = temp_deck[0] - j;
					for (int k = 10; k-- > 0; )
					{
						if (temp_deck[1].count(k) != 0) {
							for (size_t l = 0; l < 8; l++)
							{
								file >> temp_s;
							}
							file >> temp_s;
							temp_ss = istringstream(temp_s.substr(14));
							temp_ss >> temp;
							sum += temp * (i == j ? 1 : 2) * ((double)deck.count(i) * (double)temp_deck[0].count(j) * (double)temp_deck[1].count(k));
						}
					}
				}
			}
		}
	}
	double size = deck.size();
	cout << sum / (size * (size - 1) * (size - 2)) << endl;
	cout << "press any key and enter to end...";
	cin >> temp_s;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
