#include <VecN.h>
#include <MatN.h>

using namespace MathLib;

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