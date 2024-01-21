#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 800

typedef struct
{
	float x;
	float y;
} v2;

typedef struct
{
	int type;
	v2 velocity;
} pixel;

enum
{
	AIR,
	SAND,
	WATER,
};
int randInRange(int min, int max)
{
	int x = min + (int)(rand() / (double)(RAND_MAX - 1) * (max - min + 1));
	return x;
}

void draw_pixels(SDL_Renderer *renderer, pixel *pixels)
{
	SDL_RenderClear(renderer);
	for (int x = 0; x < SIZE; ++x)
	{
		for (int y = 0; y < SIZE; ++y)
		{
			if (pixels[x + (y * SIZE)].type == AIR)
			{
				SDL_SetRenderDrawColor(renderer, 120, 180, 255, 255);
			}
			else if (pixels[x + (y * SIZE)].type == WATER)
			{
				SDL_SetRenderDrawColor(renderer, 10, 30, 255, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 200, 130, 20, 255);
			}
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
	SDL_RenderPresent(renderer);
}

void update_pixels(pixel *pixels)
{
	char complete[SIZE * SIZE];
	for (int x = 0; x < SIZE * SIZE; ++x)
	{
		complete[x] = 0;
	}
	for (int x = 0; x != SIZE; ++x)
	{
		for (int y = 0; y != SIZE; ++y)
		{
			if (pixels[x + (y * SIZE)].type == WATER && !complete[x + (y * SIZE)])
			{
				if (y + 1 < SIZE - 1)
				{
					if (pixels[x + ((y + 2) * SIZE)].type == AIR)
					{
						pixels[x + ((y + 2) * SIZE)].type = WATER;
						pixels[x + ((y)*SIZE)].type = AIR;
						complete[x + ((y + 2) * SIZE)] = 1;
					}
					else
					{
						if (x + 2 < SIZE - 1 && x - 2 > 0)
						{
							if (pixels[(x + 1) + ((y + 1) * SIZE)].type == AIR)
							{
								pixels[(x + 1) + ((y + 1) * SIZE)].type = WATER;
								complete[(x + 1) + ((y + 1) * SIZE)] = 1;
								pixels[x + (y * SIZE)].type = AIR;
							}
							else if (pixels[(x - 1) + ((y + 1) * SIZE)].type == AIR)
							{
								pixels[(x - 1) + ((y + 1) * SIZE)].type = WATER;
								complete[(x - 1) + ((y + 1) * SIZE)] = 1;
								pixels[x + (y * SIZE)].type = AIR;
							}
							else
							{
								if (randInRange(0, 1000) > SIZE)
								{
									if (pixels[(x - 8) + (y * SIZE)].type == AIR)
									{
										pixels[(x - 8) + (y * SIZE)].type = WATER;
										pixels[x + (y * SIZE)].type = AIR;
										complete[x + (y * SIZE)] = 1;
									}
								}
								else
								{
									if (pixels[(x + 8) + (y * SIZE)].type == AIR)
									{
										pixels[(x + 8) + (y * SIZE)].type = WATER;
										pixels[x + (y * SIZE)].type = AIR;
										complete[x + (y * SIZE)] = 1;
									}
								}
							}
						}
					}
				}
			}
			if (pixels[x + (y * SIZE)].type == SAND && !complete[x + (y * SIZE)])
			{
				if (y + 3 < SIZE - 3)
				{
					if (pixels[x + ((y + 3) * SIZE)].type == AIR)
					{
						pixels[x + ((y + 3) * SIZE)].type = SAND;
						pixels[x + ((y)*SIZE)].type = AIR;
						complete[x + ((y + 3) * SIZE)] = 1;
					}
					else if (pixels[x + ((y + 1) * SIZE)].type == WATER)
					{
						pixels[x + ((y + 1) * SIZE)].type = SAND;
						pixels[x + ((y)*SIZE)].type = WATER;
						complete[x + ((y + 1) * SIZE)] = 1;
					}
					else
					{
						if (x + 1 < SIZE - 1 && x - 1 > 0)
						{
							if (pixels[(x + 1) + ((y + 1) * SIZE)].type == AIR && pixels[(x - 1) + ((y + 1) * SIZE)].type == AIR)
							{
								if (randInRange(0, 10000) > 5000)
								{
									pixels[(x + 2) + ((y + 1) * SIZE)].type = SAND;
									complete[(x + 1) + ((y + 1) * SIZE)] = 1;
									pixels[x + (y * SIZE)].type = AIR;
								}
								else
								{
									pixels[(x - 2) + ((y + 1) * SIZE)].type = SAND;
									complete[(x - 1) + ((y + 1) * SIZE)] = 1;
									pixels[x + (y * SIZE)].type = AIR;
								}
							}
							else if (pixels[(x + 1) + ((y + 1) * SIZE)].type == AIR)
							{
								pixels[(x + 1) + ((y + 1) * SIZE)].type = SAND;
								complete[(x + 1) + ((y + 1) * SIZE)] = 1;
								pixels[x + (y * SIZE)].type = AIR;
							}
							else if (pixels[(x - 1) + ((y + 1) * SIZE)].type == AIR)
							{
								pixels[(x - 1) + ((y + 1) * SIZE)].type = SAND;
								complete[(x - 1) + ((y + 1) * SIZE)] = 1;
								pixels[x + (y * SIZE)].type = AIR;
							}
							if (pixels[(x + 1) + ((y + 1) * SIZE)].type == WATER)
							{
								pixels[(x + 1) + ((y + 1) * SIZE)].type = SAND;
								complete[(x + 1) + ((y + 1) * SIZE)] = 1;
								pixels[x + (y * SIZE)].type = WATER;
							}
							else if (pixels[(x - 1) + ((y + 1) * SIZE)].type == WATER)
							{
								pixels[(x - 1) + ((y + 1) * SIZE)].type = SAND;
								complete[(x - 1) + ((y + 1) * SIZE)] = 1;
								pixels[x + (y * SIZE)].type = WATER;
							}
						}
					}
				}
				else
				{
					complete[x + (y * SIZE)] = 1;
				}
			}
		}
	}
}

int main()
{
	srand(1321 * time(NULL) + 14142);
	printf("%d\n", rand());
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer(SIZE, SIZE, 0, &window, &renderer);
	SDL_SetWindowTitle(window, "Liam's stupid sand game thing");
	pixel *pixels = malloc(SIZE * SIZE * sizeof(pixel));
	for (int i = 0; i != SIZE * SIZE; ++i)
	{
		pixels[i].type = AIR;
	}
	int cursor_type = SAND;
	int paused = 0;
	while (1)
	{
		draw_pixels(renderer, pixels);
		if (!paused)
		{
			update_pixels(pixels);
		}
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_KEYDOWN)
			{
				if (ev.key.keysym.sym == SDLK_a)
				{
					cursor_type = AIR;
				}
				else if (ev.key.keysym.sym == SDLK_s)
				{
					cursor_type = SAND;
				}
				else if (ev.key.keysym.sym == SDLK_w)
				{
					cursor_type = WATER;
				}
				else if (ev.key.keysym.sym == SDLK_c)
				{
					free(pixels);
					pixels = calloc(1, SIZE * SIZE * sizeof(pixel));
				}
				else if (ev.key.keysym.sym == SDLK_p)
				{
					paused = !paused;
				}
			}
			else if (ev.type == SDL_MOUSEMOTION && ev.button.button == SDL_BUTTON_LEFT)
			{
				int x = ev.motion.x;
				int y = ev.motion.y;
				if (x > 0 && (y > 0) && (y < SIZE))
				{
					pixels[x + (y * SIZE)].type = cursor_type;
					if ((x - 1) > 0)
					{
						pixels[(x - 1) + (y * SIZE)].type = cursor_type;
						pixels[(x - 1) + ((y - 1) * SIZE)].type = cursor_type;
					}
					if ((x + 1) < SIZE)
					{
						pixels[(x + 1) + (y * SIZE)].type = cursor_type;
						pixels[(x + 1) + ((y - 1) * SIZE)].type = cursor_type;
					}
					if ((y + 1) < SIZE)
					{
						pixels[(x) + ((y + 1) * SIZE)].type = cursor_type;
					}
					if ((y - 1) > 0)
					{
						pixels[(x) + ((y - 1) * SIZE)].type = cursor_type;
					}
				}
			}
			if (ev.type == SDL_MOUSEMOTION && ev.button.button == SDL_BUTTON_RIGHT)
			{
				int x = ev.motion.x;
				int y = ev.motion.y;
				if (x && y)
				{
					pixels[x + (y * SIZE)].type = AIR;
				}
			}
			if (ev.type == SDL_QUIT)
			{
				return 0;
			}
		}
	}
	return 0;
}
