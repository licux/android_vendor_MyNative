#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "GLEngine.h"
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))

void draw3D_depthorder(GLEngine *engine){

    // Draw Rectangle
    {
        {
            glUniform4f(engine->unif_color, 1.0f, 0.0f, 1.0f, 0.5f);
        }

        const GLfloat positionQuad[] = {
            -0.25f, 0.25f, 0.5f,
            -0.25f, -0.25f, 0.5f,
            0.25f, 0.25f, 0.5f,
            0.25f, -0.25f, 0.5f,
        };

        glVertexAttribPointer(engine->attr_pos, 3, GL_FLOAT, GL_FALSE, 0, positionQuad);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    
    // Draw triangle
    {
        {
            glUniform4f(engine->unif_color, 1.0f, 1.0f, 1.0f, 0.5f);
        }
        const GLfloat positionTriangle[] = {
            0.0f, 0.5f, -0.5f,
            -0.5f, 0.0f, -0.5f,
            0.5f, 0.0f, -0.5f,
        };

        glVertexAttribPointer(engine->attr_pos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)positionTriangle);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    }

}

void draw3D_camera(GLEngine* engine){
    const vec3 camera_pos = engine->cameraPos;
    const vec3 camera_look = vec3_create(0, 0, 0);
    const vec3 camera_up = vec3_create(0, 1, 0);
    const mat4 lookAt = mat4_lookAt(camera_pos, camera_look, camera_up);

    const GLfloat prj_near = 1.0f;
    const GLfloat prj_far = 30.0f;
    const GLfloat prj_fovY = 45.0f;
    const GLfloat prj_aspect = (GLfloat)(engine->width) / (GLfloat)(engine->height);
    const mat4 projection = mat4_perspective(prj_near, prj_far, prj_fovY, prj_aspect);

    glUniformMatrix4fv(engine->unif_lookat, 1, GL_FALSE, (GLfloat*)lookAt.m);
    glUniformMatrix4fv(engine->unif_projection, 1, GL_FALSE, (GLfloat*)projection.m);

    engine->cameraPos.x -= 0.01f;
    engine->cameraPos.z += 0.02f;

    // Draw triangle
    {
        {
            glUniform4f(engine->unif_color, 1.0f, 0.0f, 0.0f, 1.0f);
        }
        const GLfloat positionTriangle[] = {
            0.0f, 0.5f, -0.5f,
            -0.5f, 0.0f, -0.5f,
            0.5f, 0.0f, -0.5f,
        };

        glVertexAttribPointer(engine->attr_pos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)positionTriangle);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    }

    // Draw Rectangle
    {
        {
            glUniform4f(engine->unif_color, 1.0f, 0.0f, 1.0f, 1.0f);
        }

        const GLfloat positionQuad[] = {
            -0.25f, 0.25f, 0.5f,
            -0.25f, -0.25f, 0.5f,
            0.25f, 0.25f, 0.5f,
            0.25f, -0.25f, 0.5f,
        };

        glVertexAttribPointer(engine->attr_pos, 3, GL_FLOAT, GL_FALSE, 0, positionQuad);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

void draw3D_cube(GLEngine *engine){
    const vec3 camera_pos = vec3_create(3, 3, -5);
    const vec3 camera_look = vec3_create(0, 0, 0);
    const vec3 camera_up = vec3_create(0, 1, 0);

    const GLfloat prj_near = 1.0f;
    const GLfloat prj_far = 100.0f;
    const GLfloat prj_fovY = 45.0f;
    const GLfloat prj_aspect = (GLfloat)engine->width / (GLfloat)engine->height;

    const mat4 lookAt = mat4_lookAt(camera_pos, camera_look, camera_up);
    const mat4 projection = mat4_perspective(prj_near, prj_far, prj_fovY, prj_aspect);
    const mat4 world = mat4_rotate(vec3_createNormalized(1, 1, 0), engine->rotate);

    mat4 wlp = mat4_multiply(projection, lookAt);
    wlp = mat4_multiply(wlp, world);
    glUniformMatrix4fv(engine->unif_wlp, 1, GL_FALSE, (GLfloat*)wlp.m);

    engine->rotate += 1;

    {
        const GLfloat LEFT = -1.0f;
        const GLfloat RIGHT = 1.0f;
        const GLfloat FRONT = -1.0f;
        const GLfloat BACK = 1.0f;
        const GLfloat TOP = 1.0f;
        const GLfloat BOTTOM = -1.0f;

        const vec3 positionCube[] = {
            {LEFT, BOTTOM, FRONT}, {RIGHT, BOTTOM, FRONT}, {LEFT, BOTTOM, BACK},
            {LEFT, BOTTOM, BACK}, {RIGHT, BOTTOM, FRONT}, {RIGHT, BOTTOM, BACK},

            {LEFT, TOP, FRONT}, {LEFT, TOP, BACK}, {RIGHT, TOP, FRONT},
            {LEFT, TOP, BACK}, {RIGHT, TOP, BACK}, {RIGHT, TOP, FRONT},

            {RIGHT, TOP, FRONT}, {RIGHT, TOP, BACK}, {RIGHT, BOTTOM, FRONT},
            {RIGHT, TOP, BACK}, {RIGHT, BOTTOM, BACK}, {RIGHT, BOTTOM, FRONT},

            {LEFT, TOP, FRONT}, {LEFT, BOTTOM, FRONT}, {LEFT, TOP, BACK},
            {LEFT, TOP, BACK}, {LEFT, BOTTOM, FRONT}, {LEFT, BOTTOM, BACK},

            {LEFT, TOP, BACK}, {LEFT, BOTTOM, BACK}, {RIGHT, TOP, BACK},
            {RIGHT, TOP, BACK}, {LEFT, BOTTOM, BACK}, {RIGHT, BOTTOM, BACK},

            {LEFT, TOP, FRONT}, {RIGHT, TOP, FRONT}, {LEFT, BOTTOM, FRONT},
            {RIGHT, TOP, FRONT}, {RIGHT, BOTTOM, FRONT}, {LEFT, BOTTOM, FRONT},
        };

        const vec2 uvCube[] = {
            {0, 0}, {1, 0}, {0, 1}, {1,0}, {0, 1}, {1, 1},
            {0, 0}, {1, 0}, {0, 1}, {1,0}, {0, 1}, {1, 1},


            {0, 0}, {1, 0}, {0, 1}, {1,0}, {0, 1}, {1, 1},
            {0, 0}, {1, 0}, {0, 1}, {1,0}, {0, 1}, {1, 1},

            {0, 0}, {1, 0}, {0, 1}, {1,0}, {0, 1}, {1, 1},
            {0, 0}, {1, 0}, {0, 1}, {1,0}, {0, 1}, {1, 1},
        };
        

        glBindTexture(GL_TEXTURE_2D, engine->texture_id);
        glUniform1i(engine->unif_texture, 0);

        glVertexAttribPointer(engine->attr_pos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)positionCube);
        glVertexAttribPointer(engine->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)uvCube);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


void draw3D_cubeindex(GLEngine *engine){
    const vec3 camera_pos = vec3_create(3, 3, -5);
    const vec3 camera_look = vec3_create(0, 0, 0);
    const vec3 camera_up = vec3_create(0, 1, 0);

    const GLfloat prj_near = 1.0f;
    const GLfloat prj_far = 100.0f;
    const GLfloat prj_fovY = 45.0f;
    const GLfloat prj_aspect = (GLfloat)engine->width / (GLfloat)engine->height;

    const mat4 lookAt = mat4_lookAt(camera_pos, camera_look, camera_up);
    const mat4 projection = mat4_perspective(prj_near, prj_far, prj_fovY, prj_aspect);
    const mat4 world = mat4_rotate(vec3_createNormalized(1, 1, 0), engine->rotate);

    mat4 wlp = mat4_multiply(projection, lookAt);
    wlp = mat4_multiply(wlp, world);
    glUniformMatrix4fv(engine->unif_wlp, 1, GL_FALSE, (GLfloat*)wlp.m);

    engine->rotate += 1;

    {
        const GLfloat LEFT = -1.0f;
        const GLfloat RIGHT = 1.0f;
        const GLfloat FRONT = -1.0f;
        const GLfloat BACK = 1.0f;
        const GLfloat TOP = 1.0f;
        const GLfloat BOTTOM = -1.0f;

        typedef struct CubeVertex{
            vec3 posotion;
            vec2 uv;
        }CubeVertex;

        const CubeVertex cubeVertices[] = {
            { {LEFT, TOP, FRONT}, {0, 1} },
            { {LEFT, TOP, BACK}, {0, 0} },
            { {RIGHT, TOP, FRONT}, {1, 1} },
            { {RIGHT, TOP, BACK}, {1, 0} },
            { {LEFT, BOTTOM, FRONT}, {1, 1} },
            { {LEFT, BOTTOM, BACK}, {1, 0} },
            { {RIGHT, BOTTOM, FRONT}, {0, 1} },
            { {RIGHT, BOTTOM, BACK}, {0, 0} },
        };

        const GLushort cubeIndices[] = {
            0, 1, 2,
            2, 1, 3,

            2, 3, 6,
            6, 3, 7,

            6, 7, 4,
            4, 7, 5,

            4, 5, 0,
            0, 5, 1,

            1, 5, 3,
            3, 5, 7,

            0, 2, 4,
            4, 2, 6,
        };
       

        glBindTexture(GL_TEXTURE_2D, engine->texture_id);
        glUniform1i(engine->unif_texture, 0);

        glVertexAttribPointer(engine->attr_pos, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (GLvoid*)cubeVertices);
        glVertexAttribPointer(engine->attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (GLvoid*)((GLubyte*)cubeVertices + sizeof(vec3)));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, cubeIndices);
    }
}

void draw3D_cubeindex_degenerate(GLEngine *engine){
    const vec3 camera_pos = vec3_create(3, 3, -5);
    const vec3 camera_look = vec3_create(0, 0, 0);
    const vec3 camera_up = vec3_create(0, 1, 0);

    const GLfloat prj_near = 1.0f;
    const GLfloat prj_far = 100.0f;
    const GLfloat prj_fovY = 45.0f;
    const GLfloat prj_aspect = (GLfloat)engine->width / (GLfloat)engine->height;

    const mat4 lookAt = mat4_lookAt(camera_pos, camera_look, camera_up);
    const mat4 projection = mat4_perspective(prj_near, prj_far, prj_fovY, prj_aspect);
    const mat4 world = mat4_rotate(vec3_createNormalized(1, 1, 0), engine->rotate);

    mat4 wlp = mat4_multiply(projection, lookAt);
    wlp = mat4_multiply(wlp, world);
    glUniformMatrix4fv(engine->unif_wlp, 1, GL_FALSE, (GLfloat*)wlp.m);

    engine->rotate += 1;

    {
        const GLfloat LEFT = -1.0f;
        const GLfloat RIGHT = 1.0f;
        const GLfloat FRONT = -1.0f;
        const GLfloat BACK = 1.0f;
        const GLfloat TOP = 1.0f;
        const GLfloat BOTTOM = -1.0f;

        typedef struct CubeVertex{
            vec3 posotion;
            vec2 uv;
        }CubeVertex;

        const CubeVertex cubeVertices[] = {
            { {LEFT, TOP, FRONT}, {0, 1} },
            { {LEFT, TOP, BACK}, {0, 0} },
            { {RIGHT, TOP, FRONT}, {1, 1} },
            { {RIGHT, TOP, BACK}, {1, 0} },
            { {LEFT, BOTTOM, FRONT}, {1, 1} },
            { {LEFT, BOTTOM, BACK}, {1, 0} },
            { {RIGHT, BOTTOM, FRONT}, {0, 1} },
            { {RIGHT, BOTTOM, BACK}, {0, 0} },
        };

        const GLushort cubeIndices[] = {
            0, 1, 2, 3,
            3, 2,
            2, 3, 6, 7,
            7, 6,
            6, 7, 4, 5,
            5, 4,
            4, 5, 0, 1,
            1, 1,
            1, 5, 3, 7,
            7, 0,
            0, 2, 4, 6,
        };
       

        glBindTexture(GL_TEXTURE_2D, engine->texture_id);
        glUniform1i(engine->unif_texture, 0);

        glVertexAttribPointer(engine->attr_pos, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (GLvoid*)cubeVertices);
        glVertexAttribPointer(engine->attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (GLvoid*)((GLubyte*)cubeVertices + sizeof(vec3)));
        glDrawElements(GL_TRIANGLE_STRIP, 4 * 6 + 2 * 5, GL_UNSIGNED_SHORT, cubeIndices);
    }
}

void draw3D_rectindex_atlas(GLEngine *engine){

    typedef struct RectVertex{
        vec2 pos;
        vec2 uv;
    }RectVertex;

    const RectVertex vertices[] = {
        // rect 0
        { {-0.75f, 0.75f}, {0.0f, 0.0f,} },
        { {-0.75f, 0.25f}, {0.0f, 1.0f,} },
        { {-0.25f, 0.75f}, {0.5f, 0.0f,} },
        { {-0.25f, 0.25f}, {0.5f, 1.0f,} },
        // rect 1
        { {0.25f, -0.25f}, {0.5f, 0.0f,} },
        { {0.25f, -0.75f}, {0.5f, 1.0f,} },
        { {0.75f, -0.25f}, {1.0f, 0.0f,} },
        { {0.75f, -0.75f}, {1.0f, 1.0f,} },
    };

    const GLubyte indices[] = {
        0, 1, 2, 3,
        3, 4,
        4, 5, 6 ,7
    };

    glBindTexture(GL_TEXTURE_2D, engine->texture_id);
    glUniform1i(engine->unif_texture, 0);

    glVertexAttribPointer(engine->attr_pos, 2, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (GLvoid*)vertices);
    glVertexAttribPointer(engine->attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (GLvoid*)((GLubyte*)vertices + sizeof(vec2)));

    glDrawElements(GL_TRIANGLE_STRIP, 4 + 2 + 4, GL_UNSIGNED_BYTE, indices);
}