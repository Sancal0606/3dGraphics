#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include<stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000/FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* color_buffer_texture;
extern uint32_t* color_buffer;
extern int window_width;
extern int window_height;


bool initialize_window(void);
void draw_grid(void);
void draw_rectangle(int posX, int posY, int rectangle_width, int rectangle_height, uint32_t color);
void draw_pixel(int x, int y, uint32_t color);
void clear_color_buffer(uint32_t color);
void render_color_buffer();
void destroy_window(void);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

#endif // !DISPLAY_H
