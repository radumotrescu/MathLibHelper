#ifndef Vec3_h_include
#define Vec3_h_include

#include <array>

namespace MathLib
{
	template <uint64_t size>
	struct VecN
	{
		VecN operator+(const VecN& rhs) const
		{
			auto data = std::array<double, size>();
			for (auto i = 0; i < size; i++)
				data[i] = rhs.m_data[i] + m_data[i];
			return { data };
		}

		VecN operator-(const VecN& rhs) const
		{
			auto data = std::array<double, size>();
			for (auto i = 0; i < size; i++)
				data[i] = rhs.m_data[i] - m_data[i];
			return { data };
		}

		VecN operator*(int scalar) const
		{
			auto data = std::array<double, size>();
			for (auto i = 0; i < size; i++)
				data[i] = rhs.m_data[i] * m_data[i];
			return { data };
		}

		bool operator==(const VecN& rhs) const
		{
			auto data = std::array<double, size>();
			for (auto i = 0; i < size; i++)
				if (rhs.m_data[i] != m_data[i])
					return false;
			return true;
		}

		std::array<double, size> m_data;
	};
}

#endif