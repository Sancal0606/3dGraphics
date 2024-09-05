#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#include "array.h"
#include "mesh.h"
#include "matrix.h"

triangle_t* triangles_to_render = NULL;
float fov_factor = 640;
bool is_running = false;
vec3_t camera_position = {.x = 0,.y = 0,.z = 0.0};

float previous_frame_time = 0.0;
bool back_face_culling = true;
bool draw_wireframe = true;
bool draw_vertex = true;
bool draw_solid = true;

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
	load_cube_mesh_data();
	//load_obj_file_data("./assets/cube.obj");

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
			if (event.key.keysym.sym == SDLK_c) {
				back_face_culling = !back_face_culling;
			}
			if (event.key.keysym.sym == SDLK_1) {
				draw_wireframe = true;
				draw_vertex = true;
				draw_solid = false;
			}
			if (event.key.keysym.sym == SDLK_2) {
				draw_wireframe = true;
				draw_vertex = false;
				draw_solid = false;
			}
			if (event.key.keysym.sym == SDLK_3) {
				draw_wireframe = false;
				draw_vertex = false;
				draw_solid = true;
			}
			if (event.key.keysym.sym == SDLK_4) {
				draw_wireframe = true;
				draw_vertex = false;
				draw_solid = true;
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



int partition( int low, int high) {
	float pivot_value = triangles_to_render[high].avg_depth;

	int i = low - 1;
	for (int j = low; j <= high - 1; j++)
	{
		if (triangles_to_render[j].avg_depth > pivot_value) {
			i++;
			triangle_t temp = triangles_to_render[i];
			triangles_to_render[i] = triangles_to_render[j];
			triangles_to_render[j] = temp;
		}
	}

	triangle_t temp = triangles_to_render[i + 1];
	triangles_to_render[i + 1] = triangles_to_render[high];
	triangles_to_render[high] = temp;
	return i + 1;
}

void quick_sort(int low, int high) {
	if (low < high) {
		int partition_index = partition(low, high);
		quick_sort(low, partition_index - 1);
		quick_sort(partition_index + 1, high);
	}
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

	mesh.scale.x += 0.002;
	//Create a scale matrix that will be used to multiply the mesh vertices
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);

	int num_faces = array_length(mesh.faces);
	//Loop all triangle faces of our mesh
	for (int i = 0; i < num_faces; i++)
	{
		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		

		vec4_t transformed_vertices[3];
		
		//Loop all three vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++)
		{
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

			//Use a matrix to scale our original vertex
			transformed_vertex = mat4_mul_vec4(scale_matrix, transformed_vertex);

			//Translate the vertex away from the camera
			transformed_vertex.z += 5;
			transformed_vertices[j] = transformed_vertex;
		}
		
		vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]);
		vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]);
		vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]);

		vec3_t vector_ab = vec3_sub(vector_b, vector_a);
		vec3_normalize(&vector_ab);

		vec3_t vector_ac = vec3_sub(vector_c, vector_a);
		vec3_normalize(&vector_ac);

		vec3_t normal_vector = vec3_cross(vector_ab, vector_ac);
		vec3_normalize(&normal_vector);
		vec3_t camera_ray = vec3_sub(camera_position, vector_a);

		float dot_product = vec3_dot(normal_vector, camera_ray);
		if (dot_product < 0 && back_face_culling) {
			continue;
		}
		
		vec2_t projected_points[3];

		for (int j = 0; j < 3; j++)
		{
			projected_points[j] = project(vec3_from_vec4(transformed_vertices[j]));
			//Scale anf translate the projected point to the middle of the screen
			projected_points[j].x += (window_width / 2);
			projected_points[j].y += (window_height / 2);
			
		}

		//Calculate the average depth for each face based on the vertices after transformation
		float avg_depth = (float)(transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3;

		triangle_t projected_triangle = {
			.points = {
				projected_points[0].x, projected_points[0].y,
				projected_points[1].x, projected_points[1].y,
				projected_points[2].x, projected_points[2].y,
			},
			.color = mesh_face.color,
			.avg_depth = avg_depth
			
		};
		//Save the projected triangle in the array of triangles to render
		//triangles_to_render[i] = projected_triangle;
		array_push(triangles_to_render, projected_triangle);
	}

	//Sort the triangles to render by their avg_depth
	quick_sort(0, array_length(triangles_to_render) - 1);
	
	
}

void render(void) {

	//draw_grid();
	int num_triangles = array_length(triangles_to_render);
	//Loop all projected triangles and render then
	for (int i = 0; i < num_triangles; i++)
	{
		triangle_t triangle = triangles_to_render[i];
		//Draw Vertex Points
		if (draw_vertex) {
			draw_rectangle(triangle.points[0].x, triangle.points[0].y, 9, 9, 0xFFFF0000);
			draw_rectangle(triangle.points[1].x, triangle.points[1].y, 9, 9, 0xFFFF0000);
			draw_rectangle(triangle.points[2].x, triangle.points[2].y, 9, 9, 0xFFFF0000);
		}
		if (draw_solid) {
			draw_filled_triangle(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				triangle.color
			);
		}
		if (draw_wireframe) {
			draw_triangle(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				0xFFFFFFF
			);
		}
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