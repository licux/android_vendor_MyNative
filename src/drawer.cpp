#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "GLEngine.h"

#include <android/log.h>


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
