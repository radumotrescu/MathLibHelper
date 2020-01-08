#ifndef MatN_h_include
#define MatN_h_include

#include <vector>
#include <algorithm>

namespace MathLib
{
	using MatrixData = std::vector<std::vector<double>>;

	template <uint64_t size>
	class Matrix
	{
	public:
		Matrix()
		{
			m_data = MatrixData(size, std::vector<double>(size, 0));
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
			m_data = MatrixData(size, std::vector<double>(size, 0));
		}

		Matrix SimpleMulti(const Matrix& rhs) const
		{
			auto md = MatrixData(size, std::vector<double>(size, 0));
			const auto rowMajor = ConvertToRowMajor();
#pragma omp parallel for
			for (auto rowIdx = 0; rowIdx < size; rowIdx++)
			{
				const auto colMajor = rhs.ConvertToColMajor();
				for (auto colIdx = 0; colIdx < size; colIdx++)
				{
					auto sum = 0.;
					for (auto idx = 0; idx < size; idx++)
						sum += rowMajor[rowIdx * size + idx] * colMajor[colIdx * size + idx];
					md[rowIdx][colIdx] = sum;
				}
			}
			return { md };
		}

		Matrix Addition(const Matrix& rhs) const
		{
			auto md = MatrixData(size, std::vector<double>(size, 0));
#pragma omp parallel for
			for (auto rowIdx = 0; rowIdx < size; rowIdx++)
			{
				for (auto colIdx = 0; colIdx < size; colIdx++)
				{
					md[rowIdx][colIdx] = m_data[rowIdx][colIdx] + rhs.m_data[rowIdx][colIdx];
				}
			}
			return { md };
		}

		Matrix Subtraction(const Matrix& rhs) const
		{
			auto md = MatrixData(size, std::vector<double>(size, 0));
#pragma omp parallel for
			for (auto rowIdx = 0; rowIdx < size; rowIdx++)
			{
				for (auto colIdx = 0; colIdx < size; colIdx++)
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
			result.reserve(size * size);
			for (auto rowIdx = 0; rowIdx < size; rowIdx++)
			{
				for (auto colIdx = 0; colIdx < size; colIdx++)
				{
					result.push_back(m_data[rowIdx][colIdx]);
				}
			}
			return result;
		}

		std::vector<double> ConvertToColMajor() const
		{
			auto result = std::vector<double>();
			result.reserve(size * size);
			for (auto rowIdx = 0; rowIdx < size; rowIdx++)
			{
				for (auto colIdx = 0; colIdx < size; colIdx++)
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
		uint64_t m_size = size;
	};
}

#endif