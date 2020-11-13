#include "pch.h"
#include "BlackJack.h"

using namespace std;
namespace nagisakuya {
	namespace BlackJack {
		Deck::Deck(int NumberofDeck) {
			for (size_t i = 0; i < 9; i++)
			{
				content[i] = NumberofDeck * 4;
			}
			content[9] = NumberofDeck * 16;
		}
		string Deck::print() const{
			string r;
			for (int i = 0; i < 10; i++)
			{
				r += Translate(i) + ": " + to_string(content[i]) + "\t";
			}
			return r;
		}
		int Deck::size() const {
			int sum = 0;
			for (int i : content)
			{
				sum += i;
			}
			return sum;
		}
		int Deck::DrowRandom() {
			int temp = rand() % this->size();
			int sum = 0;
			for (size_t i = 0; i < 14; i++)
			{
				sum += content[i];
				if (sum >= temp) {
					content[i]--;
					return i;
				}
			}
			return 0;
		}

		void Deck::import(istream& input)
		{
			string temp_s;
			for (size_t i = 0; i < 10; i++)
			{
				input >> temp_s;
				input >> content[i];
			}
		}

	}
}