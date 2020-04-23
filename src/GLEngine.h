#ifndef __GLENGINE_H__
#define __GLENGINE_H__

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "Matrix.h"

/**
 * Our saved state data.
 */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

/**
 * Shared state for our app.
 */
class GLEngine {
public:
    struct android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;
    GLint unif_matrix;
    GLint unif_uvmatrix;
    GLint attr_pos;
    GLint attr_color;
    GLint attr_uv;
    GLint unif_texture;
    GLuint texture_id;
    GLint unif_uv;
    GLint unif_color;
    GLint unif_pos;
    GLint unif_lookat;
    GLint unif_projection;
    GLint unif_wlp;
    GLint unif_translate;
    GLint unif_rotate;
    GLint unif_scale;
    GLfloat rotate;
    vec3 cameraPos;
    int32_t width;
    int32_t height;
    struct saved_state state;
};

// /**
//  * Shared state for our app.
//  */
// struct engine {
//     struct android_app* app;

//     ASensorManager* sensorManager;
//     const ASensor* accelerometerSensor;
//     ASensorEventQueue* sensorEventQueue;

//     int animating;
//     EGLDisplay display;
//     EGLSurface surface;
//     EGLContext context;
//     GLuint vertex_shader;
//     GLuint fragment_shader;
//     GLuint program;
//     GLint attr_pos;
//     int32_t width;
//     int32_t height;
//     struct saved_state state;
// };

#endif // __GLNENGINE_H__