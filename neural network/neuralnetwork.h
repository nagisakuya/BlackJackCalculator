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
		constexpr double double_range = 1.0e300;
		constexpr double double_under_range = 1.0e-300;
		void avoid_flow(double& d, double upper_range = double_range, double under_range = double_under_range) {
			if (d < -upper_range) d = -upper_range;
			else if (d > -under_range && d < under_range) d = 0;
			else if (d > upper_range) d = upper_range;
		}
		enum class Activator { sigmoid, ReLU, LReLU, exp, softmax };
		constexpr double sigmoid_range = 700;
		double sigmoid(double d) {
			if (d > sigmoid_range) d = sigmoid_range;
			else if (d < -sigmoid_range) d = -sigmoid_range;
			return 1.0 / (1.0 + std::exp(-d));
		}
		const double sigmoid_dif_range = sigmoid(sigmoid_range);
		double sigmoid_dif(double y) {
			//if (y > sigmoid_dif_range || y < -sigmoid_dif_range) return 0;
			//一度端に到達すると逆側に学習が進まなくなってしまう
			return y * (1 - y);
		}
		double ReLU(double d) {
			return (d > 0) * d;
		}
		double ReLU_dif(double y) {
			return y > 0;
		}
		constexpr double LReLU_alpha = 0.1;
		double LReLU(double d) {
			return d > 0 ? d : d * LReLU_alpha;
		}
		double LReLU_dif(double y) {
			return y > 0 ? 1 : LReLU_alpha;
		}
		double exp(double d) {
			return std::exp(d);
		}
		double exp_dif(double y) {
			return y;
		}
		const std::unordered_map<Activator, std::pair<std::function<double(double)>, std::function<double(double)>>> activator_to_func = {
			{Activator::sigmoid,std::make_pair(*sigmoid , *sigmoid_dif)},
			{Activator::ReLU,std::make_pair(*ReLU,*ReLU_dif)},
			{Activator::LReLU,std::make_pair(*LReLU,*LReLU_dif)},
			{Activator::exp,std::make_pair(*exp,*exp_dif)},
		};
		template<size_t _F, size_t _S>
		using layer_pair_data = std::pair < std::array<double, _F>, std::array<double, _S>>;
		template<size_t _F, size_t _S>
		using mini_batch = std::vector<layer_pair_data<_F, _S>>;
		template<size_t _In, size_t _Out> class Weight;
		template<size_t _Size>
		class Layer : public matrix<double, _Size, 1> {
		public:
			Layer() {};
			Layer(matrix<double, _Size, 1> const& matrix) { this->contents = matrix.get_contents(); }
			Layer(std::array<double, _Size> ar) {
				for (size_t i = 0; i < _Size; i++)
				{
					(*this)[i] = ar[i];
				}
			}
			operator std::array<double, _Size>() {
				std::array<double, _Size> re;
				for (size_t i = 0; i < _Size; i++) {
					re[i] = (*this)[i];
				}
				return re;
			}
			double& operator[](size_t i) { return this->contents[i][0]; }
			double const& operator[](size_t i) const { return this->contents[i][0]; }
			std::tuple<size_t, double> max() {
				size_t max_number = 0;
				double max_value = (*this)[0];
				for (size_t i = 1; i < _Size; i++)
				{
					if (max_value < (*this)[i])
					{
						max_number = i;
						max_value = (*this)[i];
					}
				}
				return std::tie(max_number, max_value);
			}
			void activate(Activator activator) {

				if (activator == Activator::softmax) {
					double max_value = std::get<1>(max());
					double sum = 0;
					for (size_t i = 0; i < _Size; i++) {
						{
							(*this)[i] = std::exp((*this)[i] - max_value);
							sum += (*this)[i];
						}
					}
					*this *= 1.0 / sum;
				}
				else {
					for (size_t i = 0; i < _Size; i++) {
						{
							(*this)[i] = activator_to_func.at(activator).first((*this)[i]);
						}
					}
				}
			}
			/// <summary>
			/// 出力層用 隠れ層用はWeightのものを使用
			/// </summary>
			/// <param name="teacher"></param>
			/// <param name="activator"></param>
			/// <returns></returns>
			Layer<_Size> calculate_delta(Layer<_Size> const& teacher, Activator activator) {
				Layer<_Size> delta;
				if (activator == Activator::softmax) {
					for (size_t i = 0; i < _Size; i++)
					{
						delta[i] = (*this)[i] - teacher[i];
					}
				}
				else {
					for (size_t i = 0; i < _Size; i++)
					{
						delta[i] = ((*this)[i] - teacher[i]) * activator_to_func.at(activator).second((*this)[i]);
					}
				}
				return delta;
			}
		};
		template<size_t _In, size_t _Out>
		class Weight : public matrix<double, _Out, _In> {
		private:
			double learning_rate;
			matrix<double, _Out, _In> velocity;
		public:
			Weight(double learning_rate) : learning_rate(learning_rate) {
				velocity.fill(0);
			};
			/*
			//この方法はメモリ的に不利
			matrix<double, _Out, _In>& calc_update_value(Layer<_In>const& input, Layer<_Out>const& delta) {
				matrix<double, _Out, _In> re;
				this->foreach([&](size_t i, size_t j) {re[i][j] = delta[i] * input[j]; });
				return re; 
			}
			void update(matrix<double, _Out, _In> const& update_value) {
				*this -= learning_rate * update_value;
			}*/
			void SGD(Layer<_In> const& input, Layer<_Out>const& delta) {
				//update(calc_update_value(input, delta));
				this->foreach([&](size_t i, size_t j) {this->contents[i][j] -= learning_rate * delta[i] * input[j];  avoid_flow(this->contents[i][j]); });
			}
			void mini_batch_SGD(mini_batch<_In,_Out> const & input_and_delta) {
				this->foreach([&](size_t i, size_t j) {
					double update_sum = 0;
					for (auto k = input_and_delta.begin(); k != input_and_delta.end(); k++)
					{
						update_sum += k->first[j] * k->second[i];
					}
					this->contents[i][j] -= learning_rate * update_sum / input_and_delta.size();
					avoid_flow(this->contents[i][j]); 
					});
			}
			static constexpr double momentum_beta = 0.75;
			void momentum(Layer<_In> const& input, Layer<_Out>const& delta) {
				this->foreach([&](size_t i, size_t j) {
					velocity[i][j] = momentum_beta * velocity[i][j] + (1 - momentum_beta) * delta[i] * input[j];
					this->contents[i][j] -= learning_rate * velocity[i][j];
					avoid_flow(this->contents[i][j]);
					});
			}
			std::array<double, _In> operator[](size_t i) { return this->contents[i]; }
			void randamize(double range = 1.0) {
				std::uniform_real_distribution<> rnd(-range, range);
				this->foreach([&](size_t i, size_t j) {this->contents[i][j] = rnd(engine); });
			}
			void distribution(double deviation = 1.0) {
				std::normal_distribution<> dist(0.0, 1.0);
				this->foreach([&](size_t i, size_t j) {this->contents[i][j] = dist(engine); });
			}
			/// <summary>
			/// 隠れ層用 出力層用はLayerのものを使用
			/// </summary>
			/// <param name="student_layer"></param>
			/// <param name="teacher_delta"></param>
			/// <param name="activator"></param>
			/// <returns></returns>
			Layer<_In> calculate_delta(Layer<_In> const& student_layer, Layer<_Out> const& teacher_delta, Activator activator) const {
				Layer<_In> delta = this->transpose() * teacher_delta;
				for (size_t i = 0; i < _In; i++)
				{
					delta[i] = delta[i] * activator_to_func.at(activator).second(student_layer[i]);
				}
				return delta;
			}
		};
		template<size_t _In, size_t _Hid, size_t _Out>
		class ThreeLayersNeural {
		private:
			Activator hactivator;
			Activator oactivator;
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
			explicit ThreeLayersNeural(double learning_rate = 0.001, Activator hactivator = Activator::sigmoid, Activator oactivator = Activator::sigmoid)
				: input_to_hidden(learning_rate), hidden_to_output(learning_rate), hactivator(hactivator), oactivator(oactivator) {
				input_to_hidden.distribution();
				hidden_to_output.distribution();
			};
			std::array<double, _Out> calculate(std::array<double, _In> in) {
				return std::get<2>(calc(in));
			}
			void SGD(layer_pair_data<_In , _Out> data) {
				Layer<_In> input;
				Layer<_Hid> hidden;
				Layer<_Out> output;
				std::tie(input, hidden, output) = calc(data.first);
				Layer<_Out> delta_out = output.calculate_delta(data.second, oactivator);
				Layer<_Hid> delta_hidden = hidden_to_output.calculate_delta(hidden, delta_out, hactivator);
				hidden_to_output.SGD(hidden, delta_out);
				input_to_hidden.SGD(input, delta_hidden);
			}
			void mini_batch_SGD(mini_batch<_In, _Out> batch) {
				mini_batch<_Hid, _Out> ho_output_delta;
				mini_batch<_In, _Hid> ih_output_delta;
				for (auto i = batch.begin(); i != batch.end(); i++)
				{
					Layer<_In> input;
					Layer<_Hid> hidden;
					Layer<_Out> output;
					std::tie(input, hidden, output) = calc(i->first);
					Layer<_Out> delta_out = output.calculate_delta(i->second, oactivator);
					Layer<_Hid> delta_hidden = hidden_to_output.calculate_delta(hidden, delta_out, hactivator);
					ho_output_delta.push_back(std::make_pair(hidden,delta_out));
					ih_output_delta.push_back(std::make_pair(input, delta_hidden));
				}
				hidden_to_output.mini_batch_SGD(ho_output_delta);
				input_to_hidden.mini_batch_SGD(ih_output_delta);
			}
		};
	}
}