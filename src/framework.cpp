#include "framework.h"

// Globals.
bool should_quit = false;
int back_buffer_width;
int back_buffer_height;

// Internal.
static GLuint vbo;
static GLuint last_bound_texture_id;
static GLuint shader_program;
static GLuint projection_loc;

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

void init_framework(void) {
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
}

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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

void rendering_2d(int w, int h) {
    float proj[16] = {
        2.0f/w,  0,       0,   0,
        0,       2.0f/h,  0,   0,
        0,       0,       1,   0,
       -1,      -1,       0,   1
    };
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, proj);
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

Vector2 rotate_z(Vector2 v, Vector2 c, float theta) {
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
