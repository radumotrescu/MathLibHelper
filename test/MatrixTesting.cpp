#include <doctest/doctest.h>

#include <MatN.h>

using namespace MathLib;

TEST_SUITE("MatN tests")
{
	TEST_CASE("simple")
	{
		const auto mat = Matrix<3>();
		const auto d = mat.m_data.size();
		REQUIRE_EQ(d, 3);
	}
}
