#ifndef VECTOR_H
#define VECTOR_H
#include <math.h>

typedef struct {
	float x;
	float y;
} vec2_t;

typedef struct {
	float x;
	float y;
	float z;
} vec3_t;

//Vector 2D functions
float vec2_length(vec2_t v);


//Vector 3D functions

float vec3_length(vec3_t v);


vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);


//TODO: Add functions to manipulate vectors 2D and 3D
#endif // !VECTOR_H
