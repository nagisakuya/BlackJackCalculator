// NumeralNeural.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "../neural network/neuralnetwork.h"
#include <iostream>
#include <fstream>
#include <string>


using namespace std;
using namespace nagisakuya::neural;

string split(const string& src, const char* delim = ",") {
	stringstream ss;
	string::size_type len = src.length();
	for (string::size_type i = 0, n; i < len; i = n + 1) {
		n = src.find_first_of(delim, i);
		if (n == string::npos) {
			n = len;
		}
		ss << src.substr(i, n - i) << " ";
	}
	return ss.str();
}

constexpr size_t INPUTS = 784;
constexpr size_t HIDDENS = 100;
constexpr size_t OUTPUTS = 10;

constexpr double LEARNING_RATE = 0.01;

constexpr size_t BATCH_SIZE = 60000;
constexpr size_t TEST_DATE_SIZE = 10000;

constexpr size_t MINI_BATCH_SIZE = 8;
constexpr size_t STUDY_TIMES = 100000;

class mnist_datas {
private:
	const size_t size;
	size_t called = 0;
	ifstream data_ifstream;
public:
	mnist_datas(string path, size_t data_size)
		:size(data_size) {
		data_ifstream.open(path);
		if (!data_ifstream.is_open()) {
			cout << "file open error : PATH = " << path;
			exit(0);
		}
	}
	~mnist_datas() {
		data_ifstream.close();
	}
	pair<array<double, INPUTS>, size_t> get_data() {
		called++;
		if (called % size == 0) {
			data_ifstream.seekg(0, ios_base::beg);
		}
		string buffer;
		data_ifstream >> buffer;
		stringstream ss(split(buffer));
		int answer;
		ss >> answer;
		array<double, INPUTS> input;
		for (size_t i = 0; i < INPUTS; i++)
		{
			ss >> input[i];
			input[i] = (input[i] + 1) / 256;
		}
		return make_pair(input, answer);
	}
	layer_pair_data<INPUTS, OUTPUTS> get_train_data() {
		auto data = get_data();
		array<double, OUTPUTS> teacher;
		teacher.fill(0);
		teacher[data.second] = 1.0;
		
		return make_pair(data.first, teacher);
	}
	mini_batch<INPUTS, OUTPUTS> get_mini_batch(size_t batch_size) {
		mini_batch<INPUTS, OUTPUTS> batch;
		for (size_t i = 0; i < 16; i++)
		{
			batch.push_back(get_train_data());
		}
		return batch;
	}
};

int main()
{
	mnist_datas train_data("mnist_train.csv", BATCH_SIZE);
	mnist_datas test_data("mnist_test.csv", TEST_DATE_SIZE);
	auto* mnist = new ThreeLayersNeural<INPUTS, HIDDENS, OUTPUTS>(LEARNING_RATE, Activator::sigmoid, Activator::sigmoid);
	for (size_t i = 0; i < STUDY_TIMES; i++)
	{
		mnist->mini_batch_SGD(train_data.get_mini_batch(MINI_BATCH_SIZE));
		if (i % (STUDY_TIMES / 20) == 0)
		{
			cout << i * 100 / STUDY_TIMES << "% of trainings compleated" << endl;
		}
	}
	int correct_count = 0;
	for (size_t i = 0; i < TEST_DATE_SIZE; i++)
	{
		auto data = test_data.get_data();
		array<double, OUTPUTS> temp = mnist->calculate(data.first);
		int answer = data.second;
		double max = DBL_MIN;
		double number = -1;
		for (size_t i = 0; i < OUTPUTS; i++)
		{
			if (temp[i] > max) {
				max = temp[i];
				number = i;
			}
		}
		if (number == data.second) {
			correct_count++;
		}
		cout << number << " " << answer << (number == answer ? "correct" : "incorrect") << endl;
	}
	cout << "CorrectRate " << correct_count / 10000.0;
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
