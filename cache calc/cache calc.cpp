#include <iostream>

long long int func(int card_number, int d_sum,int p_sum) {
	long long int r = 0;
	for (size_t i = 0; (signed)i <= (17 + card_number - 1 - d_sum) / card_number ; i++)
	{
		for (size_t j = 0; (signed)j <= (21 + card_number - 1 - p_sum) / card_number; j++)
		{
			if (card_number == 10) {
				r += 1;
				
			}
			else {
				r += func(card_number + 1, d_sum + card_number * i, p_sum + card_number * j);
			}
		}
		
	}
	return r;
}

int main()
{
	const int size_double = 8;
	const int size_int = 4;
	long long int i = func(1, 0, 0);
	std::cout << i << " way" << std::endl;
	std::cout << i * (8 * size_double + size_int * 10) << " byte" << std::endl;
	std::cout << i * (8 * size_double + size_int * 10) / std::pow(2, 30) << " GB" << std::endl;
}
