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

vec2_t vec2_sub(vec2_t a, vec2_t b) {
	vec2_t sub_vecs = {
		.x = a.x - b.x,
		.y = a.y - b.y
	};
	return sub_vecs;
}

vec2_t vec2_mul(vec2_t v, float factor) {
	vec2_t result = {
		.x = v.x * factor,
		.y = v.y * factor
	};
	return result;
}

vec2_t vec2_div(vec2_t v, float factor) {
	vec2_t result = {
		.x = v.x / factor,
		.y = v.y / factor
	};
	return result;
}

float vec2_dot(vec2_t a, vec2_t b) {
	return (a.x * b.x) + (a.y * b.y);
}

void vec2_normalize(vec2_t* v) {
	float length = sqrt(v->x * v->x + v->y * v->y);
	v->x /= length;
	v->y /= length;
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

vec3_t vec3_sub(vec3_t a, vec3_t b) {
	vec3_t sub_vecs = {
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	};
	return sub_vecs;
}

vec3_t vec3_mul(vec3_t v, float factor) {
	vec3_t result = {
		.x = v.x * factor,
		.y = v.y * factor,
		.z = v.z * factor
	};
	return result;
}

vec3_t vec3_div(vec3_t v, float factor) {
	vec3_t result = {
		.x = v.x / factor,
		.y = v.y / factor,
		.z = v.z / factor
	};
	return result;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
	vec3_t result = {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};
	return result;
}

float vec3_dot(vec3_t a, vec3_t b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void vec3_normalize(vec3_t* v) {
	
	float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x /= length;
	v->y /= length;
	v->z /= length;
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

vec4_t vec4_from_vec3(vec3_t v) {
	vec4_t result = { v.x,v.y,v.z, 1 };
	return result;
}

vec3_t vec3_from_vec4(vec4_t v) {
	vec3_t result = { v.x,v.y,v.z };
	return result;
}

vec2_t vec2_from_vec4(vec4_t v) {
	vec2_t result = { v.x,v.y };
	return result;
}
