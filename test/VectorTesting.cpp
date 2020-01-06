#include <doctest/doctest.h>
#include <type_traits>

#include <VecN.h>

using namespace MathLib;

TEST_SUITE("Vec3 tests")
{
	TEST_CASE("Vec3 is POD type")
	{
		REQUIRE(std::is_pod<VecN<1>>::value);
	}

	TEST_CASE("Vec3 equality operator works")
	{
		const auto lhs = VecN<3>{ {1., 1., 1.} };
		const auto expected = std::array<double, 3>{ 1., 1., 1. };
		REQUIRE_EQ(lhs.m_data, expected);
	}

	TEST_CASE("Vec3 addition operator works")
	{
		auto lhs = VecN<3>{ {1., 1., 1.} };
		auto rhs = VecN<3>{ {1., 1., 1.} };
		const auto result = lhs + rhs;
		const auto expected = VecN<3>{ 2., 2., 2. };
		CHECK(result == expected);
	}

	TEST_CASE("Vec3 subtraction operator works")
	{
		auto lhs = VecN<3>{ {1., 1., 1.} };
		auto rhs = VecN<3>{ {1., 1., 1.} };
		const auto result = lhs - rhs;
		const auto expected = VecN<3>{ 0., 0., 0. };
		CHECK(result == expected);
	}
}