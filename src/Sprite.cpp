#include "Sprite.h"

mat4 Sprite_createPositionMatrix(const int surface_width, const int surface_height, const GLint x, const GLint y,
                                        const GLint width, const GLint height, const GLfloat rotate){
    const GLfloat surfaceAspect = (GLfloat)surface_width / (GLfloat)surface_height;
    const mat4 aspect = mat4_scale(1, surfaceAspect, 1);

    const GLfloat xScale = (GLfloat)width / (GLfloat)surface_width * 2.0f;
    const GLfloat yScale = (GLfloat)height / (GLfloat)surface_width * 2.0f;

    const mat4 scale = mat4_scale(xScale, yScale, 0);

    const GLfloat vertexLeft = 0.5f * (1.0f - xScale) * 0.5f;
    const GLfloat vertexTop = 0.5f + (1.0f - (yScale * surfaceAspect)) * 0.5f;
    const GLfloat moveX = (GLfloat)x / (GLfloat)surface_width * 2.0f;
    const GLfloat moveY = -((GLfloat)y / (GLfloat)surface_height * 2.0f);

    const mat4 translate = mat4_translate(-vertexLeft + moveX, vertexTop + moveY, 0);

    const mat4 rotateM = mat4_rotate(vec3_create(0, 0, 1), rotate);

    mat4 matrix = mat4_multiply(translate, aspect);
    matrix = mat4_multiply(matrix, rotateM);
    matrix = mat4_multiply(matrix, scale);

    return matrix;
}

mat4 Sprite_createUvMatrix(const int texture_width, const int texture_height, GLint x, GLint y, const GLint width, const GLint height){
    const GLfloat xScale = (GLfloat)width / (GLfloat)texture_width;
    const GLfloat yScale = (GLfloat)height / (GLfloat)texture_height;

    const mat4 scale = mat4_scale(xScale, yScale, 0);

    const GLfloat xMove = (GLfloat)x / (GLfloat)texture_width;
    const GLfloat yMove = (GLfloat)y / (GLfloat)texture_height;
    const mat4 translate = mat4_translate(xMove, yMove, 0);

    return mat4_multiply(translate, scale);
}
