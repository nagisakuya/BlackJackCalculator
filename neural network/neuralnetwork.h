#pragma once
#include <cmath>
#include "pch.h"
#include "../matrix/matrix.h"
#include <random>
#include <unordered_map>
#include <functional>

using namespace nagisakuya::Utility;

namespace nagisakuya {
	namespace neural {
		std::random_device seed_gen;
		std::default_random_engine engine(seed_gen());
		constexpr double sigmoid_range = 34.538776394910684;
		enum class Activator { sigmoid, ReLU , softmax};
		double sigmoid(double d) {
			if (d > sigmoid_range) d = sigmoid_range;
			if (d < -sigmoid_range) d = -sigmoid_range;
			return 1 / (1 + std::exp(-d));
		}
		double sigmoid_dif(double y) {
			return y * (1 - y);
		}
		double ReLU(double d) {
			return (d > 0) * d;
		}
		double ReLU_dif(double y) {
			return y > 0;
		}
		double softmax( double d) {

		}
		const std::unordered_map<Activator, std::pair<std::function<double(double)>, std::function<double(double)>>> activator_to_func = { {Activator::sigmoid,std::make_pair(*sigmoid , *sigmoid_dif)},{Activator::ReLU,std::make_pair(*ReLU,*ReLU_dif)} };
		void avoid_flow(double& d, double upper_range = 1.0e300, double under_range = 1.0e-300) {
			if (d < -upper_range) d = -upper_range;
			if (d > -under_range && d < under_range) d = 0;
			if (d > upper_range) d = upper_range;
		}
		template<size_t _Size>
		class Layer {
		private:
			matrix<double, _Size, 1> nodes;
		public:
			Layer() {};
			Layer(matrix<double, _Size, 1> m) { nodes = m; }
			operator matrix<double, _Size, 1>() { return nodes; }
			Layer(std::array<double, _Size> ar) {
				for (size_t i = 0; i < _Size; i++)
				{
					nodes[i][0] = ar[i];
				}
			}
			operator std::array<double, _Size>() {
				std::array<double, _Size> re;
				for (size_t i = 0; i < _Size; i++) {
					re[i] = nodes[i][0];
				}
				return re;
			}
			double& operator[](size_t i) { return nodes[i][0]; }
			void activate(Activator activator) {
				for (size_t i = 0; i < _Size; i++) {
					{
						nodes[i][0] = activator_to_func.at(activator).first(nodes[i][0]);
					}
				}
			}
		};
		template<size_t _In, size_t _Out>
		class Weight {
		private:
			matrix<double, _Out, _In> weight;
		public:
			Weight() {};
			Weight(matrix<double, _Out, _In> const& m) { weight = m; };
			void renew(Layer<_In> input, Layer<_Out> delta, double learning_rate) {
				weight.foreach([&](size_t i, size_t j) {weight[i][j] -= learning_rate * delta[i] * input[j];  avoid_flow(weight[i][j]); });
			}
			Weight<_Out, _In> transpose() { return weight.transpose(); };
			operator matrix<double, _Out, _In>() { return weight; }
			std::array<double, _In> operator[](size_t i) { return weight[i]; }
			void randamize(double range = 1.0) {
				std::uniform_real_distribution<> rnd(-range, range);
				weight.foreach([&](size_t i, size_t j) {weight[i][j] = rnd(engine); });
			}
			void distribution(double deviation = 1.0) {
				std::normal_distribution<> dist(0.0, 1.0);
				weight.foreach([&](size_t i, size_t j) {weight[i][j] = dist(engine); });
			}
		};
		template<size_t _In, size_t _Out>
		Layer<_Out> const operator*(Weight<_In, _Out> weight, Layer<_In> layer) { return (matrix<double, _Out, _In>)weight * (matrix<double, _In, 1>)layer; }
		template<size_t _In, size_t _Hid, size_t _Out>
		class ThreeLayersNeural {
		private:
			Activator hactivator;
			Activator oactivator;
			double learning_rate;
			Weight<_In, _Hid> input_to_hidden;
			Weight<_Hid, _Out> hidden_to_output;
			std::tuple<Layer<_In>, Layer<_Hid>, Layer<_Out>>  calc(Layer<_In> input) {
				Layer<_Hid> hidden = input_to_hidden * input;
				hidden.activate(hactivator);
				Layer<_Out> output = hidden_to_output * hidden;
				output.activate(oactivator);
				std::tuple<Layer<_In>, Layer<_Hid>, Layer<_Out>> re = std::make_tuple(input, hidden, output);
				return re;
			}
		public:
			explicit ThreeLayersNeural(double rate = 0.01, Activator hactivator = Activator::sigmoid, Activator oactivator = Activator::sigmoid) :learning_rate(rate), hactivator(hactivator), oactivator(oactivator) { input_to_hidden.distribution(); hidden_to_output.distribution(); };
			std::array<double, _Out> calculate(std::array<double, _In> in) {
				return std::get<2>(calc(in));
			}
			void study(std::array<double, _In> in, std::array<double, _Out> answer) {
				Layer<_In> input;
				Layer<_Hid> hidden;
				Layer<_Out> output;
				std::tie(input, hidden, output) = calc(in);
				Layer<_Out> delta_out;
				for (size_t i = 0; i < _Out; i++)
				{
					delta_out[i] = (output[i] - answer[i]) * activator_to_func.at(hactivator).second(output[i]);
				}
				hidden_to_output.renew(hidden, delta_out, learning_rate);
				Layer<_Hid> delta_hidden = hidden_to_output.transpose() * delta_out;
				for (size_t i = 0; i < _Out; i++)
				{
					delta_hidden[i] = (hidden[i] - delta_hidden[i]) * activator_to_func.at(oactivator).second(output[i]);
				}
				input_to_hidden.renew(input, delta_hidden, learning_rate);
			}
		};
	}
}