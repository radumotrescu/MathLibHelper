#include <doctest/doctest.h>

#include <MatN.h>
#include "TestUtils.h"

using namespace MathLib;

TEST_SUITE("MatN tests")
{
    TEST_CASE("Matrix row size size is set correctly")
    {
        const auto mat = Mat3();
        const auto rowSize = mat.GetData().size();
        REQUIRE_EQ(rowSize, 3);
    }

    TEST_CASE("Matrix column size size is set correctly")
    {
        const auto mat = Mat3();
        const auto columnSize = mat.GetData()[0].size();
        REQUIRE_EQ(columnSize, 3);
    }

    TEST_CASE("Matrix simple multiplication")
    {
        {
            const auto mat = Mat2({ {2., 2.},
                                          {2., 2.} });

            const auto identity = Mat2({ {1., 0.},
                                          {0., 1.} });
            const auto result = mat.SimpleMulti(identity);
            REQUIRE_EQ(result.GetData()[0], std::vector<double>{2., 2.});
            REQUIRE_EQ(result.GetData()[1], std::vector<double>{2., 2.});
        }

        // test operator overloading
        {
            const auto mat = Mat2({ {2., 2.},
                                          {2., 2.} });

            const auto identity = Mat2({ {1., 0.},
                                          {0., 1.} });
            const auto result = mat * identity;
            REQUIRE_EQ(result.GetData()[0], std::vector<double>{2., 2.});
            REQUIRE_EQ(result.GetData()[1], std::vector<double>{2., 2.});
        }

        // test operator overloading
        {
            const auto mat = Mat3({ {2., 2., 3.},
                                          {2., 2., 1.},
                                          {2., 1., 0.} });

            const auto identity = Mat3({ {1., 0., 1.},
                                          {0., 1., 2.},
                                          {4., 1., 3.} });
            const auto result = mat * identity;
            REQUIRE_EQ(result.GetData()[0], std::vector<double>{14., 5., 15.});
            REQUIRE_EQ(result.GetData()[1], std::vector<double>{6., 3., 9.});
            REQUIRE_EQ(result.GetData()[2], std::vector<double>{2., 1., 4.});
        }

        {
            const auto mat1 = Matrix<1, 2>({ {2., 3.} });

            const auto mat2 = Matrix<2, 1>({ {2.},
                                             {1.} });
            const auto result = mat1 * mat2;
            REQUIRE_EQ(result.GetData()[0], std::vector<double>{7.});
        }

        {
            const auto mat1 = Matrix<1, 2>({ {2., 2.} });

            const auto mat2 = Matrix<2, 1>({ {1.},
                                             {1.} });
            const auto result = mat2 * mat1;
            REQUIRE_EQ(result.GetData()[0], std::vector<double>{2., 2.});
            REQUIRE_EQ(result.GetData()[1], std::vector<double>{2., 2.});
        }

        {
            const auto mat1 = Matrix<2, 2>({ {0.5, 0.}, {0., 1.} });

            const auto mat2 = Matrix<2, 1>({ {2.},
                                             {2.} });
            const auto result = mat1 * mat2;
            REQUIRE_EQ(result.GetData()[0], std::vector<double>{1.});
            REQUIRE_EQ(result.GetData()[1], std::vector<double>{2.});
        }
    }

    TEST_CASE("Matrix simple addition")
    {
        {
            const auto mat = Mat2({ {2., 2.},
                                         {2., 2.} });

            const auto identity = Mat2({ {1., 0.},
                                          {0., 1.} });
            const auto result = mat.Addition(identity);
            REQUIRE_EQ(result.GetData()[0], std::vector<double>{3., 2.});
            REQUIRE_EQ(result.GetData()[1], std::vector<double>{2., 3.});
        }

        // test operator overloading
        {
            const auto mat = Mat2({ {2., 2.},
                                    {2., 2.} });

            const auto identity = Mat2({ {1., 0.},
                                       {0., 1.} });
            const auto result = mat + identity;
            REQUIRE_EQ(result.GetData()[0], std::vector<double>{3., 2.});
            REQUIRE_EQ(result.GetData()[1], std::vector<double>{2., 3.});
        }

        // simple addition with uneven matrix
        {
            const auto mat1 = Matrix<2, 3>({ {2., 2., 1.},
                                    {2., 2., 1.} });

            const auto mat2 = Matrix<2, 3>({ {2., 2., 1.},
                                    {2., 2., 1.} });
            const auto result = mat1 + mat2;
            REQUIRE_EQ(result.GetData()[0], std::vector<double>{4., 4., 2.});
            REQUIRE_EQ(result.GetData()[1], std::vector<double>{4., 4., 2.});
        }
    }

    TEST_CASE("Matrix simple subtraction")
    {
        {
            const auto mat = Mat2({ {2., 2.},
                                          {2., 2.} });

            const auto identity = Mat2({ {1., 0.},
                                          {0., 1.} });

            const auto result = mat.Subtraction(identity);

            REQUIRE_EQ(result.GetData()[0], std::vector<double>{1., 2.});
            REQUIRE_EQ(result.GetData()[1], std::vector<double>{2., 1.});
        }

        // test operator overloading
        {
            const auto mat = Mat2({ {2., 2.},
                                          {2., 2.} });

            const auto identity = Mat2({ {1., 0.},
                                          {0., 1.} });

            const auto result = mat - identity;

            REQUIRE_EQ(result.GetData()[0], std::vector<double>{1., 2.});
            REQUIRE_EQ(result.GetData()[1], std::vector<double>{2., 1.});
        }

        // simple addition with uneven matrix
        {
            const auto mat1 = Matrix<2, 3>({ {2., 2., 1.},
                                    {2., 2., 1.} });

            const auto mat2 = Matrix<2, 3>({ {2., 2., 1.},
                                    {2., 2., 1.} });
            const auto result = mat1 - mat2;
            REQUIRE_EQ(result.GetData()[0], std::vector<double>{0., 0., 0.});
            REQUIRE_EQ(result.GetData()[1], std::vector<double>{0., 0., 0.});
        }
    }

    TEST_CASE("Matrix convert to row major")
    {
        const auto mat = Mat2({ {1., 2.},
                                     {3., 4.} });
        const auto rowMajor = mat.ConvertToRowMajor();
        REQUIRE_EQ(rowMajor, std::vector<double>{1., 2., 3., 4.});
    }

    TEST_CASE("Matrix convert to column major")
    {
        const auto mat = Mat2({ {1., 2.},
                                {3., 4.} });
        const auto colMajor = mat.ConvertToColMajor();
        REQUIRE_EQ(colMajor, std::vector<double>{1., 3., 2., 4.});
    }
}
