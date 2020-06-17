#include "unidefs.h"
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "Entities.h"
#include "../test/TestUtils.h"

struct Color
{
    union {
        struct {
            unsigned char b, g, r, a;
        };
        unsigned char raw[4];
        unsigned int val;
    };
    int bytespp;

    Color() : val(0), bytespp(1) {
    }

    Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : b(B), g(G), r(R), a(A), bytespp(4) {
    }

    Color(int v, int bpp) : val(v), bytespp(bpp) {
    }

    Color(const Color &c) : val(c.val), bytespp(c.bytespp) {
    }

    Color(const unsigned char *p, int bpp) : val(0), bytespp(bpp) {
        for (int i = 0; i < bpp; i++) {
            raw[i] = p[i];
        }
    }

    Color & operator =(const Color &c) {
        if (this != &c) {
            bytespp = c.bytespp;
            val = c.val;
        }
        return *this;
    }
};

class SdlRenderer
{
public:

    SdlRenderer() = delete;

    SdlRenderer(int width, int height)
        :_width(width), _height(height), _zBuffer(_width* _height, std::numeric_limits<double>::lowest()), _pixelColors(_width* _height, Color(0, 0, 0, 255))
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        }

        _window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_RESIZABLE);
        if (!_window) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        }
        _surface = SDL_GetWindowSurface(_window);
        _pixels = static_cast<unsigned int*>(_surface->pixels);
    }

    void AddRectangle(std::shared_ptr<MathLib::Triangle3D> triangle)
    {
        _triangles.push_back(triangle);
    }

    void Render()
    {
        SDL_Event event;
        auto timer = TestUtils::Timer();
        while (true) {
            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT) {
                break;
            }
            timer.Reset();


            // clear pixels
            {
                std::fill(_pixelColors.begin(), _pixelColors.end(), Color(0, 0, 0, 255));
            }
            // process triangles
            {
                auto red = Color(255, 0, 0, 255);
                for (const auto& t : _triangles)
                {
                    const auto& p1 = t->m_data[0];
                    const auto& p2 = t->m_data[1];
                    const auto& p3 = t->m_data[2];

                    auto bb = findBB(p1, p2, p3);

                    for (auto x = bb.minX; x <= bb.maxX; x++)
                    {
                        for (auto y = bb.minY; y <= bb.maxY; y++)
                        {
                            auto bc = barycentric(p1, p2, p3, { x, y });
                            if (bc.X() < 0 || bc.Y() < 0 || bc.Z() < 0)
                                continue;
                            auto z = p1.Z() * bc.X() + p2.Z() * bc.Y() + p3.Z() * bc.Z();
                            if (z >= _zBuffer[static_cast<int>(x + y * _width)])
                            {
                                _zBuffer[static_cast<int>(x + y * _width)] = z;
                                _pixelColors[x + y * _width] = red;
                            }
                        }
                    }
                    t->translate(Vec3f{0.1, 0., 0.});
                }
            }

            for (auto j = 0; j < _height; j++)
                for (auto i = 0; i < _width; i++)
                {
                    const auto& c = _pixelColors[i + j * _width];
                    _pixels[i + j * _width] = SDL_MapRGBA(_surface->format, c.r, c.g, c.b, c.a);
                }

            SDL_UpdateWindowSurface(_window);
            spdlog::info("fps: {}", 1./timer.Elapsed()*1000);
        }

        SDL_DestroyWindow(_window);
        SDL_Quit();
    }

private:

    struct BoundingBox
    {
        double minX, maxX, minY, maxY;
    };
    using BB = BoundingBox;

    BB findBB(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3)
    {
        auto minMaxX = std::minmax({ p1.X(), p2.X(), p3.X() });
        auto minMaxY = std::minmax({ p1.Y(), p2.Y(), p3.Y() });
        auto minX = minMaxX.first;
        auto maxX = std::min(_width - 1., minMaxX.second);
        auto minY = minMaxY.first;
        auto maxY = std::min(_height - 1., minMaxY.second);
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

    uint _width;
    uint _height;

    // these should not be deleted by hand, only by SDL
    SDL_Window* _window;
    SDL_Surface* _surface;
    uint* _pixels;
    std::vector<double> _zBuffer;
    std::vector<Color> _pixelColors;

    std::vector<std::shared_ptr<MathLib::Triangle3D>> _triangles;
};
