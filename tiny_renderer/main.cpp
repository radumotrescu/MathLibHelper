#include <iostream>
#include <limits>
#include <numeric>

#include "tgaimage.h"
#include "model.h"
#include "../test/TestUtils.h"
#include "Entities.h"
#include "ImageRenderer2D.h"
#include "SdlRenderer.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

using namespace MathLib;

namespace
{
    const TGAColor white = TGAColor(255, 255, 255, 255);
    const TGAColor red = TGAColor(255, 0, 0, 255);
    const TGAColor green = TGAColor(0, 255, 0, 255);
    const int width = 800;
    const int height = 800;
    const int depth = 255;
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

void triangle(Vec3f p1, Vec3f p2, Vec3f p3, const std::vector<Vec2f>& uv, std::vector<double>& zBuffer, TGAImage& image, double intensity, const TGAColor& color)
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
            if (static_cast<int>(x + y * width) > zBuffer.size())
                continue;

            if (z >= zBuffer[static_cast<int>(x + y * width)])
            {
                zBuffer[static_cast<int>(x + y * width)] = z;
                image.set(x, y, TGAColor(color.r*intensity, color.g*intensity, color.b*intensity, color.a));
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

Mat4 viewport(int x, int y, int w, int h)
{
    auto m = Mat4();
    m.Identity();

    m[0][3] = x;
    m[1][3] = y;
    m[2][3] = depth / 2.;

    m[0][0] = w / 2.;
    m[1][1] = h / 2.;
    m[2][2] = depth / 2.;

    return m;
}

Vec3f m2v(const Mat4& m)
{
    return Vec3f{ m[0][0] / m[3][0],
                 m[1][0] / m[3][0],
                 m[2][0] / m[3][0] };
}

Mat4 v2m(const Vec3f& v)
{
    auto m = Mat4();
    m[0][0] = v.X();
    m[1][0] = v.Y();
    m[2][0] = v.Z();
    m[3][0] = 1.;

    return m;
}


void african_head()
{
    TGAImage image(width, height, TGAImage::RGB);

    auto model = Model("obj/african_head.obj");
    //model.loadTexture("obj/african_head_texture.tga");
    const auto light_dir = Vec3f{ 0 , 0, -1 };
    auto zBuffer = std::vector<double>(width * height, std::numeric_limits<double>::lowest());

    auto timer = TestUtils::Timer();

    auto projection = Mat4{};
    projection.Identity();

    auto vp = viewport(400, 400, 600, 600);

    auto cameraZdistance = 5.;
    projection[3][2] = -1. / cameraZdistance;

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

            //screenCoords[j] = Vec3f{ std::round((worldCoords[j].X() + 1.) * width / 2.), std::round((worldCoords[j].Y() + 1.) * height / 2.), worldCoords[j].Z() };

            auto v = v2m(worldCoords[j]);
            //screenCoords[j] = m2v(vp * v);

            screenCoords[j] = m2v(vp * projection * v);
            screenCoords[j][0] = std::round(screenCoords[j][0]);
            screenCoords[j][1] = std::round(screenCoords[j][1]);
            screenCoords[j][2] = std::round(screenCoords[j][2]);
        }

        auto n = (worldCoords[2] - worldCoords[0]).Cross(worldCoords[1] - worldCoords[0]);
        n.Normalize();
        auto intensity = n.Dot(light_dir);

        if (intensity > 0)
        {
            auto uv = std::vector<Vec2f>();
            uv.resize(3);
            for (auto vertexIdx = 0; vertexIdx < 3; vertexIdx++) // for each vertex in the face
            {
                uv[vertexIdx] = model.uv(i, vertexIdx);
            }

            triangle(screenCoords[0], screenCoords[1], screenCoords[2], uv, zBuffer, image, intensity, white);
        }
    }
    std::cout << '\n' << timer.Elapsed() << " milliseconds";

    image.flip_vertically(); // I want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
}

void transformationTests()
{
    auto p1 = Point2D({ 2., 2. });
    p1.scale({ 2., 0.5 });
    p1.translate({ 1., 1. });

    auto p2 = Point2D({ 1., 0. });
    p2.rotate(90);

    auto l1 = Line2D({ 0., 0. }, { 1., 0. });
    l1.scale({ 2., 1. });
    l1.translate({ 1., 2. });

    auto l2 = Line2D({ 0., 0. }, { 1., 0. });
    l2.rotate(90);

    auto t1 = Triangle2D({ 0., 0. }, { 1., 0. }, { 1., 1. });
    t1.scale({ 2., 0 });
    t1.translate({ 10., -1 });

    auto t2 = Triangle2D({ 0., 0. }, { 1., 0. }, { 1., 1. });
    t2.rotate(90);
}

void rendererTest()
{
    auto renderer = ImageRenderer2D(400, 400);
    auto triangle = Triangle2D{ {10., 10.}, {100., 10.}, {10., 100.} };
    auto rectangle = Rectangle2D{ {150., 200.}, {300., 200 }, {315., 399.}, {200., 300. } };
    renderer.DrawTriangle(triangle, white);
    renderer.DrawRectangle(rectangle, red);

    // first scale, then rotate, then translate
    triangle.rotate(75.);
    triangle.translate({ 150., 0. });
    renderer.DrawTriangle(triangle, white);
    renderer.ExportImage("transformations");
}

void perfTest()
{
    spdlog::info("Welcome to start of tiny renderer");

    auto sdl = SdlRenderer(640, 480);
    auto r = Color(255, 0, 0, 255);
    auto g = Color(0, 255, 0, 255);
    auto b = Color(0, 0, 255, 255);

    auto t1 = std::make_shared<MathLib::Triangle3D>(Vec3f{ 200., 200., 1. }, Vec3f{ 300., 200., 1. }, Vec3f{ 300., 300., 2. });
    t1->perspectiveProject(10.);
    sdl.AddRectangle(t1, r);

    auto t2 = std::make_shared<MathLib::Triangle3D>(Vec3f{ 400., 200., 2. }, Vec3f{ 400., 300., 2. }, Vec3f{ 500., 300., 2. });
    t2->perspectiveProject(10);
    sdl.AddRectangle(t2, g);

    for (int i = 0; i < 1000; i++)
    {
        auto t = std::make_shared<MathLib::Triangle3D>(Vec3f{ 0., 0., 0. }, Vec3f{ 100., 0., 0 }, Vec3f{ 100., 100., 0 });
        sdl.AddRectangle(t, b);
    }

    sdl.Render();
}


int main(int argc, char** argv)
{
    //triangle_tests();
    //bmw();
    african_head();
    //transformationTests();
    //rendererTest();

    return 0;
}