#ifndef ImageRenderer3D_h_include
#define ImageRenderer3D_h_include

#include "tgaimage.h"
#include <Entities.h>

#include <string>

using namespace MathLib;

class ImageRenderer3D
{
public:
    ImageRenderer3D(const uint32_t width, const uint32_t height)
        :m_image(TGAImage(width, height, TGAImage::RGB))
        , m_width(width)
        , m_height(height)
        , m_zBuffer(m_width * m_height, 0)
    {
    }

    ImageRenderer3D() = delete;
    ImageRenderer3D(const ImageRenderer3D& other) = default;
    ImageRenderer3D(ImageRenderer3D&& other) = default;

    void DrawTriangle(const MathLib::Triangle3D& triangle, const TGAColor& color)
    {
        const auto& p1 = triangle.m_data[0];
        const auto& p2 = triangle.m_data[1];
        const auto& p3 = triangle.m_data[2];
        _drawTriangle(p1, p2, p3, color);
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

    BB findBB(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3)
    {
        const auto minMaxX = std::minmax({ p1.X(), p2.X(), p3.X() });
        const auto minMaxY = std::minmax({ p1.Y(), p2.Y(), p3.Y() });
        const auto minX = minMaxX.first;
        const auto maxX = std::min(m_width - 1., minMaxX.second);
        const auto minY = minMaxY.first;
        const auto maxY = std::min(m_height - 1., minMaxY.second);
        return { minX, maxX, minY, maxY };
    }

    Vec3f barycentric(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, const Vec3f& P)
    {
        const auto u = Vec3f{ {p2.X() - p1.X(), p3.X() - p1.X(), p1.X() - P.X()} }.
            Cross(Vec3f{ p2.Y() - p1.Y(),p3.Y() - p1.Y(), p1.Y() - P.Y() });
        if (std::abs(u.Z()) < 1)
            return { -1., 1., 1. };
        return Vec3f{ {1. - (u.X() + u.Y()) / u.Z(), u.Y() / u.Z(), u.X() / u.Z()} };
    }

    void _drawTriangle(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, const TGAColor& color)
    {
        const auto& bb = findBB(p1, p2, p3);

        for (auto x = bb.minX; x <= bb.maxX; x++)
        {
            for (auto y = bb.minY; y <= bb.maxY; y++)
            {
                auto bc = barycentric(p1, p2, p3, { x, y });
                if (bc.X() < 0 || bc.Y() < 0 || bc.Z() < 0)
                    continue;
                auto z = p1.Z() * bc.X() + p2.Z() * bc.Y() + p3.Z() * bc.Z();
                const auto screenPixel = static_cast<int>(x + y * m_width);
                if (m_zBuffer[screenPixel] < z)
                {
                    m_image.set(x, y, color);
                    m_zBuffer[screenPixel] = z;
                }
            }
        }
    }

    TGAImage m_image;
    mutable uint32_t m_width;
    mutable uint32_t m_height;
    std::vector<double> m_zBuffer;
};

#endif