#pragma once
#include <cmath>
#include "pch.h"
#include "../matrix/matrix.h"
#include <random>

using namespace nagisakuya::Utility;

namespace nagisakuya {
	namespace neural {
		std::random_device seed_gen;
		std::default_random_engine engine(seed_gen());
		constexpr double sigmoid_range = 34.538776394910684;
		double sigmoid(double d) {
			if (d > sigmoid_range) d = sigmoid_range;
			if (d < -sigmoid_range) d = -sigmoid_range;
			return 1 / (1 + std::exp(-d));
		}
		void avoid_flow(double& d , double upper_range = 1.0e300,double under_range = 1.0e-300) {
			if (d < -upper_range) d = -upper_range;
			if (d > -under_range && d < under_range) d = 0;
			if (d > upper_range) d = upper_range;
		}
		template<size_t nodecount>
		class Layer {
		private:
			matrix<double, nodecount, 1> nodes;
		public:
			Layer() {};
			Layer(matrix<double, nodecount, 1> m) { nodes = m; }
			operator matrix<double, nodecount, 1>() { return nodes; }
			Layer(std::array<double, nodecount> ar) {
				for (size_t i = 0; i < nodecount; i++)
				{
					nodes[i][0] = ar[i];
				}
			}
			operator std::array<double, nodecount>() {
				std::array<double, nodecount> re;
				for (size_t i = 0; i < nodecount; i++) {
					re[i] = nodes[i][0];
				}
				return re;
			}
			double& operator[](size_t i) { return nodes[i][0]; }
			void activation() {
				for (size_t i = 0; i < nodecount; i++)
				{
					nodes[i][0] = sigmoid(nodes[i][0]);
				}
			}
		};
		template<size_t inputnodecount, size_t outputnodecount>
		class Weight {
		private:
			matrix<double, outputnodecount, inputnodecount> weight;
		public:
			Weight() {};
			Weight(matrix<double, outputnodecount, inputnodecount> const& m) { weight = m; };
			void renew(Layer<inputnodecount> input, Layer<outputnodecount> delta, double learning_rate) {
				weight.foreach([&](size_t i, size_t j) {weight[i][j] -= learning_rate * delta[i] * input[j];  avoid_flow(weight[i][j]); });
			}
			Weight<outputnodecount, inputnodecount> transpose() { return weight.transpose(); };
			operator matrix<double, outputnodecount, inputnodecount>() { return weight; }
			std::array<double, inputnodecount> operator[](size_t i) { return weight[i]; }
			void randamize(double range = 1.0) {
				std::uniform_real_distribution<> rnd(-range, range);
				weight.foreach([&](size_t i, size_t j) {weight[i][j] = rnd(engine); });
			}
			void distribution(double deviation = 1.0) {
				std::normal_distribution<> dist(0.0, 1.0);
				weight.foreach([&](size_t i, size_t j) {weight[i][j] = dist(engine); });
			}
		};
		template<size_t inputnodecount, size_t outputnodecount>
		Layer<outputnodecount> const operator*(Weight<inputnodecount, outputnodecount> weight, Layer<inputnodecount> layer) { return (matrix<double, outputnodecount, inputnodecount>)weight * (matrix<double, inputnodecount, 1>)layer; }
		template<size_t inputnodecount, size_t hiddennodecount, size_t outputnodecount>
		class NeuralNetwork {
		private:
			double learning_rate;
			Weight<inputnodecount, hiddennodecount> input_to_hidden;
			Weight<hiddennodecount, outputnodecount> hidden_to_output;
			std::tuple<Layer<inputnodecount>, Layer<hiddennodecount>, Layer<outputnodecount>>  calc(Layer<inputnodecount> input) {
				Layer<hiddennodecount> hidden = input_to_hidden * input;
				hidden.activation();
				Layer<outputnodecount> output = hidden_to_output * hidden;
				output.activation();
				std::tuple<Layer<inputnodecount>, Layer<hiddennodecount>, Layer<outputnodecount>> re = std::make_tuple(input, hidden, output);
				return re;
			}
		public:
			explicit NeuralNetwork(double rate = 0.3) :learning_rate(rate) { input_to_hidden.distribution(); hidden_to_output.distribution(); };
			std::array<double, outputnodecount> calculate(std::array<double, inputnodecount> in) {
				return std::get<2>(calc(in));
			}
			void study(std::array<double, inputnodecount> in, std::array<double, outputnodecount> answer) {
				Layer<inputnodecount> input;
				Layer<hiddennodecount> hidden;
				Layer<outputnodecount> output;
				std::tie(input, hidden, output) = calc(in);
				Layer<outputnodecount> delta_out;
				for (size_t i = 0; i < outputnodecount; i++)
				{
					delta_out[i] = (output[i] - answer[i]) * output[i] * (1.0 - output[i]);
				}
				hidden_to_output.renew(hidden, delta_out, learning_rate);
				Layer<hiddennodecount> delta_hidden = hidden_to_output.transpose() * delta_out;
				input_to_hidden.renew(input, delta_hidden, learning_rate);
			}
		};
	}
}