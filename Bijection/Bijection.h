#pragma once
#include "pch.h"
#include <unordered_map>


namespace nagisakuya {
	namespace Utility {
		template <typename F, typename S> class bijection
		{
		private:
			typename std::unordered_map<F, const void*> first_element;
			typename std::unordered_map<S, const void*> second_element;
		public:
			class Iterator {
			private:
				typename std::unordered_map<F, const void*>::iterator ite;
			public:
				Iterator(typename std::unordered_map<F, const void*>::iterator i) { ite = i; }
				F first() const { return ite->first; }
				S second() const { return *(S*)(ite->second); }
				Iterator& operator++() { ite++; }
				bool operator==(const Iterator& i) { return ite == i.ite; }
			};
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
			S at(F first) const {
				return *(S*)first_element.at(first);
			}
			F at_reverse(S second) const {
				return *(F*)second_element.at(second);
			}
			size_t count(F first) const {
				return first_element.count(first);
			}
			size_t count_reverse(S second) const {
				return second_element.count(second);
			}
			bijection operator =(std::initializer_list<typename std::pair < F, S >> input) {
				return bijection(input);
			}
			Iterator begin() { return Iterator(first_element.begin()); }
			Iterator end() { return Iterator(first_element.end()); }
		};
	}
}