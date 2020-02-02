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

		template <uint64_t rowSize_, uint64_t colSize_>
		Matrix SimpleMulti(const Matrix<rowSize_, colSize_>& rhs) const
		{
			if (!CanBeMultiplied(rhs))
				return {};

			auto md = MatrixData(rowSize, std::vector<double>(colSize_, 0));
#pragma omp parallel for
			if (colSize_ != colSize || rowSize != rowSize_)
			{
				for (auto rowIdx = 0; rowIdx < rowSize; rowIdx++)
				{
					for (auto colIdx = 0; colIdx < colSize_; colIdx++)
					{
						auto sum = 0.;
						for (auto idx = 0; idx < colSize; idx++)
							sum += m_data[rowIdx][idx] * rhs.m_data[idx][colIdx];
						md[rowIdx][colIdx] = sum;
					}
				}
			}
			else
			{
				const auto rowMajor = ConvertToRowMajor();
				const auto colMajor = rhs.ConvertToColMajor();
				for (auto rowIdx = 0; rowIdx < rowSize; rowIdx++)
				{
					for (auto colIdx = 0; colIdx < colSize_; colIdx++)
					{
						auto sum = 0.;
						for (auto idx = 0; idx < rowSize_; idx++)
							sum += rowMajor[rowIdx * rowSize + idx] * colMajor[colIdx * colSize_ + idx];
						md[rowIdx][colIdx] = sum;
					}
				}
			}
			return { md };
		}

		Matrix Addition(const Matrix& rhs) const
		{
			if (!SizesAreEqual(rhs))
				return {};

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
			if (!SizesAreEqual(rhs))
				return {};
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

		template <uint64_t rowSize_, uint64_t colSize_>
		Matrix operator*(const Matrix<rowSize_, colSize_>& rhs) const
		{
			return SimpleMulti(rhs);
		};

		std::vector<double> ConvertToRowMajor() const
		{
			auto result = std::vector<double>();
			result.reserve(rowSize * colSize);
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
			result.reserve(rowSize * colSize);
			for (auto colIdx = 0; colIdx < colSize; colIdx++)
			{
				for (auto rowIdx = 0; rowIdx < rowSize; rowIdx++)
				{
					result.push_back(m_data[rowIdx][colIdx]);
				}
			}
			return result;
		}
		bool operator==(const Matrix& rhs) const
		{
			if (!SizesAreEqual())
				return false;
			for (auto rowIdx = 0; rowIdx < rowSize; rowIdx++)
			{
				for (auto colIdx = 0; colIdx < colSize; colIdx++)
				{
					if (m_data[rowIdx][colIdx] == rhs.m_data[rowIdx][colIdx])
						return false;
				}
			}
			return true;
		}

		Matrix(const Matrix& other) = default;
		Matrix(Matrix&& other) = default;
		~Matrix() = default;

		MatrixData m_data;
	private:
		template <uint64_t rowSize_, uint64_t colSize_>
		bool CanBeMultiplied(const Matrix<rowSize_, colSize_>& rhs) const
		{
			return rowSize == colSize_;
		}

		bool SizesAreEqual(const Matrix& rhs) const
		{
			return m_rowSize == rhs.m_rowSize && m_colSize == rhs.m_colSize;
		}

		const uint64_t m_rowSize = rowSize;
		const uint64_t m_colSize = colSize;
	};

	using Mat2 = Matrix<2, 2>;
	using Mat3 = Matrix<3, 3>;
	using Mat4 = Matrix<4, 4>;
}

#endif