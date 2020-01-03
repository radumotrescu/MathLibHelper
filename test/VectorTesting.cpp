#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <type_traits>

#include <Vec3.h>
#include <Vec2.h>

using namespace MathLib;

TEST_SUITE("Vec3 tests")
{

	TEST_CASE("Vec3 is POD type")
	{
		REQUIRE(std::is_pod<Vec3>::value);
	}


	TEST_CASE("Vec3 equality operator works")
	{
		const auto lhs = Vec3{ {1., 1., 1.} };
		const auto expected = std::array<double, 3>{ 1., 1., 1. };
		REQUIRE_EQ(lhs.data, expected);
	}

	TEST_CASE("Vec3 addition operator works")
	{
		auto lhs = Vec3{ {1., 1., 1.} };
		auto rhs = Vec3{ {1., 1., 1.} };
		const auto result = lhs + rhs;
		const auto expected = Vec3{ 2., 2., 2. };
		CHECK(result == expected);
	}

	TEST_CASE("Vec3 subtraction operator works")
	{
		auto lhs = Vec3{ {1., 1., 1.} };
		auto rhs = Vec3{ {1., 1., 1.} };
		const auto result = lhs - rhs;
		const auto expected = Vec3{ 0., 0., 0. };
		CHECK(result == expected);
	}
}


TEST_SUITE("Vec2 tests")
{

	TEST_CASE("Vec2 is POD type")
	{
		REQUIRE(std::is_pod<Vec2>::value);
	}


	TEST_CASE("Vec2 equality operator works")
	{
		const auto lhs = Vec2{ {1., 1.} };
		const auto expected = std::array<double, 2>{ 1., 1. };
		REQUIRE_EQ(lhs.data, expected);
	}

	TEST_CASE("Vec2 addition operator works")
	{
		auto lhs = Vec2{ {1., 1. } };
		auto rhs = Vec2{ {1., 1. } };
		const auto result = lhs + rhs;
		const auto expected = Vec2{ 2., 2. };
		CHECK(result == expected);
	}

	TEST_CASE("Vec2 subtraction operator works")
	{
		auto lhs = Vec2{ {1., 1. } };
		auto rhs = Vec2{ {1., 1. } };
		const auto result = lhs - rhs;
		const auto expected = Vec2{ 0., 0. };
		CHECK(result == expected);
	}
}
