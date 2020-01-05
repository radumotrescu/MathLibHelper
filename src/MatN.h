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

		void MatrixReset()
		{
			m_data = MatrixData(size, std::vector<double>(size, 0));
		}

		Matrix SimpleMulti(const Matrix& rhs) const
		{
			auto md = MatrixData(size, std::vector<double>(size, 0));
			#pragma omp parallel for
			for (auto rowIdx = 0; rowIdx < size; rowIdx++)
			{
				for (auto colIdx = 0; colIdx < size; colIdx++)
				{
					auto sum = 0.;
					for (auto idx = 0; idx < size; idx++)
						sum += m_data[rowIdx][idx] * rhs.m_data[idx][colIdx];
					md[rowIdx][colIdx] = sum;
				}
			}
			return md;
		}

		Matrix(const Matrix& other) = default;
		Matrix(Matrix&& other) = default;
		~Matrix() = default;

	public:
		MatrixData m_data;
		uint64_t m_size = size;
	};
}

#endif