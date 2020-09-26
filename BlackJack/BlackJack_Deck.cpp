#include "BlackJack.h"
using namespace std;
namespace nagisakuya {
	namespace BlackJack {
		Deck::Deck(int NumberofDeck, int Joker) {
			for (size_t i = 0; i < 13; i++)
			{
				content[i] = 4 * NumberofDeck;
			}
			content[13] = Joker;
			first_size = size();
		}

		void Deck::print() {
			for (int i = 0; i < 14; i++)
			{
				cout << "number of " << Translate(i) << "=" << content[i] << endl;
			}
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

		int Deck::size() {
			int sum = 0;
			for (int i : content)
			{
				sum += i;
			}
			return sum;
		}
	}
}