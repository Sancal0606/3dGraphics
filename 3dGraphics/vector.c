#include "vector.h"
#include <math.h>

//Vector 2
float vec2_length(vec2_t v) {
	return sqrt(powf(v.x, 2) + powf(v.y, 2));
}



//Vector3
float vec3_length(vec3_t v) {
	return sqrt(powf(v.x, 2) + powf(v.y, 2) + powf(v.z,2));
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