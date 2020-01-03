#ifndef Vec2_h_include
#define Vec2_h_include

#include <array>

namespace MathLib
{

	struct Vec2
	{
		Vec2 operator+(const Vec2& rhs) const
		{
			return { rhs.data[0] + data[0], rhs.data[1] + data[1]};
		}

		Vec2 operator-(const Vec2& rhs) const
		{
			return { rhs.data[0] - data[0], rhs.data[1] - data[1] };
		}
		
		Vec2 operator*(int scalar) const
		{
			return { data[0] * scalar, data[1] * scalar };
		}

		bool operator==(const Vec2& rhs) const
		{
			return rhs.data[0] == data[0] && rhs.data[1] == data[1];
		}

		std::array<double, 2> data;
	};
}

#endif