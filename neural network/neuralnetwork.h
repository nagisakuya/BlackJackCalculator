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
		template<size_t nodecount>
		class Layer {
		private:
			static constexpr double sigmoid_range = 34.538776394910684;
			matrix<double, nodecount, 1> nodes;
		public:
			Layer() {};
			Layer(matrix<double, nodecount, 1> m) { nodes = m; }
			operator matrix<double, nodecount, 1>() { return nodes; }
			Layer(std::array<double, nodecount> ar) { matrix<double, nodecount, 1>::foreach([&](size_t i, size_t j) {nodes[i][0] = ar[i]; }); }
			operator std::array<double, nodecount>() { std::array<double, nodecount> re; matrix<double, nodecount, 1>::foreach([&](size_t i, size_t j) {re[i] = nodes[i][0]; }); return re; }
			double& operator[](size_t i) { return nodes[i][0]; }
			Layer activation() { Layer<nodecount> re; matrix<double, nodecount, 1>::foreach([&](size_t i, size_t j) {re[i] = sigmoid(nodes[i][0]); }); return re; }
			static double sigmoid(double d) { 
				if (d > sigmoid_range)return 1;
				if (d < -sigmoid_range)return 0;
				return 1 / (1 + std::exp(d)); }
		};
		template<size_t inputnodecount, size_t outputnodecount>
		class Weight {
		private:
			matrix<double, outputnodecount, inputnodecount> weight;
		public:
			Weight() {};
			Weight(matrix<double, outputnodecount, inputnodecount> const& m) { weight = m; };
			void renew(Layer<inputnodecount> input,Layer<outputnodecount> delta,double learning_rate){
				matrix<double, outputnodecount, inputnodecount>::foreach([&](size_t i, size_t j) {weight[i][j] += learning_rate * input[j] * delta[i]; });
			}
			Weight<outputnodecount,inputnodecount> transpose() { return weight.transpose(); };
			operator matrix<double, outputnodecount, inputnodecount>() { return weight; }
			std::array<double, inputnodecount> operator[](size_t i) { return weight[i]; }
			void randamize() {
				std::uniform_real_distribution<> rnd(0, 1.0);
				matrix<double, outputnodecount, inputnodecount>::foreach([&](size_t i, size_t j) {weight[i][j] = rnd(engine); });
			};
		};
		template<size_t inputnodecount, size_t outputnodecount>
		Layer<outputnodecount> const operator*(Weight<inputnodecount, outputnodecount> weight, Layer<inputnodecount> layer) { return (matrix<double, outputnodecount, inputnodecount>)weight * (matrix<double, inputnodecount, 1>)layer; }
		template<size_t inputnodecount, size_t hiddennodecount, size_t outputnodecount>
		class NeuralNetwork {
		private:
			double learning_rate;
			Layer<inputnodecount> input;
			Layer<hiddennodecount> hidden_in;
			Layer<hiddennodecount> hidden_out;
			Layer<outputnodecount> output_in;
			Layer<outputnodecount> output_out;
			Weight<inputnodecount, hiddennodecount> input_to_hidden;
			Weight<hiddennodecount, outputnodecount> hidden_to_output;
		public:
			explicit NeuralNetwork(double rate = 0.3):learning_rate(rate){ input_to_hidden.randamize(); hidden_to_output.randamize(); };
			std::array<double, outputnodecount> calculate(std::array<double, inputnodecount> in) {
				input = in;
				hidden_in = input_to_hidden * input;
				hidden_out = hidden_in.activation();
				output_in = hidden_to_output * hidden_out;
				output_out = output_in.activation();
				return output_out;
			}
			void study(std::array<double, inputnodecount> in, std::array<double, outputnodecount> answer) {
				calculate(in);
				Layer<outputnodecount> delta_out;
				for (size_t i = 0; i < outputnodecount; i++)
				{
					delta_out[i] = (output_out[i] - answer[i]) * output_in[i] * (1 - output_in[i]);
				}
				hidden_to_output.renew(hidden_out, delta_out, learning_rate);
				Layer<hiddennodecount> delta_hidden = hidden_to_output.transpose() * delta_out;
				for (size_t i = 0; i < hiddennodecount; i++)
				{
					delta_hidden[i] *= hidden_in[i] * (1 - hidden_in[i]);
				}
				input_to_hidden.renew(input, delta_hidden, learning_rate);
			}
		};
	}
}