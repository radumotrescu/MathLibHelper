#include "tgaimage.h"
#include "model.h"
#include "../test/TestUtils.h"
#include <iostream>
#include <limits>

using namespace MathLib;

namespace
{
    const TGAColor white = TGAColor(255, 255, 255, 255);
    const TGAColor red = TGAColor(255, 0, 0, 255);
    const TGAColor green = TGAColor(0, 255, 0, 255);
    const int width = 800;
    const int height = 800;
}

void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color)
{
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror = std::abs(dy) * 2;
    int error = 0;

    int y = y0;
    int yIncr = y1 > y0 ? 1 : -1;
    for (auto x = x0; x <= x1; x++)
    {
        if (steep)
            image.set(y, x, color);
        else
            image.set(x, y, color);
        error += derror;
        if (error > dx)
        {
            y += yIncr;
            error -= dx * 2;
        }
    }
}

struct BoundingBox
{
    double minX, maxX, minY, maxY;
};
using BB = BoundingBox;

BB findBB(Vec3f p1, Vec3f p2, Vec3f p3)
{
    auto minMaxX = std::minmax({ p1.X(), p2.X(), p3.X() });
    auto minMaxY = std::minmax({ p1.Y(), p2.Y(), p3.Y() });
    auto minX = minMaxX.first;
    auto maxX = std::min(width - 1., minMaxX.second);
    auto minY = minMaxY.first;
    auto maxY = std::min(height - 1., minMaxY.second);
    return { minX, maxX, minY, maxY };
}

Vec3f barycentric(Vec3f p1, Vec3f p2, Vec3f p3, Vec3f P)
{
    auto u = Vec3f{ {p2.X() - p1.X(), p3.X() - p1.X(), p1.X() - P.X()} }.
        Cross(Vec3f{ p2.Y() - p1.Y(),p3.Y() - p1.Y(), p1.Y() - P.Y() });
    if (std::abs(u.Z()) < 1)
        return { -1., 1., 1. };
    return Vec3f{ {1. - (u.X() + u.Y()) / u.Z(), u.Y() / u.Z(), u.X() / u.Z()} };
}

void triangle(Vec3f p1, Vec3f p2, Vec3f p3, std::vector<double>& zBuffer, TGAImage& image, const TGAColor& color)
{
    auto bb = findBB(p1, p2, p3);

    for (auto x = bb.minX; x <= bb.maxX; x++)
    {
        for (auto y = bb.minY; y <= bb.maxY; y++)
        {
            auto bc = barycentric(p1, p2, p3, { x, y });
            if (bc.X() < 0 || bc.Y() < 0 || bc.Z() < 0)
                continue;
            auto z = p1.Z() * bc.X() + p2.Z() * bc.Y() + p3.Z() * bc.Z();
            if (z >= zBuffer[static_cast<int>(x + y * width)])
            {
                zBuffer[static_cast<int>(x + y * width)] = z;
                image.set(x, y, color);
            }
        }
    }
}

void triangle_tests()
{
    TGAImage image(300, 300, TGAImage::RGB);

    auto t1 = std::vector<Vec2i>{ Vec2i{50, 10},   Vec2i{30, 50},  Vec2i{10, 100} };
    auto t2 = std::vector<Vec2i>{ Vec2i{180, 50},  Vec2i{150, 1},   Vec2i{70, 180} };
    auto t3 = std::vector<Vec2i>{ Vec2i{180, 150}, Vec2i{120, 160}, Vec2i{130, 180} };

    //triangle(t1[0], t1[1], t1[2], image, white);
    //triangle(t2[0], t2[1], t2[2], image, red);
    //triangle(t3[0], t3[1], t3[2], image, green);

    image.flip_vertically(); // I want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
}

void transformationTests()
{
    TGAImage image(400, 400, TGAImage::RGB);
}

void african_head()
{
    TGAImage image(width, height, TGAImage::RGB);

    auto model = Model("obj/african_head.obj");
    const auto light_dir = Vec3f{ 0 , 0, -1 };
    auto zBuffer = std::vector<double>(width*height, std::numeric_limits<double>::lowest());

    auto timer = TestUtils::Timer();
    for (auto i = 0; i < model.nfaces(); i++)
    {
        // each face has 3 lines
        auto face = model.face(i);
        if (face.empty())
            continue;

        Vec3f screenCoords[3];
        Vec3f worldCoords[3];
        for (int j = 0; j < 3; j++)
        {
            worldCoords[j] = model.vert(face[j]);
            screenCoords[j] = Vec3f{ std::round((worldCoords[j].X() + 1.)*width / 2.), std::round((worldCoords[j].Y() + 1.)*height / 2.), worldCoords[j].Z() };
        }

        auto n = (worldCoords[2] - worldCoords[0]).Cross(worldCoords[1] - worldCoords[0]);
        n.Normalize();
        auto intensity = n.Dot(light_dir);

        if (intensity > 0)
            triangle(screenCoords[0], screenCoords[1], screenCoords[2], zBuffer, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
    }
    std::cout << '\n' << timer.Elapsed() << " milliseconds";

    image.flip_vertically(); // I want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
}

int main()
{
    triangle_tests();
    //bmw();
    african_head();
    return 0;
}