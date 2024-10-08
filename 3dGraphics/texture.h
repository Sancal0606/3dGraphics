#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include "upng.h"

typedef struct {
	float u;
	float v;
} text2_t;

extern int texture_width;
extern int texture_height;

extern const uint8_t REDBRICK_TEXTURE[];

extern uint32_t* mesh_texture;
extern upng_t* png_texture;

void load_png_texture_data(char* filename);

#endif // !TEXTURE_H
