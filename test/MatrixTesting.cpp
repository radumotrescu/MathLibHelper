#include <doctest/doctest.h>

#include <MatN.h>

using namespace MathLib;

TEST_SUITE("MatN tests")
{
	TEST_CASE("Matrix row size size is set correctly")
	{
		const auto mat = Matrix<3>();
		const auto rowSize = mat.m_data.size();
		REQUIRE_EQ(rowSize, 3);
	}

	TEST_CASE("Matrix column size size is set correctly")
	{
		const auto mat = Matrix<3>();
		const auto columnSize = mat.m_data[0].size();
		REQUIRE_EQ(columnSize, 3);
	}

	TEST_CASE("Matrix simple multiplication")
	{
		const auto mat = Matrix<2>({ {2., 2.}, 
									  {2., 2.} });

		const auto identity = Matrix<2>({ {1., 0.},
									  {0., 1.} });
		const auto result = mat.SimpleMulti(identity);
		REQUIRE_EQ(result.m_data[0], std::vector<double>{2., 2.});
		REQUIRE_EQ(result.m_data[1], std::vector<double>{2., 2.});
	}

	TEST_CASE("Matrix simple addition")
	{
		const auto mat = Matrix<2>({ {2., 2.}, 
									  {2., 2.} });

		const auto identity = Matrix<2>({ {1., 0.},
									  {0., 1.} });
		const auto result = mat.Addition(identity);
		REQUIRE_EQ(result.m_data[0], std::vector<double>{3., 2.});
		REQUIRE_EQ(result.m_data[1], std::vector<double>{2., 3.});
	}

	TEST_CASE("Matrix simple subtraction")
	{
		const auto mat = Matrix<2>({ {2., 2.}, 
									  {2., 2.} });

		const auto identity = Matrix<2>({ {1., 0.},
									  {0., 1.} });
		const auto result = mat.Subtraction(identity);
		REQUIRE_EQ(result.m_data[0], std::vector<double>{1., 2.});
		REQUIRE_EQ(result.m_data[1], std::vector<double>{2., 1.});
	}
}
