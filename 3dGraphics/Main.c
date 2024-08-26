#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#define N_POINTS (9 * 9 * 9)

vec3_t cube_points[N_POINTS]; 
bool is_running = false;

void setup(void) {
	//Allocate the required memory in bytes to hold the color buffer
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
	//Creating a SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);


	int point_count = 0;
	//Start loading my array of vectors
	//From -1 to 1 in this 9x9x9 cube
	for (float x = -1; x <= 1 ; x+= 0.25){
		for (float y = -1; y <= 1; y += 0.25){
			for (float z = -1; z <= 1; z += 0.25){
				vec3_t new_point = {.x = x,.y = y,.z = z};
				cube_points[point_count] = new_point;
				point_count++;
			}
		}
	}
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				is_running = false;
			}
			break;
	}
}

void update(void) {

}



void render(void) {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderClear(renderer);
	//draw_grid();
	draw_rectangle(300, 300, 300, 500, 0xFFFF0000);
	draw_pixel(5, 5, 0xFFFFFF);
	render_color_buffer();
	clear_color_buffer(0xFF000000);
	SDL_RenderPresent(renderer);
}



int main(int argc, char* args[]) {
	is_running = initialize_window();

	vec3_t my_vector = { 0.1, 0.1, 0.1 };
	//Game loop
	setup();
	while (is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();
	return 0;
}