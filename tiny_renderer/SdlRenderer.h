#include "unidefs.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>


class SdlRenderer
{
public:

    SdlRenderer() = delete;
    SdlRenderer(int width, int height) : _width(width), _height(height)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        }

        _window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_RESIZABLE);
        if (!_window) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        }
        _surface = SDL_GetWindowSurface(_window);
    }

    void Render()
    {
        _pixels = static_cast<unsigned int*>(_surface->pixels);

        auto fade = 1.;
        auto delta = -0.0001;
        SDL_Event event;

        while (true) {
            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT) {
                break;
            }

            for (auto j = 0; j < _height; j++)
                for (auto i = 0; i < _width; i++)
                    _pixels[i + j * _width] = SDL_MapRGBA(_surface->format, 200 * fade, 100, 250, 255);

            fade += delta;
            
            spdlog::info("fade: {}", fade);
            if (EQ(fade, 0.5, 1e-7) || EQ(fade, 1, 1e-7))
            {
                delta = -delta;
            }
            SDL_UpdateWindowSurface(_window);
        }

        SDL_DestroyWindow(_window);
        SDL_Quit();
    }

private:
    uint _width;
    uint _height;

    // these should not be deleted by hand, only by SDL
    SDL_Window* _window;
    SDL_Surface* _surface;
    uint* _pixels;
};
