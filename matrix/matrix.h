#pragma once
#include "pch.h"
#include <sstream>
#include <array>
#include <functional>

namespace nagisakuya {
	namespace Utility {
		template <typename _Ty,size_t _column,size_t _row> class matrix{
		private:
			std::array<std::array<_Ty, _row>, _column> content;
		public:
			static void foreach(std::function<void(size_t colmun, size_t row)> doeach, std::function<void(size_t colmun)>doeachendofcolumn = [](size_t colmun) {}) {
			for (size_t i = 0; i < _column; i++)
			{
				for (size_t j = 0; j < _row; j++)
				{
					doeach(i, j);
				}
				doeachendofcolumn(i);
			}
		}
			matrix() { foreach([&](size_t i, size_t j) {content[i][j] = _Ty(); }); };
			matrix(std::array<std::array<_Ty, _row>, _column> i) { content = i; }
			constexpr size_t row() const{ return _row; }
			constexpr size_t column() const{ return _column; }
			matrix<_Ty, _row, _column> transpose() const { matrix<_Ty, _row, _column>re; foreach([&](size_t i, size_t j) {re[j][i] = content[i][j]; }); return re; }
			std::array<_Ty, _row> const& operator [](size_t t) const { return content[t]; }
			std::array<_Ty, _row>& operator [](size_t t) { return content[t]; }
			matrix<_Ty, _column, _row>& operator +=(matrix<_Ty, _column, _row> const& m) {
				foreach([&](size_t i, size_t j) {content[i][j] = content[i][j] + m[i][j]; });
				return *this;
			}
			matrix<_Ty, _column, _row>& operator -=(matrix<_Ty, _column, _row> const& m) {
				foreach([&](size_t i, size_t j) {content[i][j] = content[i][j] - m[i][j]; });
				return *this;
			}
			matrix<_Ty, _column, _row>& operator *=(size_t i) {
				foreach([&](size_t i, size_t j) {content[i][j] *= i; });
				return *this;
			}
			template<typename _nTy>
			operator _nTy() const { matrix<_nTy, _column, _row> re; foreach([&](size_t i, size_t j) {re[i][j] = (_nTy)content[i][j]; }); return re; }
			std::string str() const { std::stringstream ss; foreach([&](size_t i, size_t j) {ss << content[i][j] << ",\t"; }, [&](size_t) { ss << "\n"; }); return ss.str(); }
		};
		template <typename _Ty, size_t _column, size_t _row>
		const matrix<_Ty, _column, _row> operator +(matrix<_Ty, _column ,_row> const& f,matrix<_Ty, _column, _row> const& s) {
			return matrix<_Ty, _column, _row>(f) += s;
		}
		template <typename _Ty, size_t _column, size_t _row>
		const matrix<_Ty, _column, _row> operator -(matrix<_Ty, _column, _row> const& f, matrix<_Ty, _column, _row> const& s) {
			return matrix<_Ty, _column, _row>(f) -= s;
		}
		template <typename _Ty, size_t _column, size_t _row_scolumn, size_t _srow>
		const matrix<_Ty, _column, _srow> operator *(matrix<_Ty, _column, _row_scolumn> const& f, matrix<_Ty, _row_scolumn, _srow> const& s) {
			matrix<_Ty, _column, _srow> re;
			matrix<_Ty, _column, _row_scolumn>::foreach([&](size_t i, size_t j) {
				for (size_t k = 0; k < _srow; k++) {
					re[i][k] += f[i][j] * s[j][k];//配列を横に動いてから縦に動くのでメモリアクセス的に高速なはず
				}
				});

			return re;
		}
		template <typename _Ty, size_t _column, size_t _row>
		const matrix<_Ty, _column, _row> operator *(size_t i, matrix<_Ty, _column, _row> const& m) {
			return matrix<_Ty, _column, _row>(m) *= i;
		}
		template <typename _Ty, size_t _column, size_t _row>
		const matrix<_Ty, _column, _row> operator *( matrix<_Ty, _column, _row> const& m , size_t i) {
			return m * i;
		}
	}
}
