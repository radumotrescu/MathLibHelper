#include <numeric>

#include <VecN.h>
#include <MatN.h>

using namespace MathLib;


double d2r(double degrees)
{
    const double pi = std::acos(-1);
    return degrees * pi / 180.;
}
struct Point
{
    Point(Vec2f _data) : data(_data) {};
    Vec2f data;

    void scale(const Vec2f& scalingFactors)
    {
        auto scalingMatrix = Matrix<2, 2>({ { scalingFactors.X(), 0 }, { 0, scalingFactors.Y() } });
        auto columnMajor = Matrix<2, 1>({ {data.X()}, {data.Y() }});

        auto result = (scalingMatrix * columnMajor).ConvertToRowMajor();
        data = { result[0], result[1] };
    }

    void translate(const Vec2f& offsets)
    {
        auto result = data + offsets;
        data = result;
    }

    void rotate(const double& degrees)
    {
        auto rads = d2r(degrees);
        auto rotationMatrix = Matrix<2, 2>({ { std::cos(rads), -std::sin(rads) }, { std::sin(rads), std::cos(rads) } });
        auto columnMajor = Matrix<2, 1>({ {data.X()}, {data.Y() }});

        auto result = (rotationMatrix * columnMajor).ConvertToRowMajor();
        data = { result[0], result[1] };
    }
};

struct Line
{

};

struct Triangle
{

};

struct Rectangle
{

};