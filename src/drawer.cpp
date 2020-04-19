#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "GLEngine.h"
#include "Matrix.h"
#include "Sprite.h"

#include <android/log.h>
#include <math.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))

void draw_triangle(GLEngine *engine){

    const GLfloat position[] = {
        0.0f, 1.0f,
        1.0f, -1.0f,
        -1.0f, -1.0f,
    };

    const GLubyte color[] = {
        255, 0, 0,
        0, 255, 0,
        0, 0, 255,
    };

    glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);
    glVertexAttribPointer(engine->attr_color, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, color);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void draw_rectangle(GLEngine *engine){

    // draw cross
    {
        glLineWidth(1.0f);

        const GLfloat position[] = {
            -1.0f, 0.0f,
            1.0f, 0.0f,

            0.0f, 1.0f,

            0.0f, -1.0f
        };

        glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);

        glDrawArrays(GL_LINES, 0, 4);
    }

    // draw コ
    {
        glLineWidth(5.0f);

        const GLfloat position[] = {
            -0.5f, 0.5f,
            0.5f, 0.5f,
            0.5f, -0.5f,
            -0.5f, -0.5f,
        };

        glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);

        glDrawArrays(GL_LINE_STRIP, 0, 4);
    }

    // draw rectangle
    {
        glLineWidth(10.0f);

        const GLfloat position[] = {
            -0.75f, 0.75f,
            0.75f, 0.75f,
            0.75f, -0.75f,
            -0.75f, -0.75f,
        };

        glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);

        glDrawArrays(GL_LINE_LOOP, 0, 4);
    }
    
}

void draw_point(GLEngine *engine){

    // draw point
    {
        const GLfloat position[] = {
            0.0f, 0.0f,
            0.75f, -0.75f,
        };

        glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);

        glDrawArrays(GL_POINTS, 0, 2);
    }
}

void draw_qued(GLEngine *engine){
    // left top
    {
        const GLfloat position[] = {
            -0.75f, 0.75f,
            -0.75f, 0.25f,
            -0.25f, 0.75f,

            -0.25f, 0.75f,
            -0.75f, 0.25f,
            -0.25f, 0.25f,
        };
        {
            glUniform4f(engine->unif_color, 1.0f, 0.0f, 0.0f, 1.0f);
        }
        glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // right top
    {
        const GLfloat position[] ={
            0.25f, 0.75f,
            0.25f, 0.25f,
            0.75f, 0.75f,
            0.75f, 0.25f,
        };

        {
            glUniform4f(engine->unif_color, 1.0f, 0.0f, 1.0f, 1.0f);
        }
        glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // left donw
    {
        const GLfloat position[] = {
            -0.25f, -0.25f,
            -0.75f, -0.25f,
            -0.75f, -0.75f,
            -0.25f, -0.75f,
        };

        {
            const GLfloat poly_color[] = {
                1.0f, 1.0f, 0.0f, 1.0f,
            };
            glUniform4fv(engine->unif_color, 1, poly_color);
        }
        glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}

float x = 0;
float y = 0;

void draw_move(GLEngine *engine)
{
    const GLfloat position[] = {
        0.0f, 0.5f,
        0.0f, 0.0f,
        0.5f, 0.5f,
        0.5f, 0.0f,
    };

    glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);

    {
        
        x += 0.01f;
        y += 0.01f;

        if(x > 1.0f){
            x = -1;
        }
        if(y > 1.0f){
            y = -1;
        }

        glUniform2f(engine->unif_pos, (GLfloat)x, (GLfloat)y);
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void draw_texture(GLEngine *engine){
    LOGI("start draw_texture");
    glUniform1i(engine->unif_texture, 0);
LOGI("start draw_texture2");
    {
        const GLfloat position[] = {
            -0.75f, 0.75f,
            -0.75f, -0.75f,
            0.75f, 0.75f,
            0.75f, -0.75f, 
        };

        const GLfloat uv[] = {
            0, 0,
            0, 1,
            1, 0,
            1, 1,
        };

        glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);
        LOGI("start draw_texture3");
        glVertexAttribPointer(engine->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)uv);
        LOGI("start draw_texture4");
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        LOGI("start draw_texture5");
    }
}

void draw_atlas(GLEngine *engine){
    struct AtlasTable{
        int x;
        int y;
        int width;
        int height;
    } imagetable[] ={
        { 513, 1, 458, 402 },
        { 301, 513, 128, 128 },
        { 1003, 61, 16, 16 },
        { 513, 403, 256, 256 },
        { 1, 513, 300, 400 },
        { 971, 61, 32, 32 },
        { 1, 913, 64, 64 },
        { 1003, 77, 8, 8 },
        { 1, 1, 512, 512 },
        { 971, 1, 50, 60 },
    };

    struct AtlasTable image = imagetable[8];

    const GLint pixel_x = 0;
    const GLint pixel_y = 0;
    const GLint pixel_width = image.width;
    const GLint pixel_height = image.height;

    GLfloat VERTEX_LEFT = ((GLfloat)pixel_x / (GLfloat)engine->width) * 2.0f - 1.0f;
    GLfloat VERTEX_RIGHT = ((GLfloat)(pixel_x + pixel_width) / (GLfloat)engine->width) * 2.0f - 1.0f;
    GLfloat VERTEX_TOP = (((GLfloat)pixel_y / (GLfloat)engine->height) * 2.0f - 1.0f) * -1.0f;
    GLfloat VERTEX_BOTTOM = (((GLfloat)(pixel_y + pixel_height) / (GLfloat)engine->height) * 2.0f - 1.0f) * -1.0f;

    // GLfloat UV_LEFT = (GLfloat)image.x / (GLfloat)engine->width;
    // GLfloat UV_TOP = (GLfloat)image.y / (GLfloat)engine->height;
    // GLfloat UV_RIGHT = (GLfloat)(image.x + image.width) / (GLfloat)engine->width;
    // GLfloat UV_BOTTOM = (GLfloat)(image.y + image.height) / (GLfloat)engine->height;
    GLfloat UV_LEFT = (GLfloat)image.x / (GLfloat)1024;
    GLfloat UV_TOP = (GLfloat)image.y / (GLfloat)1024;
    GLfloat UV_RIGHT = (GLfloat)(image.x + image.width) / (GLfloat)1024;
    GLfloat UV_BOTTOM = (GLfloat)(image.y + image.height) / (GLfloat)1024;

    const GLfloat position[] = {
        VERTEX_LEFT, VERTEX_TOP,
        VERTEX_LEFT, VERTEX_BOTTOM,
        VERTEX_RIGHT, VERTEX_TOP,
        VERTEX_RIGHT, VERTEX_BOTTOM,
    };

    const GLfloat uv[] = {
        UV_LEFT, UV_TOP,
        UV_LEFT, UV_BOTTOM,
        UV_RIGHT, UV_TOP,
        UV_RIGHT, UV_BOTTOM,
    };

    glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);
    glVertexAttribPointer(engine->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)uv);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


GLfloat rotate = 0;
void draw_matrix(GLEngine *engine){
    const GLfloat position[] = {
        -0.5f, 0.5f,
        -0.5f, -0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f,
    };

    const GLfloat uv[] = {
        0, 0,
        0, 1,
        1, 0,
        1, 1,
    };

    glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);
    glVertexAttribPointer(engine->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)uv);

    {
        mat4 matrix = mat4_identity();

        matrix.m[3][0] = 0.1f;
        matrix.m[3][1] = 0.5f;

        glUniformMatrix4fv(engine->unif_matrix, 1, GL_FALSE, (GLfloat*)matrix.m);
    }
    
    {
        mat4 matrix = mat4_identity();

        matrix.m[0][0] = 0.5f;
        matrix.m[1][1] = 2.0f;
        matrix.m[2][2] = 0.0f;

        glUniformMatrix4fv(engine->unif_matrix, 1, GL_FALSE, (GLfloat*)matrix.m);

    }

    {
        mat4 matrix = mat4_identity();

        vec3 axis = vec3_createNormalized(0.0f, 0.0f, 1.0f);

        const GLfloat axis_x = axis.x;
        const GLfloat axis_y = axis.y;
        const GLfloat axis_z = axis.z;

        const GLfloat c = cos(degree2radian(rotate));
        const GLfloat s = sin(degree2radian(rotate));
        {
            matrix.m[0][0] = (axis_x * axis_x) * (1.0f - c) + c;
            matrix.m[0][1] = (axis_x * axis_y) * (1.0f - c) - axis_z * s;
            matrix.m[0][2] = (axis_x * axis_z) * (1.0f - c) + axis_y * s;
            matrix.m[0][3] = 0;
        }
        {
            matrix.m[1][0] = (axis_y * axis_x) * (1.0f - c) + axis_z * s;
            matrix.m[1][1] = (axis_y * axis_y) * (1.0f - c) + c;
            matrix.m[1][2] = (axis_y * axis_z) * (1.0f - c) + axis_x * s;
            matrix.m[1][3] = 0;
        }
        {
            matrix.m[2][0] = (axis_z * axis_x) * (1.0f - c) - axis_y * s;
            matrix.m[2][1] = (axis_z * axis_y) * (1.0f - c) + axis_x * s;
            matrix.m[2][2] = (axis_z * axis_z) * (1.0f - c) + c;
            matrix.m[2][3] = 0;
        }
        {
            matrix.m[3][0] = 0;
            matrix.m[3][1] = 0;
            matrix.m[3][2] = 0;
            matrix.m[3][3] = 1;
        }

        glUniformMatrix4fv(engine->unif_matrix, 1, GL_FALSE, (GLfloat*)matrix.m);
        rotate += 0.5;
    }

    {
        const mat4 translate = mat4_translate(0.5f, 0.5f, 0.0f);
        const mat4 scale = mat4_scale(0.5f, 2.0f, 1.0f);
        const mat4 rotate = mat4_rotate(vec3_create(0.0f, 0.0f, 1.0f), engine->rotate);

        mat4 matrix = mat4_multiply(translate, rotate);
        matrix = mat4_multiply(matrix, scale);

        // glUniformMatrix4fv(engine->unif_translate, 1, GL_FALSE, (GLfloat*)translate.m);
        // glUniformMatrix4fv(engine->unif_scale, 1, GL_FALSE, (GLfloat*)scale.m);
        // glUniformMatrix4fv(engine->unif_rotate, 1, GL_FALSE, (GLfloat*)rotate.m);
        glUniformMatrix4fv(engine->unif_matrix, 1, GL_FALSE, (GLfloat*)matrix.m);

        engine->rotate += 0.5f;
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void draw_spritematrix(GLEngine *engine){

    const GLfloat position[] = {
        -0.5f, 0.5f,
        -0.5f, -0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f,
    };

    const GLfloat uv[] = {
        0, 0,
        0, 1,
        1, 0,
        1, 1,
    };

    glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);
    glVertexAttribPointer(engine->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)uv);

    {
        const GLint spriteX = 256;
        const GLint spriteY = 128;
        const GLint spriteWidth = 512;
        const GLint spriteHeight = 512;

        const GLfloat surfaecAspect = (GLfloat)engine->width / (GLfloat)engine->height;
        const mat4 aspect = mat4_scale(1, surfaecAspect, 1);

        const GLfloat xScale = (GLfloat)spriteWidth / (GLfloat)engine->width * 2.0f;
        // const GLfloat yScale = (GLfloat)spriteHeight / (GLfloat)engine->height * 2.0f;
        const GLfloat yScale = (GLfloat)spriteHeight / (GLfloat)engine->width * 2.0f;

        const mat4 scale = mat4_scale(xScale, yScale, 0);

        const GLfloat vertexLeft = 0.5f + (1.0f - xScale) * 0.5f;
        // const GLfloat vertexTop = 0.5f + (1.0f - yScale) * 0.5f;
        const GLfloat vertexTop = 0.5f + (1.0f - (yScale * surfaecAspect)) * 0.5f;
        const GLfloat moveX = (GLfloat)spriteX / (GLfloat)engine->width * 2.0f;
        const GLfloat moveY = -((GLfloat)spriteY / (GLfloat)engine->height * 2.0f);

        const mat4 translate = mat4_translate(-vertexLeft + moveX, vertexTop + moveY, 0);

        const mat4 rotate = mat4_rotate(vec3_create(0, 0, 1), engine->rotate);

        mat4 matrix = mat4_multiply(translate, aspect);
        matrix = mat4_multiply(matrix, rotate);
        matrix = mat4_multiply(matrix, scale);
        
        matrix = Sprite_createPositionMatrix(engine->width, engine->height, spriteX, spriteY, spriteWidth, spriteHeight, engine->rotate);

        glUniformMatrix4fv(engine->unif_matrix, 1, GL_FALSE, (GLfloat*)matrix.m);

        engine->rotate += 0.5;

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

void draw_spriteuvmatrix(GLEngine *engine){
    
    const GLfloat position[] = {
        -0.5f, 0.5f,
        -0.5f, -0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f,
    };

    const GLfloat uv[] = {
        0, 0,
        0, 1,
        1, 0,
        1, 1,
    };

    glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);
    glVertexAttribPointer(engine->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)uv);

    {
        const GLint spriteX = 256;
        const GLint spriteY = 128;
        const GLint spriteWidth = 512;
        const GLint spriteHeight = 512;

        mat4 matrix = Sprite_createPositionMatrix(engine->width, engine->height, spriteX, spriteY, spriteWidth, spriteHeight, engine->rotate);

        glUniformMatrix4fv(engine->unif_matrix, 1, GL_FALSE, (GLfloat*)matrix.m);

        engine->rotate += 0.5f;
    }

    {
        struct AtlasTable{
            int x;
            int y;
            int width;
            int height;
        } imagetable[] ={
            { 513, 1, 458, 402 },
            { 301, 513, 128, 128 },
            { 1003, 61, 16, 16 },
            { 513, 403, 256, 256 },
            { 1, 513, 300, 400 },
            { 971, 61, 32, 32 },
            { 1, 913, 64, 64 },
            { 1003, 77, 8, 8 },
            { 1, 1, 512, 512 },
            { 971, 1, 50, 60 },
        };

        struct AtlasTable image = imagetable[3];

        const GLfloat xScale = (GLfloat)image.width / (GLfloat)1024;
        const GLfloat yScale = (GLfloat)image.height / (GLfloat)1024;

        const mat4 scale = mat4_scale(xScale, yScale, 0);

        const GLfloat xMove = (GLfloat)image.x / (GLfloat)1024;
        const GLfloat yMove = (GLfloat)image.y / (GLfloat)1024;
        const mat4 translate = mat4_translate(xMove, yMove, 0);

        // temp
        // scale = mat4_scale(1, 1, 0);
        // translate = mat4_translate(0, 0, 0);

        mat4 matrix = mat4_multiply(translate, scale);

        // matrix = Sprite_createUvMatrix(1024, 1024, image.x, image.y, image.width, image.height);
        matrix = Sprite_createUvMatrix(512, 512, 0, 0, 512, 512);

        glUniformMatrix4fv(engine->unif_uvmatrix, 1, GL_FALSE, (GLfloat*)matrix.m);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
    }
}