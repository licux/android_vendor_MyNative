/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include <initializer_list>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <jni.h>
#include <cerrno>
#include <cassert>
#include <vector>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "GLEngine.h"
#include "Resource.h"
#include "PngImage.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))

extern void draw_triangle(GLEngine *engine);
extern void draw_rectangle(GLEngine *engine);
extern void draw_point(GLEngine *engine);
extern void draw_qued(GLEngine *engine);
extern void draw_move(GLEngine *engine);
extern void draw_texture(GLEngine *engine);

const GLchar *vertex_shader_source = 
    "attribute mediump vec4 attr_pos;"
    "attribute mediump vec2 attr_uv;"
    // "attribute lowp vec4 attr_color;"
    // "varying lowp vec4 vary_color;"
    // "uniform mediump vec2 unif_pos;"
    "varying mediump vec2 vary_uv;"
    "void main() {"
    "   gl_Position = attr_pos;"
    // "   gl_Position.xy += unif_pos;"
    // "   vary_color = attr_color;"
    "   vary_uv = attr_uv;"
    "}";

const GLchar *fragment_shader_source =
    // "uniform lowp vec4 unif_color;"
    // "varying lowp vec4 vary_color;"
    "uniform sampler2D texture;"
    "varying mediump vec2 vary_uv;"
    "void main() {"
    "   gl_FragColor = texture2D(texture, vary_uv);"
    "}";

static GLuint load_shader(GLenum type, const char** source){
    GLuint shader = glCreateShader(type);
    assert(glGetError() == GL_NO_ERROR);
    assert(shader != 0);

    glShaderSource(shader, 1, source, NULL);
    glCompileShader(shader);

    {
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if(compiled == GL_FALSE){
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if(infoLen > 1){
                std::vector<char> buf(infoLen);
                if(buf.capacity() > 0){
                    glGetShaderInfoLog(shader, infoLen, NULL, buf.data());
                    LOGE("Could not compile shader %d: %s\n", type, buf.data());
                }
            }
        }
    }

    return shader;
}

static GLuint create_program(GLEngine *engine, const char** vertex_source, const char** fragmane_source){
    engine->vertex_shader = load_shader(GL_VERTEX_SHADER, vertex_source);
    engine->fragment_shader = load_shader(GL_FRAGMENT_SHADER, fragmane_source);
    
    GLenum program = glCreateProgram();
    assert(program != 0);

    glAttachShader(program, engine->vertex_shader);
    assert(glGetError() == GL_NO_ERROR);
    glAttachShader(program, engine->fragment_shader);
    assert(glGetError() == GL_NO_ERROR);

    glLinkProgram(program);
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE){
        GLint bufLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
        if(bufLength > 1){
            std::vector<char> buf(bufLength);
            if(buf.capacity() > 0){
                glGetProgramInfoLog(program, bufLength, NULL, buf.data());
                LOGE("Could not link program: %s\n", buf.data());
            }
        }
    }

    engine->attr_pos = glGetAttribLocation(program, "attr_pos");
    assert(engine->attr_pos >= 0);
    engine->attr_uv = glGetAttribLocation(program, "attr_uv");
    assert(engine->attr_uv >= 0);
    // engine->attr_color = glGetAttribLocation(program, "attr_color");
    // assert(engine->attr_color >= 0);
    // engine->unif_color = glGetUniformLocation(program, "unif_color");
    // assert(engine->unif_color >= 0);
    // engine->unif_pos = glGetUniformLocation(program, "unif_pos");
    // assert(engine->unif_posr >= 0);

    {
        engine->unif_texture = glGetUniformLocation(program, "texture");
        assert(engine->unif_texture >= 0);
    }

    {
        glGenTextures(1, &engine->texture_id);
        assert(engine->texture_id != 0);
        assert(glGetError() == GL_NO_ERROR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        assert(glGetError() == GL_NO_ERROR);

        glBindTexture(GL_TEXTURE_2D, engine->texture_id);
        assert(glGetError() == GL_NO_ERROR);

        // Read png image
        {
            PngImage image(engine->app->activity->assetManager, "texture_rgb_asymmetry.png");
            assert(image);

            glTexImage2D(GL_TEXTURE_2D, 0, image.hasAlpha() ? GL_RGBA : GL_RGB, image.getWidth(), image.getHeight(), 0,
                image.hasAlpha() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image.getData());
            assert(glGetError() == GL_NO_ERROR);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        assert(glGetError() == GL_NO_ERROR);
    }


    GLfloat minmaxWidth[2] = { 0 };
    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, minmaxWidth);
    const GLfloat minLineWidth = minmaxWidth[0];
    const GLfloat maxLineWidth = minmaxWidth[1];
    LOGI("Device Spec[%f <= width <= %f]", minLineWidth, maxLineWidth);
    GLfloat minmaxPointSize[2] = { 0 };
    glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, minmaxPointSize);
    const GLfloat minPointSize = minmaxPointSize[0];
    const GLfloat maxPointSize = minmaxPointSize[1];
    LOGI("Device Spec[%f <= point size <= %f]", minPointSize, maxPointSize);
    GLint vert_vectors = 0;
    GLint frag_vectors = 0;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &vert_vectors);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &frag_vectors);
    LOGI("MAX Uniform Vectors / Vert(%d) Frag(%d)", vert_vectors, frag_vectors);
    GLint vary_vectors = 0;
    glGetIntegerv(GL_MAX_VARYING_VECTORS, &vary_vectors);
    LOGI("MAX Varying Vectors / %d", vary_vectors);

    return program;
}

static void term_program(GLEngine* engine){
    glUseProgram(0);

    glDeleteTextures(1, &engine->texture_id);

    glDeleteProgram(engine->program);
    assert(glGetError() == GL_NO_ERROR);

    assert(glIsProgram(engine->program) == GL_FALSE);

    glDeleteShader(engine->vertex_shader);
    assert(glGetError() == GL_NO_ERROR);
    glDeleteShader(engine->fragment_shader);
    assert(glGetError() == GL_NO_ERROR);
}

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(GLEngine* engine) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };

    const EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
    EGLint w, h, format;
    EGLint numConfigs;
    EGLConfig config = nullptr;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, nullptr, nullptr);

    /* Here, the application chooses the configuration it desires.
     * find the best match if possible, otherwise use the very first one
     */
    eglChooseConfig(display, attribs, nullptr,0, &numConfigs);
    std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
    assert(supportedConfigs);
    eglChooseConfig(display, attribs, supportedConfigs.get(), numConfigs, &numConfigs);
    assert(numConfigs);
    auto i = 0;
    for (; i < numConfigs; i++) {
        auto& cfg = supportedConfigs[i];
        EGLint r, g, b, d;
        if (eglGetConfigAttrib(display, cfg, EGL_RED_SIZE, &r)   &&
            eglGetConfigAttrib(display, cfg, EGL_GREEN_SIZE, &g) &&
            eglGetConfigAttrib(display, cfg, EGL_BLUE_SIZE, &b)  &&
            eglGetConfigAttrib(display, cfg, EGL_DEPTH_SIZE, &d) &&
            r == 8 && g == 8 && b == 8 && d == 0 ) {

            config = supportedConfigs[i];
            break;
        }
    }
    if (i == numConfigs) {
        config = supportedConfigs[0];
    }

    if (config == nullptr) {
        LOGW("Unable to initialize EGLConfig");
        return -1;
    }

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    surface = eglCreateWindowSurface(display, config, engine->app->window, nullptr);
    context = eglCreateContext(display, config,  EGL_NO_CONTEXT, contextAttribs);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->state.angle = 0;

    // Check openGL on the system
    auto opengl_info = {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS};
    for (auto name : opengl_info) {
        auto info = glGetString(name);
        LOGI("OpenGL Info: %s", info);
    }
    // // Initialize GL state.
    // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    // glEnable(GL_CULL_FACE);
    // glShadeModel(GL_SMOOTH);
    // glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, w, h);
    engine->program = create_program(engine, &vertex_shader_source, &fragment_shader_source);

    return 0;
}

/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(GLEngine* engine) {
    if (engine->display == nullptr) {
        // No display.
        return;
    }

    // Just fill the screen with a color.
    // glClearColor(((float)engine->state.x)/engine->width, engine->state.angle,
    // //              ((float)engine->state.y)/engine->height, 1);
    // glClearColor((GLfloat)(rand() % 256) / 255.0f, (GLfloat)(rand() % 256) / 255.0f, (GLfloat)(rand() % 256) / 255.0f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(engine->program);

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(engine->attr_pos);
    glEnableVertexAttribArray(engine->attr_uv);
    // glEnableVertexAttribArray(engine->attr_color);

    draw_texture(engine);
    

    eglSwapBuffers(engine->display, engine->surface);
}


/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(GLEngine* engine) {
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }


    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    auto* engine = (GLEngine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        engine->animating = 1;
        engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    auto* engine = (GLEngine*)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (engine->app->window != nullptr) {
                engine_init_display(engine);
                engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != nullptr) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                                               engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                                               engine->accelerometerSensor,
                                               (1000L/60)*1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != nullptr) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                                                engine->accelerometerSensor);
            }
            // Also stop animating.
            engine->animating = 0;
            engine_draw_frame(engine);
            break;
        case APP_CMD_START:
            LOGI("APP_CMD_START");
            break;
        case APP_CMD_RESUME:
            LOGI("APP_CMD_RESUME");
            break;
        case APP_CMD_PAUSE:
            LOGI("APP_CMD_PAUSE");
            break;
        case APP_CMD_STOP:
            LOGI("APP_CMD_STOP");
            break;
        default:
            break;
    }
}

/*
 * AcquireASensorManagerInstance(void)
 *    Workaround ASensorManager_getInstance() deprecation false alarm
 *    for Android-N and before, when compiling with NDK-r15
 */
#include <dlfcn.h>
ASensorManager* AcquireASensorManagerInstance(android_app* app) {

  if(!app)
    return nullptr;

  typedef ASensorManager *(*PF_GETINSTANCEFORPACKAGE)(const char *name);
  void* androidHandle = dlopen("libandroid.so", RTLD_NOW);
  auto getInstanceForPackageFunc = (PF_GETINSTANCEFORPACKAGE)
      dlsym(androidHandle, "ASensorManager_getInstanceForPackage");
  if (getInstanceForPackageFunc) {
    JNIEnv* env = nullptr;
    app->activity->vm->AttachCurrentThread(&env, nullptr);

    jclass android_content_Context = env->GetObjectClass(app->activity->clazz);
    jmethodID midGetPackageName = env->GetMethodID(android_content_Context,
                                                   "getPackageName",
                                                   "()Ljava/lang/String;");
    auto packageName= (jstring)env->CallObjectMethod(app->activity->clazz,
                                                        midGetPackageName);

    const char *nativePackageName = env->GetStringUTFChars(packageName, nullptr);
    ASensorManager* mgr = getInstanceForPackageFunc(nativePackageName);
    env->ReleaseStringUTFChars(packageName, nativePackageName);
    app->activity->vm->DetachCurrentThread();
    if (mgr) {
      dlclose(androidHandle);
      return mgr;
    }
  }

  typedef ASensorManager *(*PF_GETINSTANCE)();
  auto getInstanceFunc = (PF_GETINSTANCE)
      dlsym(androidHandle, "ASensorManager_getInstance");
  // by all means at this point, ASensorManager_getInstance should be available
  assert(getInstanceFunc);
  dlclose(androidHandle);

  return getInstanceFunc();
}


/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
    GLEngine engine{};

    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    // Prepare to monitor accelerometer
    engine.sensorManager = AcquireASensorManagerInstance(state);
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(
                                        engine.sensorManager,
                                        ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue = ASensorManager_createEventQueue(
                                    engine.sensorManager,
                                    state->looper, LOOPER_ID_USER,
                                    nullptr, nullptr);

    if (state->savedState != nullptr) {
        // We are starting with a previous saved state; restore from it.
        engine.state = *(struct saved_state*)state->savedState;
    }

    // loop waiting for stuff to do.

    while (true) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, nullptr, &events,
                                      (void**)&source)) >= 0) {

            // Process this event.
            if (source != nullptr) {
                source->process(state, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) {
                if (engine.accelerometerSensor != nullptr) {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                                                       &event, 1) > 0) {
                        LOGI("accelerometer: x=%f y=%f z=%f",
                             event.acceleration.x, event.acceleration.y,
                             event.acceleration.z);
                    }
                }
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engine_term_display(&engine);
                return;
            }
        }

        if (engine.animating) {
            // Done with events; draw next animation frame.
            engine.state.angle += .01f;
            if (engine.state.angle > 1) {
                engine.state.angle = 0;
            }

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine_draw_frame(&engine);
        }
    }
}
//END_INCLUDE(all)
