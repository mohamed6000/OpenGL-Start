#include "general.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <dlfcn.h>
#include <time.h>

#include <math.h>

#include "stb_image.h"

#define GL_IMPLEMENTATION
#include "GL.h"


GLuint vbo;
GLuint last_bound_texture_id;
GLuint shader_program;
GLuint projection_loc;

struct Vector2 {
    float x, y;
};

struct Vector3 {
    float x, y, z;
};

struct Vector4 {
    float x, y, z, w;
};

struct Texture {
    GLuint id;
    int width;
    int height;
    int channels;
};

struct Vertex {
    Vector3 position;
    Vector4 color;
    Vector2 uv;
};

const int MAX_VERTICES = 1024;
static Vertex vertices[MAX_VERTICES];
int vertex_count = 0;

const char *vertex_shader_source = R"(
#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec4 in_color;
layout (location = 2) in vec2 in_uv;

out vec4 color;
out vec2 uv;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(in_position.x, in_position.y, in_position.z, 1.0f);
    color       = in_color;
    uv          = in_uv;
}
)";

const char *fragment_shader_source = R"(
#version 330 core
out vec4 final_color;

in vec4 color;
in vec2 uv;

uniform sampler2D texture_map;

void main() {
    final_color = texture(texture_map, uv) * color;
}
)";

void frame_flush(void) {
    if (!vertex_count) return;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size_of(vertices[0]) * vertex_count, vertices, GL_STREAM_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, vertex_count);

    vertex_count = 0;
}

void render_update_texture(Texture *texture, unsigned char *data) {
    if (!texture->id) {
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &texture->id);
    }

    if (!texture->id) return;

    // https://learn.microsoft.com/en-us/windows/win32/opengl/glteximage2d
    GLenum gl_source_format = GL_RGBA8; // Texture format.
    GLenum gl_dest_format   = GL_RGBA;  // The format of the pixel data.
    if (texture->channels == 3) {
        gl_source_format = GL_RGB8;
        gl_dest_format   = GL_RGB;
    }

    // @Todo: Add more formats.

    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glTexImage2D(GL_TEXTURE_2D, 0, gl_source_format, 
                 texture->width, texture->height, 
                 0, gl_dest_format, GL_UNSIGNED_BYTE, 
                 data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture texture_load_from_file(const char *file_path) {
    Texture result = {};

    stbi_uc *data = stbi_load(file_path, &result.width, &result.height, &result.channels, 0);
    if (data) {
        render_update_texture(&result, data);

        stbi_image_free(data);
    }

    return result;
}

void set_texture(Texture *texture) {
    if (last_bound_texture_id != texture->id) {
        frame_flush();
        GLint texture_handle = glGetUniformLocation(shader_program, "texture_map");
        glUniform1i(texture_handle, 0);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->id);
    }

    last_bound_texture_id = texture->id;
}

void draw_quad(float x0, float y0, float x1, float y1, Vector4 c) {
    assert(vertex_count <= (MAX_VERTICES - 6));

    Vertex *v = vertices + vertex_count;
    v->position.x = x0;
    v->position.y = y0;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = 0;
    v->uv.y       = 1;
    v += 1;

    v->position.x = x1;
    v->position.y = y0;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = 1;
    v->uv.y       = 1;
    v += 1;

    v->position.x = x1;
    v->position.y = y1;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = 1;
    v->uv.y       = 0;
    v += 1;

    v->position.x = x0;
    v->position.y = y0;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = 0;
    v->uv.y       = 1;
    v += 1;

    v->position.x = x1;
    v->position.y = y1;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = 1;
    v->uv.y       = 0;
    v += 1;

    v->position.x = x0;
    v->position.y = y1;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = 0;
    v->uv.y       = 0;
    v += 1;

    vertex_count += 6;
}

void draw_quad(float x0, float y0, float x1, float y1, 
               float u0, float v0, float u1, float v1,
               Vector4 c) {
    assert(vertex_count <= (MAX_VERTICES - 6));

    Vertex *v = vertices + vertex_count;
    v->position.x = x0;
    v->position.y = y0;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = u0;
    v->uv.y       = v1;
    v += 1;

    v->position.x = x1;
    v->position.y = y0;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = u1;
    v->uv.y       = v1;
    v += 1;

    v->position.x = x1;
    v->position.y = y1;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = u1;
    v->uv.y       = v0;
    v += 1;

    v->position.x = x0;
    v->position.y = y0;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = u0;
    v->uv.y       = v1;
    v += 1;

    v->position.x = x1;
    v->position.y = y1;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = u1;
    v->uv.y       = v0;
    v += 1;

    v->position.x = x0;
    v->position.y = y1;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = u0;
    v->uv.y       = v0;
    v += 1;

    vertex_count += 6;
}

void draw_quad(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
               Vector4 c0, Vector4 c1, Vector4 c2, Vector4 c3) {
    assert(vertex_count <= (MAX_VERTICES - 6));

    Vertex *v = vertices + vertex_count;
    v->position = p0;
    v->color    = c0;
    v->uv.x     = 0;
    v->uv.y     = 1;
    v += 1;

    v->position = p1;
    v->color    = c1;
    v->uv.x     = 1;
    v->uv.y     = 1;
    v += 1;

    v->position = p2;
    v->color    = c2;
    v->uv.x     = 1;
    v->uv.y     = 0;
    v += 1;

    v->position = p0;
    v->color    = c0;
    v->uv.x     = 0;
    v->uv.y     = 1;
    v += 1;

    v->position = p2;
    v->color    = c2;
    v->uv.x     = 1;
    v->uv.y     = 0;
    v += 1;

    v->position = p3;
    v->color    = c3;
    v->uv.x     = 0;
    v->uv.y     = 0;
    v += 1;

    vertex_count += 6;
}

void draw_quad(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
               Vector2 uv0, Vector2 uv1, Vector2 uv2, Vector2 uv3,
               Vector4 color) {
    assert(vertex_count <= (MAX_VERTICES - 6));

    Vertex *v = vertices + vertex_count;
    v->position = p0;
    v->color    = color;
    v->uv.x     = uv0.x;
    v->uv.y     = uv3.y;
    v += 1;

    v->position = p1;
    v->color    = color;
    v->uv.x     = uv1.x;
    v->uv.y     = uv2.y;
    v += 1;

    v->position = p2;
    v->color    = color;
    v->uv.x     = uv2.x;
    v->uv.y     = uv1.y;
    v += 1;

    v->position = p0;
    v->color    = color;
    v->uv.x     = uv0.x;
    v->uv.y     = uv3.y;
    v += 1;

    v->position = p2;
    v->color    = color;
    v->uv.x     = uv2.x;
    v->uv.y     = uv1.y;
    v += 1;

    v->position = p3;
    v->color    = color;
    v->uv.x     = uv3.x;
    v->uv.y     = uv0.y;
    v += 1;

    vertex_count += 6;
}

inline Vector2 rotate_z(Vector2 v, Vector2 c, float theta) {
    Vector2 result;

    float ct = cosf(theta);
    float st = sinf(theta);

    v.x -= c.x;
    v.y -= c.y;

    result.x = v.x*ct - v.y*st;
    result.y = v.x*st + v.y*ct;

    result.x += c.x;
    result.y += c.y;

    return result;
}

void rendering_2d(int w, int h) {
    float proj[16] = {
        2.0f/w,  0,       0,   0,
        0,       2.0f/h,  0,   0,
        0,       0,       1,   0,
       -1,      -1,       0,   1
    };
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, proj);
}

bool should_quit = false;

GLXFBConfig glx_choose_best_fb_config(Display *display, int screen_id, int *visual_attributes) {
    // Getting matching framebuffer configs.
    int fb_count;
    GLXFBConfig *fb_configs = glXChooseFBConfig(display, screen_id, visual_attributes, &fb_count);
    if (!fb_configs) {
        write_string("Failed to glXChooseFBConfig.\n", true);
        return 0;
    }

    print("Found %d matching FB configs.\n", fb_count);

    int best_fbc_index  = -1;
    int worst_fbc_index = -1;
    int best_sample_count  = -1;
    int worst_sample_count = 999;

    for (int index = 0; index < fb_count; index++) {
        XVisualInfo *vi = glXGetVisualFromFBConfig(display, fb_configs[index]);
        if (vi) {
            int sample_buffer, sample_count;
            glXGetFBConfigAttrib(display, fb_configs[index], GLX_SAMPLE_BUFFERS, &sample_buffer);
            glXGetFBConfigAttrib(display, fb_configs[index], GLX_SAMPLES,        &sample_count);

            if ((best_fbc_index < 0) || sample_buffer && (sample_count > best_sample_count)) {
                best_fbc_index = index;
                best_sample_count = sample_count;
            }

            if ((worst_fbc_index < 0) || !sample_buffer && (sample_count < worst_sample_count)) {
                worst_fbc_index = index;
                worst_sample_count = sample_count;
            }
            XFree(vi);
        }
    }

    GLXFBConfig best_config = fb_configs[best_fbc_index];
    XFree(fb_configs);

    return best_config;
}

int main(void) {
    Display *display = XOpenDisplay(null);
    if (!display) {
        write_string("Failed to XOpenDisplay.\n", true);
        return 0;
    }

    if (!glx_load()) return 0;

    // Get a matching FB config.
    int visual_attributes[] = {
        GLX_X_RENDERABLE,   True,
        GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
        GLX_RENDER_TYPE,    GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
        GLX_RED_SIZE,       8,
        GLX_GREEN_SIZE,     8,
        GLX_BLUE_SIZE,      8,
        GLX_ALPHA_SIZE,     8,
        GLX_DEPTH_SIZE,     24,
        GLX_STENCIL_SIZE,   8,
        GLX_DOUBLEBUFFER,   True,
        // GLX_SAMPLE_BUFFERS, 1,
        // GLX_SAMPLES,        4,
        None,
    };

    // GLX version.
    GLint glx_major, glx_minor;
    glXQueryVersion(display, &glx_major, &glx_minor);
    if (((glx_major == 1) && (glx_minor < 3)) || (glx_major < 1)) {
        write_string("Invalid GLX version: You need at least GLX 1.3.\n", true);
        return 0;
    }

    print("GLX Version: %d.%d\n", glx_major, glx_minor);

    GL_LOAD_PROC(glXCreateContextAttribsARB);
    GL_LOAD_PROC(glXChooseFBConfig);
    GL_LOAD_PROC(glXGetFBConfigAttrib);
    GL_LOAD_PROC(glXGetVisualFromFBConfig);
    GL_LOAD_PROC(glXQueryExtensionsString);

    int screen_id   = XDefaultScreen(display);
    u64 black_color = BlackPixel(display, screen_id);
    u64 white_color = WhitePixel(display, screen_id);

    GLXFBConfig best_config = glx_choose_best_fb_config(display, screen_id, visual_attributes);

    // Get visual.
    XVisualInfo *visual_info = glXGetVisualFromFBConfig(display, best_config);

    Window parent_window = RootWindow(display, screen_id);

    Colormap colormap = XCreateColormap(display, parent_window, 
                                        visual_info->visual, 
                                        AllocNone);

    XSetWindowAttributes window_attributes = {};
    window_attributes.border_pixel      = black_color;
    window_attributes.background_pixel  = white_color;
    window_attributes.override_redirect = True;
    window_attributes.colormap   = colormap;
    window_attributes.event_mask = ExposureMask|KeyPressMask|StructureNotifyMask;

    Window window = XCreateWindow(display, parent_window, 
                                  10, 10,
                                  800, 600,
                                  0, 
                                  visual_info->depth, 
                                  InputOutput,
                                  visual_info->visual,
                                  CWBackPixel|CWColormap|CWBorderPixel|CWEventMask,
                                  &window_attributes);
    if (!window) {
        write_string("Failed to create window.\n", true);
        return 0;
    }

    XFree(visual_info);

    const char *title = "OpenGL (Linux)";
    Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
    Atom utf8_string = XInternAtom(display, "UTF8_STRING",  False);

    XChangeProperty(display, window, net_wm_name, utf8_string, 
                    8, PropModeReplace, 
                    (const u8 *)title, string_length(title));

    XStoreName(display, window, title);  // Legacy fallback.

    // Notify when the window manager attempts to delete the window.
    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    if (!XSetWMProtocols(display, window, &wm_delete_window, 1)) {
        write_string("Failed to XSetWMProtocols.\n", true);
    }

    const char *glx_extensions = glXQueryExtensionsString(display, screen_id);
    if (!gl_is_extension_supported(glx_extensions, "GLX_ARB_create_context")) {
        write_string("glXCreateContextAttribsARB() not found.\n", true);
        return 0;
    }

    int context_attributes[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        // GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
#if GL_USE_LEGACY_PROCS
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
#else
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
#endif
        None,
    };

    GLXContext gl_context = glXCreateContextAttribsARB(display, 
                                                       best_config,
                                                       null,
                                                       True,
                                                       context_attributes);
    if (!gl_context) {
        write_string("Failed to glXCreateContextAttribsARB.\n", true);
        return 0;
    }

    if (!glXMakeCurrent(display, window, gl_context)) {
        write_string("Failed to glXMakeCurrent.\n", true);
        return 0;
    }

    if (!gl_load()) return 0;

    // Load modern GL functions...
    gl_load_extensions();

    const GLubyte *gl_version  = glGetString(GL_VERSION);
    const GLubyte *gl_vendor   = glGetString(GL_VENDOR);
    const GLubyte *gl_renderer = glGetString(GL_RENDERER);
    const GLubyte *gl_shader_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    print("OpenGL Version:  %s\n", gl_version);
    print("OpenGL Vendor:   %s\n", gl_vendor);
    print("OpenGL Renderer: %s\n", gl_renderer);
    print("OpenGL Shading Language Version: %s\n", gl_shader_version);

    Texture test = texture_load_from_file("data/textures/Texturtest planar.png");
    Texture cat  = texture_load_from_file("data/textures/cat.png");

    XClearWindow(display, window);
    XMapRaised(display, window);

    XWindowAttributes size_attributes;
    XGetWindowAttributes(display, window, &size_attributes);

    int back_buffer_width  = size_attributes.width;
    int back_buffer_height = size_attributes.height;


    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, null);
    glCompileShader(vertex_shader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex_shader, size_of(infoLog), NULL, infoLog);
        print("Failed to compile vertex shader:\n%s\n", infoLog);
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, null);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment_shader, size_of(infoLog), NULL, infoLog);
        print("Failed to compile fragment shader:\n%s\n", infoLog);
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * size_of(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * size_of(float), (void *)12);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * size_of(float), (void *)28);
    glEnableVertexAttribArray(2);

    glUseProgram(shader_program);

    projection_loc = glGetUniformLocation(shader_program, "projection");


    // glDisable(GL_DEPTH_TEST);
    // Depth is mapped as near=-1 and far 1.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Vector2 cat_pos0 = {400, 300};
    Vector2 cat_pos1 = {500, 400};
    float cat_rot_angle = 0;

    Vector3 red_cat_p0 = {500, 300, 0};
    Vector3 red_cat_p1 = {650, 300, 0};
    Vector3 red_cat_p2 = {650, 400, 0};
    Vector3 red_cat_p3 = {500, 400, 0};

    float64 ONE_OVER_NANO_SECOND = 0.000000001;
    timespec tspec;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tspec);
    float64 last_counter = 0;

    while (!should_quit) {
        int clock_result = clock_gettime(CLOCK_MONOTONIC_RAW, &tspec);
        float64 current_counter = (float64)tspec.tv_sec + tspec.tv_nsec * ONE_OVER_NANO_SECOND;
        float current_dt = current_counter - last_counter;
        last_counter = current_counter;

        // Non-blocking event loop.
        while (XPending(display) > 0) {
            XEvent event;
            XNextEvent(display, &event);

            switch (event.type) {
                case ClientMessage:
                    if ((Atom)event.xclient.data.l[0] == wm_delete_window) {
                        should_quit = true;
                    }
                    break;

                case DestroyNotify:
                    should_quit = true;
                    break;

                case Expose:
                    // Repaint.
                    break;

                case ConfigureNotify:
                    // Handle resize here.
                    back_buffer_width  = event.xconfigure.width;
                    back_buffer_height = event.xconfigure.height;
                    break;

                case KeyPress:
                    write_string("Key press event.\n");
                    break;
            }
        }

        glViewport(0, 0, back_buffer_width, back_buffer_height);

        rendering_2d(back_buffer_width, back_buffer_height);

        glClearColor(0.2f, 0.38f, 0.72f, 1);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        set_texture(&test);

        draw_quad(10, 10, 100, 100, Vector4{1,1,1,1});

        draw_quad(Vector3{100,100,0}, Vector3{250,100,0}, Vector3{250,250,0}, Vector3{100,250,0},
                  Vector4{1,0,0,1},   Vector4{0,1,0,1},   Vector4{0,0,1,1},   Vector4{0,1,0,1});

        set_texture(&cat);

        Vector3 center = {
            (red_cat_p0.x+red_cat_p1.x)/2,
            0,
            (red_cat_p0.z+red_cat_p3.z)/2,
        };

        draw_quad(red_cat_p0, 
                  red_cat_p1, 
                  red_cat_p2, 
                  red_cat_p3,
                  Vector2{0,0}, 
                  Vector2{1,0},
                  Vector2{1,0.5f},
                  Vector2{0,0.5f},
                  Vector4{1,0,0,1});

        float depth,x0,y0,x1,y1;

        set_texture(&test);
        depth = 0;
        x0 = 200;
        y0 = 200;
        x1 = 500;
        y1 = 500;
        draw_quad(Vector3{x0,y0,depth}, Vector3{x1,y0,depth}, Vector3{x1,y1,depth}, Vector3{x0,y1,depth},
                  Vector4{1,0,0,1},   Vector4{0,1,0,1},   Vector4{0,0,1,1},   Vector4{0,1,0,1});

        set_texture(&cat);
        depth = -0.2f;

        Vector2 center_2d = {
            (cat_pos0.x+cat_pos1.x)/2,
            (cat_pos0.y+cat_pos1.y)/2,
        };

        cat_rot_angle += current_dt * (float)TAU;
        if (cat_rot_angle >= 360.0f) cat_rot_angle = 0;

        Vector2 p0 = rotate_z(Vector2{cat_pos0.x,cat_pos0.y}, center_2d, cat_rot_angle);
        Vector2 p1 = rotate_z(Vector2{cat_pos1.x,cat_pos0.y}, center_2d, cat_rot_angle);
        Vector2 p2 = rotate_z(Vector2{cat_pos1.x,cat_pos1.y}, center_2d, cat_rot_angle);
        Vector2 p3 = rotate_z(Vector2{cat_pos0.x,cat_pos1.y}, center_2d, cat_rot_angle);

        draw_quad(Vector3{p0.x, p0.y ,depth},
                  Vector3{p1.x, p1.y ,depth}, 
                  Vector3{p2.x, p2.y ,depth}, 
                  Vector3{p3.x, p3.y ,depth}, 
                  Vector4{1,1,1,1},
                  Vector4{1,1,1,1},
                  Vector4{1,1,1,1},
                  Vector4{1,1,1,1});


        frame_flush();

        glXSwapBuffers(display, window);
    }

    glXDestroyContext(display, gl_context);

    XFreeColormap(display, colormap);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}





#define GENERAL_IMPLEMENTATION
#include "general.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"