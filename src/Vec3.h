#ifndef Vec3_h_include
#define Vec3_h_include

#include <array>

namespace MathLib
{
	struct Vec3
	{
		Vec3 operator+(const Vec3& rhs) const
		{
			return { rhs.data[0] + data[0], rhs.data[1] + data[1], rhs.data[2] + data[2] };
		}

		Vec3 operator-(const Vec3& rhs) const
		{
			return { rhs.data[0] - data[0], rhs.data[1] - data[1], rhs.data[2] - data[2] };
		}
		
		Vec3 operator*(int scalar) const
		{
			return { data[0] * scalar, data[1] * scalar, data[2] *scalar };
		}

		bool operator==(const Vec3& rhs) const
		{
			return rhs.data[0] == data[0] && rhs.data[1] == data[1] && rhs.data[2] == data[2];
		}

		std::array<double, 3> data;
	};
}

#endif