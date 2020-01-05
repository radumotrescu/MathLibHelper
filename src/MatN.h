#ifndef MatN_h_include
#define MatN_h_include

#include <vector>

namespace MathLib
{
	template <int size>
	class Matrix
	{
	public:
		Matrix()
		{
			m_data = std::vector<std::vector<double>>(size, std::vector<double>(size, 0));
		}
		//Matrix(const Matrix& other);
		//void MatrixReset();

	public:
		std::vector<std::vector<double>> m_data;
	};

}

#endif