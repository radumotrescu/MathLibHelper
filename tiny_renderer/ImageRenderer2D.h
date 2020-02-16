#ifndef ImageRenderer2D_h_include
#define ImageRenderer2D_h_include

#include "tgaimage.h"
#include <Entities.h>

#include <string>

using namespace MathLib;

class ImageRenderer2D
{
public:
    ImageRenderer2D(const uint32_t width, const uint32_t height)
        :m_image(TGAImage(width, height, TGAImage::RGB))
    {
    }

    ImageRenderer2D() = delete;
    ImageRenderer2D(const ImageRenderer2D& other) = default;
    ImageRenderer2D(ImageRenderer2D&& other) = default;

    void DrawLine(const Line2D& line, const TGAColor& color)
    {
        auto x0 = line.m_data[0].X();
        auto x1 = line.m_data[1].X();
        auto y0 = line.m_data[0].Y();
        auto y1 = line.m_data[1].Y();

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

        const int dx = x1 - x0;
        const int dy = y1 - y0;
        const int derror = std::abs(dy) * 2;
        int error = 0;

        int y = y0;
        const int yIncr = y1 > y0 ? 1 : -1;
        for (auto x = x0; x <= x1; x++)
        {
            if (steep)
                m_image.set(y, x, color);
            else
                m_image.set(x, y, color);
            error += derror;
            if (error > dx)
            {
                y += yIncr;
                error -= dx * 2;
            }
        }
    }

    void DrawTriangle(const Triangle2D& triangle, const TGAColor& color)
    {
        const auto& p1 = triangle.m_data[0];
        const auto& p2 = triangle.m_data[1];
        const auto& p3 = triangle.m_data[2];
        _drawTriangle(p1, p2, p3, color);
    }

    void DrawRectangle(const Rectangle2D& rectangle, const TGAColor& color)
    {
        const auto& p1 = rectangle.m_data[0];
        const auto& p2 = rectangle.m_data[1];
        const auto& p3 = rectangle.m_data[2];
        const auto& p4 = rectangle.m_data[3];
        _drawTriangle(p1, p2, p3, color);
        _drawTriangle(p1, p3, p4, color);
    }

    void ExportImage(std::string path)
    {
        m_image.flip_vertically(); // I want to have the origin at the left bottom corner of the image
        m_image.write_tga_file(path.append(".tga").c_str());
    }

private:
    struct BoundingBox
    {
        double minX, maxX, minY, maxY;
    };
    using BB = BoundingBox;

    BB findBB(const Vec2f& p1, const Vec2f& p2, const Vec2f& p3)
    {
        const auto minMaxX = std::minmax({ p1.X(), p2.X(), p3.X() });
        const auto minMaxY = std::minmax({ p1.Y(), p2.Y(), p3.Y() });
        const auto minX = minMaxX.first;
        const auto maxX = std::min(m_width - 1., minMaxX.second);
        const auto minY = minMaxY.first;
        const auto maxY = std::min(m_height - 1., minMaxY.second);
        return { minX, maxX, minY, maxY };
    }

    Vec3f barycentric(const Vec2f& p1, const Vec2f& p2, const Vec2f& p3, const Vec2f& P)
    {
        const auto u = Vec3f{ {p2.X() - p1.X(), p3.X() - p1.X(), p1.X() - P.X()} }.
            Cross(Vec3f{ p2.Y() - p1.Y(),p3.Y() - p1.Y(), p1.Y() - P.Y() });
        if (std::abs(u.Z()) < 1)
            return { -1., 1., 1. };
        return Vec3f{ {1. - (u.X() + u.Y()) / u.Z(), u.Y() / u.Z(), u.X() / u.Z()} };
    }

    void _drawTriangle(const Vec2f& p1, const Vec2f& p2, const Vec2f& p3, const TGAColor& color)
    {
        const auto& bb = findBB(p1, p2, p3);

        for (auto x = bb.minX; x <= bb.maxX; x++)
        {
            for (auto y = bb.minY; y <= bb.maxY; y++)
            {
                auto bc = barycentric(p1, p2, p3, { x, y });
                if (bc.X() < 0 || bc.Y() < 0 || bc.Z() < 0)
                    continue;
                m_image.set(x, y, color);
            }
        }
    }

    TGAImage m_image;
    mutable uint32_t m_width;
    mutable uint32_t m_height;

};

#endif