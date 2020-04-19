#ifndef ___MATRIX_H__
#define ___MATRIX_H__

#include <GLES2/gl2.h>

#define degree2radian(degree) ((degree * M_PI) / 180.0)

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

extern mat4 mat4_identity();
extern mat4 mat4_translate(const GLfloat x, const GLfloat y, const GLfloat z);
extern mat4 mat4_scale(const GLfloat x, const GLfloat y, const GLfloat z);
extern mat4 mat4_rotate(const vec3 axis, const GLfloat rotate);
extern mat4 mat4_multiply(const mat4 a, const mat4 b);


#endif