#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* color_buffer_texture = NULL;
uint32_t* color_buffer = NULL;
int window_width = 800;
int window_height = 600;
float* z_buffer = NULL;
#include <stdio.h> 
#include <stdlib.h>

bool initialize_window(void) {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL \n");
		return false;
	}

	//Use SDL to query what is the fullscreen max width and height
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	window_width = display_mode.w;
	window_height = display_mode.h;

	//Create Window
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		fprintf(stderr, "Error creating SDL windown \n");
		return false;
	}

	//Create SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer \n");
		return false;
	}

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}


void draw_grid(void) {
	for (int y = 0; y < window_height; y += 10)
	{
		for (int x = 0; x < window_width; x += 10)
		{
			color_buffer[(window_width * y) + x] = 0xFFFFFFFF;
		}
	}
}

void draw_pixel(int x, int y, uint32_t color) {
	if (x>=0 && x < window_width && y >= 0 && y < window_height) {
		color_buffer[(window_width * y) + x] = color;
	}
}


void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);

	int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / (float)longest_side_length;
	float y_inc = delta_y / (float)longest_side_length;

	float current_x = x0;
	float current_y = y0;

	int length = snprintf(NULL, 0, "%g", x_inc);
	char* str = malloc(length + 1);
	snprintf(str, length + 1, "%g", x_inc);

	for (int i = 0; i <= longest_side_length; i++)
	{
		draw_pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}
}


void draw_rectangle(int posX, int posY, int rectangle_width, int rectangle_height, uint32_t color) {
	for (int y = posY; y < rectangle_height + posY; y++)
	{
		for (int x = posX; x < rectangle_width + posX; x++)
		{
			draw_pixel(x, y, color);
			//color_buffer[(window_width * y) + x] = color;
		}
	}
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	draw_line(x0, y0, x1, y1, color);
	draw_line(x0, y0, x2, y2, color);
	draw_line(x1, y1, x2, y2, color);
}


void clear_color_buffer(uint32_t color) {
	for (int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)
		{
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

void clear_z_buffer(void) {
	for (int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)
		{
			z_buffer[(window_width * y) + x] = 1.0;
		}
	}
}

void render_color_buffer() {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(sizeof(uint32_t) * window_width)
	);
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}


void destroy_window(void) {
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}