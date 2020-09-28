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
		Deck::Deck(array<int, 10> input)
		{
			content = input;
		}
		void Deck::print() const{
			for (int i = 0; i < 14; i++)
			{
				cout << "number of " << Translate(i) << "=" << content[i] << endl;
			}
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

	}
}