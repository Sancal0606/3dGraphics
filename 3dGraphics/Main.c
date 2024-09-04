#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#include "array.h"
#include "mesh.h"

triangle_t* triangles_to_render = NULL;
float fov_factor = 250;
bool is_running = false;
vec3_t camera_position = {.x = 0,.y = 0,.z = 0.0};

float previous_frame_time = 0.0;

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

	//Loads the cube variables in the mesh data structure
	//load_cube_mesh_data();
	load_obj_file_data("./assets/cube.obj");

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

//Function that receives a 3D vector and returns a projected 2D point
vec2_t project(vec3_t point) {
	vec2_t projected_point = {
		.x = (fov_factor * point.x) / point.z,
		.y = (fov_factor * point.y) / point.z,
	};
	return projected_point;
}

void update(void) {
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait >= 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));
	previous_frame_time = SDL_GetTicks();

	//Initialize the array of triangles to render
	triangles_to_render = NULL;

	mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.01;

	int num_faces = array_length(mesh.faces);
	//Loop all triangle faces of our mesh
	for (int i = 0; i < num_faces; i++)
	{
		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		triangle_t projected_triangle;

		vec3_t transformed_vertices[3];
		
		//Loop all three vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++)
		{
			vec3_t transformed_vertex = face_vertices[j];

			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			//Translate the vertex away from the camera
			transformed_vertex.z += 5;
			transformed_vertices[j] = transformed_vertex;
		}
		
		vec3_t vector_a = transformed_vertices[0];
		vec3_t vector_b = transformed_vertices[1];
		vec3_t vector_c = transformed_vertices[2];

		vec3_t vector_ab = vec3_sub(vector_b, vector_a);
		vec3_t vector_ac = vec3_sub(vector_c, vector_a);

		vec3_t normal_vector = vec3_cross(vector_ab, vector_ac);
		vec3_t camera_ray = vec3_sub(camera_position, vector_a);

		float dot_product = vec3_dot(normal_vector, camera_ray);
		if (dot_product < 0) {
			continue;
		}

		for (int j = 0; j < 3; j++)
		{
			vec2_t projected_point = project(transformed_vertices[j]);
			//Scale anf translate the projected point to the middle of the screen
			projected_point.x += (window_width / 2);
			projected_point.y += (window_height / 2);
			projected_triangle.points[j] = projected_point;
		}
		//Save the projected triangle in the array of triangles to render
		//triangles_to_render[i] = projected_triangle;
		array_push(triangles_to_render, projected_triangle);
	}	
}

void render(void) {

	//draw_grid();
	int num_triangles = array_length(triangles_to_render);
	//Loop all projected triangles and render then
	for (int i = 0; i < num_triangles; i++)
	{
		triangle_t triangle = triangles_to_render[i];
		//Draw Vertex Points
		//draw_rectangle(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFF00);
		//draw_rectangle(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFF00);
		//draw_rectangle(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFF00);
		draw_triangle(
			triangle.points[0].x, triangle.points[0].y,
			triangle.points[1].x, triangle.points[1].y, 
			triangle.points[2].x, triangle.points[2].y,
			0xFF00FF00
		);
	}
	
	//Clear the array of triangles to render every frame loop
	array_free(triangles_to_render);
	render_color_buffer();
	clear_color_buffer(0xFF000000);
	SDL_RenderPresent(renderer);
}

//Free memory that was dynamically allocated by the program
void free_resources(void) {
	array_free(mesh.faces);
	array_free(mesh.vertices);
	free(color_buffer);
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
	free_resources();
	return 0;
}