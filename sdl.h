#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <SDL2/SDL.h>

static const int x_size = 1200;
static const int y_size = 800;
static const int x_top_left_corner = 1920 / 2 - x_size / 2;
static const int y_top_left_corner = 1080 / 2 - y_size / 2;
static const int title_size = 10;

static const int scancode_up    = 82;
static const int scancode_down  = 81;
static const int scancode_left  = 80;
static const int scancode_right = 79;

static const int scancode_plus  = 46;
static const int scancode_minus = 45;

static const double screen_scale_factor = 1.4;
static const double screen_num_pixels_to_move = 10;

static const int byte_size = 8;
static const uint32_t std_color = 0x000f0f0f;

struct WindowInfo
{
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Surface*  surface  = nullptr;
    SDL_Texture*  texture  = nullptr;
};

void MakeWindow   (WindowInfo* info);
void UpdateWindow (WindowInfo* info);
void SetPixel     (WindowInfo* info, int x, int y, uint32_t pixel);
void SetFPS       (WindowInfo* info, int fps);
void DestroyWindow(WindowInfo* info);

uint32_t Color(unsigned char red, unsigned char green, unsigned char black);