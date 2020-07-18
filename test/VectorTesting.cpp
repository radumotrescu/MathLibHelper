#include <doctest/doctest.h>
#include <type_traits>

#include <VecN.h>

using namespace MathLib;

TEST_SUITE("Vec3 tests")
{
    TEST_CASE("Vec3 equality operator works")
    {
        {
            const auto lhs = Vec3f{ {1., 1., 1.} };
            const auto expected = std::array<double, 3>{ 1., 1., 1. };
            REQUIRE_EQ(lhs._data, expected);
        }
        {
            const auto lhs = Vec3f{ {1.532, 1., 1.} };
            const auto expected = std::array<double, 3>{ 1.532, 1., 1. };
            REQUIRE_EQ(lhs._data, expected);
        }
        {
            const auto lhs = Vec3f{ {1.532, 1., 1.} };
            const auto expected = std::array<double, 3>{ 1.5325, 1., 1. };
            REQUIRE_NE(lhs._data, expected);
        }
        {
            const auto lhs = Vec3i{ {1, 1, 1} };
            const auto expected = std::array<int, 3>{ 1, 1, 1 };
            REQUIRE_EQ(lhs._data, expected);
        }
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

    TEST_CASE("Vec3 dot product works")
    {
        auto lhs = Vec3f{ {1., 3., -5.} };
        auto rhs = Vec3f{ {4., -2., -1.} };
        const auto result = lhs.Dot(rhs);
        REQUIRE_EQ(result, 3.);
    }

    TEST_CASE("Vec3 cross product works")
    {
        auto lhs = Vec3i{ {3, -3, 1} };
        auto rhs = Vec3i{ {4, 9, 2} };
        const auto result = lhs.Cross(rhs);
        REQUIRE_EQ(result, Vec3i{ {-15, -2, 39} });
    }

    TEST_CASE("Vector normalization works")
    {
        auto vector = Vec3i{ 3, 1, 2 };
        //REQUIRE_EQ(vector.Normalize(), 3.742);
    }
}