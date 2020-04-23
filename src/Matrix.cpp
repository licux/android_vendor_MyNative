#include <math.h>
#include <string>

#include "Matrix.h"

vec3 vec3_create(const GLfloat x, const GLfloat y, const GLfloat z){
    vec3 v = {x, y, z};
    return v;
}

GLfloat vec3_length(const vec3 v){
    return (GLfloat)sqrt(((double)v.x * (double)v.x)
                            + ((double)v.y * (double)v.y)
                            + ((double)v.z * (double)v.z));
}

vec3 vec3_normalize(const vec3 v){
    const GLfloat len = vec3_length(v);
    return vec3_create(v.x / len, v.y / len, v.z / len);
}

vec3 vec3_createNormalized(const GLfloat x, const GLfloat y, const GLfloat z){
    return vec3_normalize(vec3_create(x, y, z));
}

GLfloat vec3_dot(const vec3 v0, const vec3 v1){
    return (v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z);
}

vec3 vec3_cross(const vec3 v0, const vec3 v1){
    return vec3_create((v0.y * v1.z) - (v0.z * v1.y), (v0.z * v1.x) - (v0.x * v1.z), (v0.x * v1.y) - (v0.y * v1.x));
}

mat4 mat4_identity(){
    mat4 result;

    int column = 0;
    int row = 0;

    for(column = 0; column < 4; column++){
        for(row = 0; row < 4; row++){
            if(column == row){
                result.m[column][row] = 1.0f;
            }else{
                result.m[column][row] = 0.0f;
            }
        }
    }

    return  result;
}

mat4 mat4_translate(const GLfloat x, const GLfloat y, const GLfloat z){
    mat4 result = mat4_identity();

    result.m[3][0] = x;
    result.m[3][1] = y;
    result.m[3][2] = z;

    return result;
}

mat4 mat4_scale(const GLfloat x, const GLfloat y, const GLfloat z){
    mat4 result = mat4_identity();

    result.m[0][0] = x;
    result.m[1][1] = y;
    result.m[2][2] = z;

    return result;
}

mat4 mat4_rotate(const vec3 axis, const GLfloat rotate){
    mat4 result;

    const GLfloat axis_x = axis.x;
    const GLfloat axis_y = axis.y;
    const GLfloat axis_z = axis.z;

    const GLfloat c = cos(degree2radian(rotate));
    const GLfloat s = sin(degree2radian(rotate));
    {
        result.m[0][0] = (axis_x * axis_x) * (1.0f - c) + c;
        result.m[0][1] = (axis_x * axis_y) * (1.0f - c) - axis_z * s;
        result.m[0][2] = (axis_x * axis_z) * (1.0f - c) + axis_y * s;
        result.m[0][3] = 0;
    }
    {
        result.m[1][0] = (axis_y * axis_x) * (1.0f - c) + axis_z * s;
        result.m[1][1] = (axis_y * axis_y) * (1.0f - c) + c;
        result.m[1][2] = (axis_y * axis_z) * (1.0f - c) + axis_x * s;
        result.m[1][3] = 0;
    }
    {
        result.m[2][0] = (axis_z * axis_x) * (1.0f - c) - axis_y * s;
        result.m[2][1] = (axis_z * axis_y) * (1.0f - c) + axis_x * s;
        result.m[2][2] = (axis_z * axis_z) * (1.0f - c) + c;
        result.m[2][3] = 0;
    }
    {
        result.m[3][0] = 0;
        result.m[3][1] = 0;
        result.m[3][2] = 0;
        result.m[3][3] = 1;
    }

    return result;
}

mat4 mat4_multiply(const mat4 a, const mat4 b){
    mat4 result;

    for(int i = 0; i < 4; i++){
        result.m[i][0] = a.m[0][0] * b.m[i][0] + a.m[1][0] * b.m[i][1]
                       + a.m[2][0] * b.m[i][2] + a.m[3][0] * b.m[i][3];
        result.m[i][1] = a.m[0][1] * b.m[i][0] + a.m[1][1] * b.m[i][1]
                       + a.m[2][1] * b.m[i][2] + a.m[3][1] * b.m[i][3];
        result.m[i][2] = a.m[0][2] * b.m[i][0] + a.m[1][2] * b.m[i][1]
                       + a.m[2][2] * b.m[i][2] + a.m[3][2] * b.m[i][3];
        result.m[i][3] = a.m[0][3] * b.m[i][0] + a.m[1][3] * b.m[i][1]
                       + a.m[2][3] * b.m[i][2] + a.m[3][3] * b.m[i][3];
    }

    return result;
}

mat4 mat4_lookAt(const vec3 eye, const vec3 look, const vec3 up){
    // mat4 result;

    // vec3 f = vec3_normalize(vec3_create(look.x - eye.x, look.y - eye.y, look.z - eye.z));
    // vec3 u = vec3_normalize(up);
    // vec3 s = vec3_normalize(vec3_cross(f, u));
    // u = vec3_cross(s, f);

    // result.m[0][0] = s.x;
    // result.m[1][0] = s.y;
    // result.m[2][0] = s.z;
    // result.m[0][1] = u.x;
    // result.m[1][1] = u.y;
    // result.m[2][1] = u.z;
    // result.m[0][2] = -f.x;
    // result.m[1][2] = -f.y;
    // result.m[2][2] = -f.z;
    // result.m[3][0] = -vec3_dot(s, eye);
    // result.m[3][1] = -vec3_dot(u, eye);
    // result.m[3][2] = vec3_dot(f, eye);
    // result.m[0][3] = 0;
    // result.m[1][3] = 0;
    // result.m[2][3] = 0;
    // result.m[3][3] = 1;

    // return result;

    mat4 result;

    vec3 f = vec3_normalize(vec3_create(look.x - eye.x, look.y - eye.y, look.z - eye.z));
    vec3 u = vec3_normalize(up);
    vec3 s = vec3_normalize(vec3_cross(f, u));
    u = vec3_cross(s, f);

    result.m[0][0] = s.x;
    result.m[1][0] = s.y;
    result.m[2][0] = s.z;
    result.m[0][1] = u.x;
    result.m[1][1] = u.y;
    result.m[2][1] = u.z;
    result.m[0][2] = -f.x;
    result.m[1][2] = -f.y;
    result.m[2][2] = -f.z;
    result.m[3][0] = -vec3_dot(s, eye);
    result.m[3][1] = -vec3_dot(u, eye);
    result.m[3][2] = vec3_dot(f, eye);
    result.m[0][3] = 0;
    result.m[1][3] = 0;
    result.m[2][3] = 0;
    result.m[3][3] = 1;

    return result;
}

mat4 mat4_perspective(const GLfloat near, const GLfloat far, const GLfloat fovY_degree, const GLfloat aspect){
    mat4 result;
    memset(result.m, 0x00, sizeof(mat4));

    const GLfloat f = (GLfloat)(1.0 / (tan(degree2radian(fovY_degree)) / 2.0));

    result.m[0][0] = f / aspect;
    result.m[1][1] = f;
    result.m[2][2] = (far + near) / (near - far);
    result.m[2][3] = -1;
    result.m[3][2] = (2.0f * far * near) / (near - far);

    return result;
}