#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_map>

namespace nagisakuya {
	namespace Utilty {
		class cfstream {
		private:
			std::ofstream fout;
		public:
			cfstream(std::string const& path, std::ios_base::openmode const mode = std::ios_base::out) {
				open(path, mode);
			}
			cfstream() {};
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
		template <typename F, typename S>
		class bijection
		{
		private:
			typename std::unordered_map<F, const void*> first_element;
			typename std::unordered_map<S, const void*> second_element;
		public:
			/*class Iterator {
			private:
				std::unordered_map<F, const void*>::iterator ite;
			public:
				Iterator(std::unordered_map<F, const void*>::iterator i) { ite = i; }
				F first() const { return ite->first; }
				S second() const { return *(S*)(ite->second); }
				Iterator& operator++() { ite++; }
				bool operator==(const Iterator& i) { return ite == i.ite; }
			};*/
			bijection() {};
			bijection(std::initializer_list<typename std::pair < F, S >> input) {
				add(input);
			}
			void add(std::initializer_list<typename std::pair < F, S >> input) {
				for (auto i : input)
				{
					emplace(i.first, i.second);
				}
			}
			void add(typename std::pair<F, S> input) {
				emplace(input.first, input.second);
			}
			void emplace(F first, S second) {
				first_element[first];
				second_element[second] = &(first_element.find(first)->first);
				first_element[first] = &(second_element.find(second)->first);
			}
			size_t size() const {
				return first_element.size();
			}
			S at_ftos(F first) const {
				return *(S*)first_element.at(first);
			}
			F at_stof(S second) const {
				return *(F*)second_element.at(second);
			}
			bijection operator =(std::initializer_list<typename std::pair < F, S >> input) {
				return bijection(input);
			}
			//Iterator begin() { return Iterator(first_element.begin()); }
			//Iterator end() { return Iterator(first_element.end()); }
		};
	}
}