#include "general.h"

#define UNICODE
#define _UNICODE

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#include <math.h>

#include "stb_image.h"

#define GL_IMPLEMENTATION
#include "GL.h"


#include "framework.h"


extern "C" {
    // https://docs.nvidia.com/gameworks/content/technologies/desktop/optimus.htm
    // SHARED_EXPORT DWORD NvOptimusEnablement = 0x00000001;

    /* 
        This link https://gpuopen.com/learn/amdpowerxpressrequesthighperformance/
        defines the constant as a DWORD, but every code I saw defines it as an int.
    */
    // SHARED_EXPORT int AmdPowerXpressRequestHighPerformance = 1;
}

int main(void) {
    OS_Window *window = init_window("OpenGL", 800, 600);

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

    float64 last_counter = 0;

    while (!should_quit) {
        float64 wall_counter = get_current_time();
        float current_dt = (float)(wall_counter - last_counter);
        last_counter = wall_counter;

        update_window_events();

        glViewport(0, 0, back_buffer_width, back_buffer_height);
        
        rendering_2d(back_buffer_width, back_buffer_height);

        // glClearColor(0.2f, 0.72f, 0.38f, 1);
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

        swap_buffers(window);
    }

    free_window_and_opengl(window);

    return 0;
}






















#define GENERAL_IMPLEMENTATION
#include "general.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "framework.cpp"
