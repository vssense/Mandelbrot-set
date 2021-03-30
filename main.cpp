#include <immintrin.h>
#include "sdl.h"

const int max_power             = 255;
const __m256 max_radius_squared = _mm256_set1_ps(100);

const int vector_size = 8;

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

void DrawFrame(WindowInfo* info, ScreenInfo* screen)
{
	for (int x = 0; x < x_size; x += vector_size)
	{
		for (int y = 0; y < y_size; ++y)
		{
			float re_x0_array[vector_size] = {};
			float im_x0_array[vector_size] = {};

			for (int i = 0; i < vector_size; ++i)
			{
				re_x0_array[i] = (i + x - x_size / 2) * screen->scale - screen->screen_center_re;
				im_x0_array[i] = (    y - y_size / 2) * screen->scale - screen->screen_center_im;
			}

			__m256 re_x0 = _mm256_load_ps(re_x0_array);
			__m256 im_x0 = _mm256_load_ps(im_x0_array);

			__m256 cur_x = _mm256_setzero_ps();
			__m256 cur_y = _mm256_setzero_ps();

			int powers[vector_size] = {0};
			int power = 0;
			for (; power < max_power; ++power)
			{
				__m256 re_squared = _mm256_mul_ps(cur_x, cur_x);
				__m256 im_squared = _mm256_mul_ps(cur_y, cur_y);

				int mask = _mm256_movemask_ps(_mm256_cmp_ps(_mm256_add_ps(re_squared, im_squared), max_radius_squared, _CMP_LT_OS));
				// mask[i] = re_squared[i] + im_squared[i] < max_radius_squared

				if (mask == 0)
				{
					break;
				}

				int bit = 1;

				for (int i = 0; i < vector_size; ++i)
				{
					if (mask & bit)
					{
						powers[i]++;
					}
					
					bit <<= 1;
				}

				__m256 new_im = _mm256_mul_ps(cur_x, cur_y);

				cur_x = _mm256_add_ps(_mm256_sub_ps(re_squared, im_squared), re_x0);
				cur_y = _mm256_add_ps(_mm256_add_ps(new_im, new_im), im_x0);
			}

			for (int i = 0; i < vector_size; ++i)
			{
				power = powers[i];
				uint32_t color = std_color;
				if (power < max_power)
				{
					color = Color(power * power, power * 3, power);
				}
				
				SetPixel(info, x + i, y, color);
			}

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

        DrawFrame(&info, &screen);
		
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
