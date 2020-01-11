#ifndef Vec3_h_include
#define Vec3_h_include

#include <array>
#include <type_traits>

namespace MathLib
{
	template <uint64_t size>
	struct VecN
	{
		VecN operator+(const VecN& rhs) const
		{
			auto data = std::array<double, size>();
			for (auto i = 0; i < size; i++)
				data[i] = m_data[i] + rhs.m_data[i];
			return { data };
		};

		VecN operator-(const VecN& rhs) const
		{
			auto data = std::array<double, size>();
			for (auto i = 0; i < size; i++)
				data[i] = m_data[i] - rhs.m_data[i];
			return { data };
		};

		VecN operator*(double scalar) const
		{
			auto data = std::array<double, size>();
			for (auto i = 0; i < size; i++)
				data[i] = m_data[i] * scalar;
			return { data };
		};

		VecN operator*(VecN rhs) const
		{
			auto data = std::array<double, size>();
			for (auto i = 0; i < size; i++)
				data[i] = m_data[i] * rhs.m_data[i];
			return { data };
		}

		VecN operator/(double scalar) const
		{
			auto data = std::array<double, size>();
			for (auto i = 0; i < size; i++)
				data[i] = m_data[i] / scalar;
			return { data };
		}

		VecN operator/(VecN rhs) const
		{
			auto data = std::array<double, size>();
			for (auto i = 0; i < size; i++)
				data[i] = m_data[i] / rhs.m_data[i];
			return { data };
		}

		bool operator==(const VecN& rhs) const
		{
			auto data = std::array<double, size>();
			for (auto i = 0; i < size; i++)
				if (rhs.m_data[i] != m_data[i])
					return false;
			return true;
		};

		template <typename = std::enable_if_t<size >= 3>>
		double Z()
		{
			return m_data[2];
		}

		std::array<double, size> m_data;
	};

	using Vec3 = VecN<3>;
	using Vec2 = VecN<2>;
	using Vec4 = VecN<4>;
}

#endif