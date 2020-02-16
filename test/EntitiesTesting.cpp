#include <doctest/doctest.h>

#include <Entities.h>
#include "TestUtils.h"

using namespace MathLib;

TEST_SUITE("Line2D transformations testing")
{
    TEST_CASE("X Scaling works")
    {
        auto line = Line2D({ 1., 1. }, { 2., 3. });
        line.scale({ 2., 1. });
        auto p1 = line.m_data[0];
        auto p2 = line.m_data[1];
        REQUIRE_EQ(p1, Vec2f{ 2., 1. });
        REQUIRE_EQ(p2, Vec2f{ 4., 3. });
    }

    TEST_CASE("Y Scaling works")
    {
        auto line = Line2D({ 1., 1. }, { 2., 3. });
        line.scale({ 1., 2. });
        auto p1 = line.m_data[0];
        auto p2 = line.m_data[1];
        REQUIRE_EQ(p1, Vec2f{ 1., 2. });
        REQUIRE_EQ(p2, Vec2f{ 2., 6. });
    }

    TEST_CASE("XY Translating works")
    {
        auto line = Line2D({ 1., 1. }, { 2., 3. });
        line.translate({ 2.5, 3.3 });
        auto p1 = line.m_data[0];
        auto p2 = line.m_data[1];
        REQUIRE_EQ(p1, Vec2f{ 3.5, 4.3 });
        REQUIRE_EQ(p2, Vec2f{ 4.5, 6.3 });
    }

    TEST_CASE("Rotation works")
    {
        auto line = Line2D({ 0., 0. }, { 1., 0. });
        line.rotate(90);
        auto p1 = line.m_data[0];
        auto p2 = line.m_data[1];
        REQUIRE_EQ(p1, Vec2f{ 0., 0. });
        REQUIRE_EQ(p2, Vec2f{ 0., 1. });
    }
}
