#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 500

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
	GLOWINGSTUFF
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
			int r, g, b;
			if (pixels[x + (y * SIZE)].type == AIR)
			{
				r = 120;
				g = 180;
				b = 255;
			}
			else if (pixels[x + (y * SIZE)].type == WATER)
			{
				r = 10;
				g = 30;
				b = 255;
			}
			else if (pixels[x + (y * SIZE)].type == GLOWINGSTUFF)
			{
				r = 250;
				g = 250;
				b = 140;
			}
			else
			{
				r = 150;
				g = 80;
				b = 0;
			}
			if (y > 1 && pixels[x + (y * SIZE)].type != GLOWINGSTUFF && pixels[x + (y * SIZE)].type != AIR)
			{
				r += 120;
				g += 120;
				b += 120;
				int start_y = y;
				int i = start_y;
				while (i > 1 && i > (start_y - 15) && pixels[x + (i * SIZE)].type != GLOWINGSTUFF)
				{
					r -= 8;
					g -= 8;
					b -= 8;
					--i;
				}
			}
			if (y < SIZE && pixels[x + (y * SIZE)].type != GLOWINGSTUFF && pixels[x + (y * SIZE)].type != AIR)
			{
				r += 120;
				g += 120;
				b += 120;
				int start_y = y;
				int i = start_y;
				while (i < SIZE && i < (start_y + 15) && pixels[x + (i * SIZE)].type != GLOWINGSTUFF)
				{
					r -= 8;
					g -= 8;
					b -= 8;
					++i;
				}
			}
			if (x < SIZE && pixels[x + (y * SIZE)].type != GLOWINGSTUFF && pixels[x + (y * SIZE)].type != AIR)
			{
				r += 120;
				g += 120;
				b += 120;
				int start_x = x;
				int i = start_x;
				while (i < SIZE && i < (start_x + 15) && pixels[i + (y * SIZE)].type != GLOWINGSTUFF)
				{
					r -= 8;
					g -= 8;
					b -= 8;
					++i;
				}
			}
			if (x > 1 && pixels[x + (y * SIZE)].type != GLOWINGSTUFF && pixels[x + (y * SIZE)].type != AIR)
			{
				r += 120;
				g += 120;
				b += 120;
				int start_x = x;
				int i = start_x;
				while (i > 1 && i > (start_x - 15) && pixels[i + (y * SIZE)].type != GLOWINGSTUFF)
				{
					r -= 8;
					g -= 8;
					b -= 8;
					--i;
				}
			}
			if (x < SIZE && y < SIZE && pixels[x + (y * SIZE)].type != GLOWINGSTUFF && pixels[x + (y * SIZE)].type != AIR)
			{
				r += 120;
				g += 120;
				b += 120;
				int i = 0;
				int x1 = x;
				int y1 = y;
				while (x1 < SIZE && y1 < SIZE && i < 10 && pixels[x1 + (y1 * SIZE)].type != GLOWINGSTUFF)
				{
					++y1;
					++x1;
					r -= 12;
					g -= 12;
					b -= 12;
					++i;
				}
			}
			if (x > 1 && y > 1 && pixels[x + (y * SIZE)].type != GLOWINGSTUFF && pixels[x + (y * SIZE)].type != AIR)
			{
				r += 120;
				g += 120;
				b += 120;
				int i = 0;
				int x1 = x;
				int y1 = y;
				while (x1 > 1 && y1 > 1 && i < 10 && pixels[x1 + (y1 * SIZE)].type != GLOWINGSTUFF)
				{
					--y1;
					--x1;
					r -= 12;
					g -= 12;
					b -= 12;
					++i;
				}
			}
			if (x > 1 && y < SIZE && pixels[x + (y * SIZE)].type != GLOWINGSTUFF && pixels[x + (y * SIZE)].type != AIR)
			{
				r += 120;
				g += 120;
				b += 120;
				int i = 0;
				int x1 = x;
				int y1 = y;
				while (x1 > 1 && y1 < SIZE && i < 10 && pixels[x1 + (y1 * SIZE)].type != GLOWINGSTUFF)
				{
					++y1;
					--x1;
					r -= 12;
					g -= 12;
					b -= 12;
					++i;
				}
			}
			if (x < SIZE && y > 1 && pixels[x + (y * SIZE)].type != GLOWINGSTUFF && pixels[x + (y * SIZE)].type != AIR)
			{
				r += 120;
				g += 120;
				b += 120;
				int i = 0;
				int x1 = x;
				int y1 = y;
				while (x1 < SIZE && y1 > 1 && i < 10 && pixels[x1 + (y1 * SIZE)].type != GLOWINGSTUFF)
				{
					--y1;
					++x1;
					r -= 12;
					g -= 12;
					b -= 12;
					++i;
				}
			}
			r = (r > 255) ? 255 : r;
			g = (g > 255) ? 255 : g;
			b = (b > 255) ? 255 : b;
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
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
			if (!complete[x + (y * SIZE)])
			{
				switch (pixels[x + (y * SIZE)].type)
				{
				case AIR:
				{
					continue;
				}
				break;
				case WATER:
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
				break;
				case SAND:
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
				break;
				case GLOWINGSTUFF:
				{
					if (y + 3 < SIZE - 3)
					{
						if (pixels[x + ((y + 3) * SIZE)].type == AIR)
						{
							pixels[x + ((y + 3) * SIZE)].type = GLOWINGSTUFF;
							pixels[x + ((y)*SIZE)].type = AIR;
							complete[x + ((y + 3) * SIZE)] = 1;
						}
						else if (pixels[x + ((y + 1) * SIZE)].type == WATER)
						{
							pixels[x + ((y + 1) * SIZE)].type = GLOWINGSTUFF;
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
										pixels[(x + 2) + ((y + 1) * SIZE)].type = GLOWINGSTUFF;
										complete[(x + 1) + ((y + 1) * SIZE)] = 1;
										pixels[x + (y * SIZE)].type = AIR;
									}
									else
									{
										pixels[(x - 2) + ((y + 1) * SIZE)].type = GLOWINGSTUFF;
										complete[(x - 1) + ((y + 1) * SIZE)] = 1;
										pixels[x + (y * SIZE)].type = AIR;
									}
								}
								else if (pixels[(x + 1) + ((y + 1) * SIZE)].type == AIR)
								{
									pixels[(x + 1) + ((y + 1) * SIZE)].type = GLOWINGSTUFF;
									complete[(x + 1) + ((y + 1) * SIZE)] = 1;
									pixels[x + (y * SIZE)].type = AIR;
								}
								else if (pixels[(x - 1) + ((y + 1) * SIZE)].type == AIR)
								{
									pixels[(x - 1) + ((y + 1) * SIZE)].type = GLOWINGSTUFF;
									complete[(x - 1) + ((y + 1) * SIZE)] = 1;
									pixels[x + (y * SIZE)].type = AIR;
								}
								if (pixels[(x + 1) + ((y + 1) * SIZE)].type == WATER)
								{
									if (randInRange(1, 10000) > 9000)
									{
										pixels[(x + 1) + ((y + 1) * SIZE)].type = GLOWINGSTUFF;
										complete[(x + 1) + ((y + 1) * SIZE)] = 1;
										pixels[x + (y * SIZE)].type = WATER;
									}
								}
								else if (pixels[(x - 1) + ((y + 1) * SIZE)].type == WATER)
								{
									if (randInRange(1, 10000) > 9000)
									{
										pixels[(x - 1) + ((y + 1) * SIZE)].type = GLOWINGSTUFF;
										complete[(x - 1) + ((y + 1) * SIZE)] = 1;
										pixels[x + (y * SIZE)].type = WATER;
									}
								}
							}
						}
					}
					else
					{
						complete[x + (y * SIZE)] = 1;
					}
				}
				break;
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
				else if (ev.key.keysym.sym == SDLK_l)
				{
					cursor_type = GLOWINGSTUFF;
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
