#ifndef MatN_h_include
#define MatN_h_include

#include <vector>
#include <algorithm>

namespace MathLib
{
	using MatrixData = std::vector<std::vector<double>>;

	template <uint64_t rowSize, uint64_t colSize>
	class Matrix
	{
	public:
		Matrix()
		{
			m_data = MatrixData(rowSize, std::vector<double>(colSize, 0));
		};

		Matrix(const MatrixData& init_data)
		{
			m_data = init_data;
		}

		MatrixData GetData() const
		{
			return m_data;
		}

		void MatrixReset()
		{
			m_data = MatrixData(rowSize, std::vector<double>(colSize, 0));
		}

		Matrix SimpleMulti(const Matrix& rhs) const
		{
			auto md = MatrixData(rowSize, std::vector<double>(colSize, 0));
			const auto rowMajor = ConvertToRowMajor();
#pragma omp parallel for
			for (auto rowIdx = 0; rowIdx < rowSize; rowIdx++)
			{
				const auto colMajor = rhs.ConvertToColMajor();
				for (auto colIdx = 0; colIdx < colSize; colIdx++)
				{
					auto sum = 0.;
					for (auto idx = 0; idx < rowSize; idx++)
						sum += rowMajor[rowIdx * rowSize + idx] * colMajor[colIdx * colSize + idx];
					md[rowIdx][colIdx] = sum;
				}
			}
			return { md };
		}

		Matrix Addition(const Matrix& rhs) const
		{
			auto md = MatrixData(rowSize, std::vector<double>(colSize, 0));
#pragma omp parallel for
			for (auto rowIdx = 0; rowIdx < rowSize; rowIdx++)
			{
				for (auto colIdx = 0; colIdx < colSize; colIdx++)
				{
					md[rowIdx][colIdx] = m_data[rowIdx][colIdx] + rhs.m_data[rowIdx][colIdx];
				}
			}
			return { md };
		}

		Matrix Subtraction(const Matrix& rhs) const
		{
			auto md = MatrixData(rowSize, std::vector<double>(colSize, 0));
#pragma omp parallel for
			for (auto rowIdx = 0; rowIdx < rowSize; rowIdx++)
			{
				for (auto colIdx = 0; colIdx < colSize; colIdx++)
				{
					md[rowIdx][colIdx] = m_data[rowIdx][colIdx] - rhs.m_data[rowIdx][colIdx];
				}
			}
			return { md };
		}

		Matrix operator+(const Matrix& rhs) const
		{
			return Addition(rhs);
		};

		Matrix operator-(const Matrix& rhs) const
		{
			return Subtraction(rhs);
		};

		Matrix operator*(const Matrix& rhs) const
		{
			return SimpleMulti(rhs);
		};

		std::vector<double> ConvertToRowMajor() const
		{
			auto result = std::vector<double>();
			result.reserve(rowSize * rowSize);
			for (auto rowIdx = 0; rowIdx < rowSize; rowIdx++)
			{
				for (auto colIdx = 0; colIdx < colSize; colIdx++)
				{
					result.push_back(m_data[rowIdx][colIdx]);
				}
			}
			return result;
		}

		std::vector<double> ConvertToColMajor() const
		{
			auto result = std::vector<double>();
			result.reserve(rowSize * rowSize);
			for (auto rowIdx = 0; rowIdx < rowSize; rowIdx++)
			{
				for (auto colIdx = 0; colIdx < colSize; colIdx++)
				{
					result.push_back(m_data[colIdx][rowIdx]);
				}
			}
			return result;
		}

		Matrix(const Matrix& other) = default;
		Matrix(Matrix&& other) = default;
		~Matrix() = default;

	private:
		MatrixData m_data;
		uint64_t m_rowSize = rowSize;
		uint64_t m_colSize = colSize;
	};

	using Mat2 = Matrix<2, 2>;
	using Mat3 = Matrix<3, 3>;
	using Mat4 = Matrix<4, 4>;
}

#endif