// TEST.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <valarray>
#include <time.h>
using namespace std;


//検証その1 forで初期化した方が約3倍早い
/*int main()
{
	valarray<double> r;
	clock_t start = clock();
	for (size_t i = 0; i < 100000000; i++)
	{
		r = { 0,0,0,0,0,0,0 };
	}
	clock_t end = clock();
	cout << (end - start) << endl;
	start = clock();
	for (size_t i = 0; i < 100000000; i++)
	{
		for (size_t j = 0; j < 7; j++)
		{
			r[j] = 0;
		}
	}
	end = clock();
	cout << (end - start)<< endl;
}*/

//検証その2 first/second は超早い 直接アドレス見てるから？
//というか、多分initiolizer_ist使うと遅い。make_pairはもっと遅い
/*int main() {
	pair<int, int> r;
	size_t count = 10000000;
	clock_t start = clock();
	for (size_t i = 0; i < count; i++)
	{
		r = { 0,0 };
	}
	clock_t end = clock();
	cout << (end - start) << endl;
	start = clock();
	for (size_t i = 0; i < count; i++)
	{
		r.first = 0;
		r.second = 0;
	}
	end = clock();
	cout << (end - start) << endl;
	start = clock();
	for (size_t i = 0; i < count; i++)
	{
		r = make_pair(0, 0);
	}
	end = clock();
	cout << (end - start) << endl;
}
*/