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
				r += Card(i).str() + ": " + to_string(count(Card(i))) + "\t";
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
		Card Deck::DrowRandom() {
			int temp = rand() % this->size();
			int sum = 0;
			for (size_t i = 0; i < 14; i++)
			{
				sum += content[i];
				if (sum >= temp) {
					Drow(i);//
					return Card(i);
				}
			}
			return 0;
		}

		Deck Deck::Drow(std::vector<Card> remlist)
		{
			for (size_t i = 0; i < remlist.size(); i++)
			{
				content[remlist[i]]--;
			}
			return *this;
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