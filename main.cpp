#include <immintrin.h>
#include "sdl.h"

const int max_power          = 255;
const int max_radius_squared = 100;

struct ScreenInfo
{
	float screen_center_re = 0;
	float screen_center_im = 0;
	float scale = 0.005;	
};

void MoveScreen(const SDL_Event* event, ScreenInfo* screen)
{
	assert(event);
	assert(screen);

	switch (event->key.keysym.scancode)
	{
		case scancode_up:
		{
			screen->screen_center_im += screen_num_pixels_to_move * screen->scale;
			break;
		}
		case scancode_down:
		{
			screen->screen_center_im -= screen_num_pixels_to_move * screen->scale;
			break;
		}
		case scancode_left:
		{
			screen->screen_center_re += screen_num_pixels_to_move * screen->scale;
			break;
		}
		case scancode_right:
		{
			screen->screen_center_re -= screen_num_pixels_to_move * screen->scale;
			break;
		}
		case scancode_plus:
		{
			screen->scale /= screen_scale_factor;
			break;
		}
		case scancode_minus:
		{
			screen->scale *= screen_scale_factor;
			break;
		}
		default:
		{
			break;
		}
	}
}

int main()
{
	WindowInfo info = {};
	MakeWindow(&info);

	ScreenInfo screen = {};
    SDL_Event event = {};

    bool is_program_running = true;

	while (is_program_running)
	{
        clock_t start_time = clock();

		for (int x = 0; x < x_size; ++x)
		{
			for (int y = 0; y < y_size; ++y)
			{
				float re_x0 = (x - x_size / 2) * screen.scale - screen.screen_center_re;
				float im_x0 = (y - y_size / 2) * screen.scale - screen.screen_center_im;

				float cur_x = 0;
				float cur_y = 0;

				int power = 0;
				for (; power < max_power; ++power)
				{
					float re_squared = cur_x * cur_x;
					float im_squared = cur_y * cur_y;
					float new_im     = cur_x * cur_y;

					if (re_squared + im_squared > max_radius_squared)
					{
						break;
					}

					cur_x = re_squared - im_squared + re_x0;
					cur_y = 2 * new_im + im_x0;
				}

				uint32_t color = std_color;

				if (power < max_power)
				{
					color = Color(power * power, power * 3, power);
				}

				SetPixel(&info, x, y, color);
			}
		}

		UpdateWindow(&info);
		clock_t end_time = clock();
		SetFPS(&info, int(CLOCKS_PER_SEC / (end_time + 0.0 - start_time)));

		while (SDL_PollEvent(&event))
        {
        	if (event.type == SDL_QUIT)
            {
				DestroyWindow(&info);
				is_program_running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
            	MoveScreen(&event, &screen);
            }
        }
	}

	return 0;
}
