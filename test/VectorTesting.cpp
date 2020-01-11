#include <doctest/doctest.h>
#include <type_traits>

#include <VecN.h>

using namespace MathLib;

TEST_SUITE("Vec3 tests")
{
	TEST_CASE("Vec3 is POD type")
	{
		REQUIRE(std::is_pod<Vec3f>::value);
	}

	TEST_CASE("Vec3 equality operator works")
	{
		const auto lhs = Vec3f{ {1., 1., 1.} };
		const auto expected = std::array<double, 3>{ 1., 1., 1. };
		REQUIRE_EQ(lhs.m_data, expected);
	}

	TEST_CASE("Vec3 addition operator works")
	{
		auto lhs = Vec3f{ {1., 1., 1.} };
		auto rhs = Vec3f{ {1., 1., 1.} };
		const auto result = lhs + rhs;
		const auto expected = Vec3f{ 2., 2., 2. };
		CHECK(result == expected);
	}

	TEST_CASE("Vec3 subtraction operator works")
	{
		auto lhs = Vec3f{ {1., 1., 1.} };
		auto rhs = Vec3f{ {1., 1., 1.} };
		const auto result = lhs - rhs;
		const auto expected = Vec3f{ 0., 0., 0. };
		CHECK(result == expected);
	}

	TEST_CASE("Vec3 scalar multiplication works")
	{
		auto lhs = Vec3f{ {1., 1., 1.} };
		auto scalar = 2;
		const auto result = lhs * scalar;
		const auto expected = Vec3f{ 2., 2., 2. };
		CHECK(result == expected);
	}

	TEST_CASE("Vec3 vector multiplication operator works")
	{
		auto lhs = Vec3f{ {2., 3., 4.} };
		auto rhs = Vec3f{ {2., 2., 2.} };
		const auto result = lhs * rhs;
		const auto expected = Vec3f{ 4., 6., 8. };
		CHECK(result == expected);
	}

	TEST_CASE("Vec3 scalar division works")
	{
		auto lhs = Vec3f{ {1., 1., 1.} };
		auto scalar = 2;
		const auto result = lhs / scalar;
		const auto expected = Vec3f{ 0.5, 0.5, 0.5 };
		CHECK(result == expected);
	}

	TEST_CASE("Vec3 vector division operator works")
	{
		auto lhs = Vec3f{ {2., 3., 4.} };
		auto rhs = Vec3f{ {2., 2., 2.} };
		const auto result = lhs / rhs;
		const auto expected = Vec3f{ 1., 1.5, 2. };
		CHECK(result == expected);
	}

	TEST_CASE("Vec3 has Z")
	{
		auto vec3 = Vec3f{};
		REQUIRE_EQ(0., vec3.Z());
	}

	TEST_CASE("Vec4 has Z")
	{
		auto vec4 = Vec4f{};
		REQUIRE_EQ(0., vec4.Z());
	}

	//TEST_CASE("Vec2 doesnt have Z")
	//{
	//	auto vec2 = Vec2{};
	//	auto Z = vec2.Z();
	//}
	
}