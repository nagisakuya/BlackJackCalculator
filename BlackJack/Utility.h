#pragma once
#include <fstream>
#include <string>
#include <iostream>

namespace nagisakuya {
	namespace BlackJack {
		class cfstream {
		private:
			std::ofstream& outputfile;
		public:
			cfstream(std::ofstream& file):outputfile(file){}
			template <typename T>
			cfstream& operator<<(const T& input) {
				std::cout << input;
				outputfile << input;
				return *this;
			}
			cfstream& operator<<(std::ostream& (*of)(std::ostream&)) {
				of(std::cout);
				of(outputfile);
				return *this;
			}
			template <typename T>
			cfstream& operator<=(const T& input) {
				outputfile << input;
				return *this;
			}
			cfstream& operator<=(std::ostream& (*of)(std::ostream&)) {
				of(outputfile);
				return *this;
			}
		};

	}
}