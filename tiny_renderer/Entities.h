#ifndef Entities_h_include
#define Entities_h_include

#include <numeric>

#include <VecN.h>
#include <MatN.h>

using namespace MathLib;


double d2r(double degrees)
{
    const double pi = std::acos(-1);
    return degrees * pi / 180.;
}
struct Point2D
{
    Point2D(Vec2f data) : m_data(data) {};
    Vec2f m_data;

    void scale(const Vec2f& scalingFactors)
    {
        auto scalingMatrix = Matrix<2, 2>({ { scalingFactors.X(), 0 }, { 0, scalingFactors.Y() } });
        auto columnMajor = Matrix<2, 1>({ {m_data.X()}, {m_data.Y() } });

        auto result = (scalingMatrix * columnMajor).ConvertToRowMajor();
        m_data = { result[0], result[1] };
    }

    void translate(const Vec2f& offsets)
    {
        auto result = m_data + offsets;
        m_data = result;
    }

    void rotate(const double& degrees)
    {
        auto rads = d2r(degrees);
        auto rotationMatrix = Matrix<2, 2>({ { std::cos(rads), -std::sin(rads) }, { std::sin(rads), std::cos(rads) } });
        auto columnMajor = Matrix<2, 1>({ {m_data.X()}, {m_data.Y() } });

        auto result = (rotationMatrix * columnMajor).ConvertToRowMajor();
        m_data = { result[0], result[1] };
    }
};

struct Line2D
{
    Line2D(Vec2f p1, Vec2f p2) : m_data{ p1, p2 } {};
    std::array<Vec2f, 2> m_data;

    void scale(const Vec2f& scalingFactors)
    {
        auto scalingMatrix = Matrix<2, 2>({ { scalingFactors.X(), 0 }, { 0, scalingFactors.Y() } });
        auto columnMajor = Matrix<2, 2>({ {m_data[0].X(), m_data[1].X()}, {m_data[0].Y(), m_data[1].Y() } });

        auto result = scalingMatrix * columnMajor;
        m_data[0] = { result.at(0, 0), result.at(1, 0) };
        m_data[1] = { result.at(0, 1), result.at(1, 1) };
    }

    void translate(const Vec2f& offsets)
    {
        for (auto& point : m_data)
            point = point + offsets;
    }

    void rotate(const double& degrees)
    {
        auto rads = d2r(degrees);
        auto rotationMatrix = Matrix<2, 2>({ { std::cos(rads), -std::sin(rads) }, { std::sin(rads), std::cos(rads) } });
        auto columnMajor = Matrix<2, 2>({ {m_data[0].X(), m_data[1].X()}, {m_data[0].Y(), m_data[1].Y() } });

        auto result = rotationMatrix * columnMajor;
        m_data[0] = { result.at(0, 0), result.at(1, 0) };
        m_data[1] = { result.at(0, 1), result.at(1, 1) };
    }
};

struct Triangle2D
{
    Triangle2D(Vec2f p1, Vec2f p2, Vec2f p3) : m_data{ p1, p2, p3 } {};
    std::array<Vec2f, 3> m_data;

    void scale(const Vec2f& scalingFactors)
    {
        auto scalingMatrix = Matrix<2, 2>({ { scalingFactors.X(), 0 }, { 0, scalingFactors.Y() } });
        auto columnMajor = Matrix<2, 3>({ {m_data[0].X(), m_data[1].X(), m_data[2].X()}, {m_data[0].Y(), m_data[1].Y(), m_data[2].Y() } });

        auto result = scalingMatrix * columnMajor;
        m_data[0] = { result.at(0, 0), result.at(1, 0) };
        m_data[1] = { result.at(0, 1), result.at(1, 1) };
        m_data[2] = { result.at(0, 2), result.at(1, 2) };
    }

    void translate(const Vec2f& offsets)
    {
        for (auto& point : m_data)
            point = point + offsets;
    }

    void rotate(const double& degrees)
    {
        auto rads = d2r(degrees);
        auto rotationMatrix = Matrix<2, 2>({ { std::cos(rads), -std::sin(rads) }, { std::sin(rads), std::cos(rads) } });
        auto columnMajor = Matrix<2, 3>({ {m_data[0].X(), m_data[1].X(), m_data[2].X()}, {m_data[0].Y(), m_data[1].Y(), m_data[2].Y() } });

        auto result = rotationMatrix * columnMajor;
        m_data[0] = { result.at(0, 0), result.at(1, 0) };
        m_data[1] = { result.at(0, 1), result.at(1, 1) };
        m_data[2] = { result.at(0, 2), result.at(1, 2) };
    }
};

struct Rectangle2D
{

    Rectangle2D(Vec2f p1, Vec2f p2, Vec2f p3, Vec2f p4) : m_data{ p1, p2, p3, p4 } {};
    std::array<Vec2f, 4> m_data;

    void scale(const Vec2f& scalingFactors)
    {
        auto scalingMatrix = Matrix<2, 2>({ { scalingFactors.X(), 0 }, { 0, scalingFactors.Y() } });
        auto columnMajor = Matrix<2, 3>({ {m_data[0].X(), m_data[1].X(), m_data[2].X(), m_data[3].X()}, 
            {m_data[0].Y(), m_data[1].Y(), m_data[2].Y(), m_data[3].Y() } });

        auto result = scalingMatrix * columnMajor;
        m_data[0] = { result.at(0, 0), result.at(1, 0) };
        m_data[1] = { result.at(0, 1), result.at(1, 1) };
        m_data[2] = { result.at(0, 2), result.at(1, 2) };
        m_data[3] = { result.at(0, 3), result.at(1, 3) };
    }

    void translate(const Vec2f& offsets)
    {
        for (auto& point : m_data)
            point = point + offsets;
    }

    void rotate(const double& degrees)
    {
        auto rads = d2r(degrees);
        auto rotationMatrix = Matrix<2, 2>({ { std::cos(rads), -std::sin(rads) }, { std::sin(rads), std::cos(rads) } });
        auto columnMajor = Matrix<2, 3>({ {m_data[0].X(), m_data[1].X(), m_data[2].X(), m_data[3].X()}, 
            {m_data[0].Y(), m_data[1].Y(), m_data[2].Y(), m_data[3].Y() } });

        auto result = rotationMatrix * columnMajor;
        m_data[0] = { result.at(0, 0), result.at(1, 0) };
        m_data[1] = { result.at(0, 1), result.at(1, 1) };
        m_data[2] = { result.at(0, 2), result.at(1, 2) };
        m_data[3] = { result.at(0, 3), result.at(1, 3) };
    }
};

#endif