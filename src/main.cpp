#include "general.h"
#include "framework.h"

#include "stb_image.h"
#include "stb_truetype.h"

#include <math.h>


extern "C" {
    // https://docs.nvidia.com/gameworks/content/technologies/desktop/optimus.htm
    // SHARED_EXPORT DWORD NvOptimusEnablement = 0x00000001;

    /* 
        This link https://gpuopen.com/learn/amdpowerxpressrequesthighperformance/
        defines the constant as a DWORD, but every code I saw defines it as an int.
    */
    // SHARED_EXPORT int AmdPowerXpressRequestHighPerformance = 1;
}

unsigned char ttf_buffer[1<<20];
unsigned char temp_bitmap[512*512];

stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
GLuint ftex;

void my_stbtt_initfont(void)
{
    fread(ttf_buffer, 1, 1<<20, fopen("c:/windows/fonts/times.ttf", "rb"));
    stbtt_BakeFontBitmap(ttf_buffer,0, 32.0, temp_bitmap,512,512, 32,96, cdata); // no guarantee this fits!
    // can free ttf_buffer at this point
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &ftex);
    glBindTexture(GL_TEXTURE_2D, ftex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 512,512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
    // can free temp_bitmap at this point
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
}

void my_stbtt_print(float x, float y, char *text)
{
    frame_flush();

    // assume orthographic projection with units = screen pixels, origin at top left
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLint texture_handle = glGetUniformLocation(get_shader(), "texture_map");
    glUniform1i(texture_handle, 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ftex);

    while (*text) {
        if (*text >= 32 && *text < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(cdata, 512,512, *text-32, &x,&y,&q,1);//1=opengl & d3d10+,0=d3d9
            draw_quad(q.x0, q.y0, q.x1, q.y1,
                      q.s0, q.t0, q.s1, q.t1,
                      Vector4{1,1,1,1});
        }
        ++text;
    }
}


int main(void) {
    OS_Window *window = init_window("OpenGL", 800, 600);

    my_stbtt_initfont();

    Texture test = texture_load_from_file("data/textures/Texturtest planar.png");
    Texture cat  = texture_load_from_file("data/textures/cat.png");

    u8 pixel_data[4] = {0xFF,0xFF,0xFF,0xFF};
    Texture white = texture_load_from_memory(pixel_data, 1, 1, 4);
    UNUSED(white);

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
        if (current_dt > 0.16f) current_dt = 0.16f;
        last_counter = wall_counter;

        update_window_events();

        if (key_esc.is_down) {
            should_quit = true;
        }

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


        my_stbtt_print(100, 100, "Hello friend!");

        frame_flush();

        swap_buffers(window);
        os_sleep(1);
    }

    free_window_and_opengl(window);

    return 0;
}








#define GENERAL_IMPLEMENTATION
#include "general.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "framework.cpp"
