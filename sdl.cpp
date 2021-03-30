#include "sdl.h"

uint32_t Color(unsigned char red, unsigned char green, unsigned char black, unsigned char a)
{
  uint32_t color = a;
  color <<= byte_size;
  color |= red;
  color <<= byte_size;
  color |= green;
  color <<= byte_size;
  color |= black;
  return color;
}

uint32_t Color(unsigned char red, unsigned char green, unsigned char black)
{
    return Color(red, green, black, 0);
}

void InitWindow(WindowInfo* info)
{
    assert(info);

    assert(SDL_Init(SDL_INIT_VIDEO) ==  0 && "Can't initialize SDL\n");
    
    info->window = SDL_CreateWindow("Mandelbrot", x_top_left_corner,
                                                  y_top_left_corner, x_size, y_size, SDL_WINDOW_MAXIMIZED);
    assert(info->window   && "Can't create window\n");

    info->renderer = SDL_CreateRenderer(info->window, -1, SDL_RENDERER_ACCELERATED);
    assert(info->renderer && "Can't create renderer\n");
}

void SetPixel(WindowInfo* info, int x, int y, uint32_t pixel)
{
    assert(info);
    assert(x >= 0);
    assert(y >= 0);

    uint32_t* target_pixel = (uint32_t*)((uint8_t*)info->surface->pixels + y * info->surface->pitch +
                                                                           x * info->surface->format->BytesPerPixel);
    *target_pixel = pixel;
}

void UpdateWindow(WindowInfo* info)
{
    assert(info);

    SDL_UpdateWindowSurface(info->window);
}

void MakeWindow(WindowInfo* info)
{
    assert(info);
    InitWindow(info);

    info->surface = SDL_GetWindowSurface(info->window); //!!! Amask(last param mb 255)
    assert(info->surface && "Can't create RGB surface\n");
}

void SetFPS(WindowInfo* info, int fps)
{
    static char title[title_size] = "";

    snprintf(title, title_size, "FPS %d", fps);
    SDL_SetWindowTitle(info->window, title);
}

void DestroyWindow(WindowInfo* info)
{
    SDL_DestroyWindow(info->window);
    SDL_Quit();

    info->texture  = nullptr;
    info->renderer = nullptr;
    info->window   = nullptr;
}
