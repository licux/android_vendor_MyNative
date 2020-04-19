#ifndef ___SPRITE_H__
#define ___SPRITE_H__

#include "Matrix.h"

extern mat4 Sprite_createPositionMatrix(const int surface_width, const int surface_height, const GLint x, const GLint y,
                                        const GLint width, const GLint height, const GLfloat rotate);

extern mat4 Sprite_createUvMatrix(const int texture_width, const int texture_height, GLint x, GLint y, const GLint width, const GLint height);

#endif // ___SPRITE_H__