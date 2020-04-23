#ifndef ___MATRIX_H__
#define ___MATRIX_H__

#include <GLES2/gl2.h>

#define degree2radian(degree) ((degree * M_PI) / 180.0)

typedef struct vec2{
    GLfloat x;
    GLfloat y;
}vec2;

typedef struct vec3{
    GLfloat x;
    GLfloat y;
    GLfloat z;
}vec3;

typedef struct mat4{
    GLfloat m[4][4];
} mat4;

extern vec3 vec3_create(const GLfloat x, const GLfloat y, const GLfloat z);
extern GLfloat vec3_length(const vec3 v);
extern vec3 vec3_normalize(const vec3 v);
extern vec3 vec3_createNormalized(const GLfloat x, const GLfloat y, const GLfloat z);
extern GLfloat vec3_dot(const vec3 v0, const vec3 v1);
extern vec3 vec3_cross(const vec3 v0, const vec3 v1);

extern mat4 mat4_identity();
extern mat4 mat4_translate(const GLfloat x, const GLfloat y, const GLfloat z);
extern mat4 mat4_scale(const GLfloat x, const GLfloat y, const GLfloat z);
extern mat4 mat4_rotate(const vec3 axis, const GLfloat rotate);
extern mat4 mat4_multiply(const mat4 a, const mat4 b);

extern mat4 mat4_lookAt(const vec3 eye, const vec3 look, const vec3 up);
extern mat4 mat4_perspective(const GLfloat near, const GLfloat far, const GLfloat fovY_degree, const GLfloat aspect);

#endif