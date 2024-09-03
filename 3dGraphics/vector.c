#include "vector.h"
#include <math.h>

//Vector 2
float vec2_length(vec2_t v) {
	return sqrt(powf(v.x, 2) + powf(v.y, 2));
}

vec2_t vec2_add(vec2_t a, vec2_t b) {
	vec2_t sum_vecs = {
		.x = a.x + b.x,
		.y = a.y + b.y
	};
	return sum_vecs;
}


//Vector3
float vec3_length(vec3_t v) {
	return sqrt(powf(v.x, 2) + powf(v.y, 2) + powf(v.z,2));
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
	vec3_t sum_vecs = {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	};
	return sum_vecs;
}

vec3_t vec3_rotate_x(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x,
		.y = v.y * cos(angle) + v.z * sin(angle),
		.z = v.z * cos(angle) - v.y * sin(angle)
	};
	return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) - v.z * sin(angle),
		.y = v.y,
		.z = v.z * cos(angle) + v.x * sin(angle)
	};
	return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) - v.y * sin(angle),
		.y = v.y * cos(angle) + v.x * sin(angle),
		.z = v.z
	};
	return rotated_vector;

}