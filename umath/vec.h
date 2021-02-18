#ifndef VEC_H
#define VEC_H

#include <math.h>

float vec2_add (float c[], float a[], float b[]);
float vec2_sub (float c[], float a[], float b[]);
float vec2_mul (float c[], float a[], float b);
float vec2_div (float c[], float a[], float b);
float vec2_mag (float v[]);
float vec2_mag2 (float v[]);
void vec2_norm (float v[]);

float vec3_add (float c[], float a[], float b[]);
float vec3_sub (float c[], float a[], float b[]);
float vec3_mul (float c[], float a[], float b);
float vec3_div (float c[], float a[], float b);
float vec3_mag (float v[]);
float vec3_mag2 (float v[]);
void vec3_norm (float v[]);

#endif