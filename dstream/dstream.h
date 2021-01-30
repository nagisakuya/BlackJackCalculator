#pragma once
#include "pch.h"
#include <fstream>
#include <iostream>

namespace nagisakuya {
	namespace Utility {
		class cfstream {
		private:
			std::ofstream fout;
		public:
			cfstream(std::string const& path, std::ios_base::openmode const mode = std::ios_base::out) {
				open(path, mode);
			}
			cfstream() {

			}
			~cfstream() {
				close();
			}
			void open(std::string const& path, std::ios_base::openmode const mode = std::ios_base::out) {
				fout.open(path, mode);
			}
			void close() {
				fout.close();
			}
			template <typename T>
			cfstream& operator<<(const T& input) {
				std::cout << input;
				fout << input;
				return *this;
			}
			cfstream& operator<<(std::ostream& (*of)(std::ostream&)) {
				of(std::cout);
				of(fout);
				return *this;
			}
			std::ofstream& fonly() {
				return fout;
			}
		};
	}
}