#include "general.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <dlfcn.h>

#include "stb_image.h"


// #include <GL/gl.h>
#define GL_PROC(ident) CONCAT(ident,PROC)

#ifndef APIENTRY
#define APIENTRY
#endif


typedef unsigned int  GLenum;
typedef unsigned char GLboolean;
typedef unsigned int  GLbitfield;
typedef int           GLint;
typedef int           GLsizei;
typedef unsigned char GLubyte;
typedef unsigned int  GLuint;
typedef float         GLfloat;
typedef float         GLclampf;
typedef double        GLclampd;
typedef void          GLvoid;


#define GL_BLEND                   0x0BE2
#define GL_CULL_FACE               0x0B44
#define GL_DEPTH_TEST              0x0B71
#define GL_LINE_SMOOTH             0x0B20
#define GL_LINE_WIDTH              0x0B21
#define GL_LINE_WIDTH_RANGE        0x0B22
#define GL_LINE_WIDTH_GRANULARITY  0x0B23
#define GL_LINE_STIPPLE            0x0B24
#define GL_LINE_STIPPLE_PATTERN    0x0B25
#define GL_LINE_STIPPLE_REPEAT     0x0B26
#define GL_UNPACK_SWAP_BYTES       0x0CF0
#define GL_UNPACK_LSB_FIRST        0x0CF1
#define GL_UNPACK_ROW_LENGTH       0x0CF2
#define GL_UNPACK_SKIP_ROWS        0x0CF3
#define GL_UNPACK_SKIP_PIXELS      0x0CF4
#define GL_UNPACK_ALIGNMENT        0x0CF5
#define GL_PACK_SWAP_BYTES         0x0D00
#define GL_PACK_LSB_FIRST          0x0D01
#define GL_PACK_ROW_LENGTH         0x0D02
#define GL_PACK_SKIP_ROWS          0x0D03
#define GL_PACK_SKIP_PIXELS        0x0D04
#define GL_PACK_ALIGNMENT          0x0D05
#define GL_TEXTURE_1D              0x0DE0
#define GL_TEXTURE_2D              0x0DE1
#define GL_SCISSOR_BOX             0x0C10
#define GL_SCISSOR_TEST            0x0C11
#define GL_STENCIL_TEST            0x0B90
#define GL_STENCIL_CLEAR_VALUE     0x0B91
#define GL_STENCIL_FUNC            0x0B92
#define GL_STENCIL_VALUE_MASK      0x0B93
#define GL_STENCIL_FAIL            0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF             0x0B97
#define GL_STENCIL_WRITEMASK       0x0B98
#define GL_STENCIL_BITS            0x0D57


/* AlphaFunction */
#define GL_NEVER    0x0200
#define GL_LESS     0x0201
#define GL_EQUAL    0x0202
#define GL_LEQUAL   0x0203
#define GL_GREATER  0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL   0x0206
#define GL_ALWAYS   0x0207

/* BeginMode */
#define GL_POINTS         0x0000
#define GL_LINES          0x0001
#define GL_LINE_LOOP      0x0002
#define GL_LINE_STRIP     0x0003
#define GL_TRIANGLES      0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN   0x0006
#define GL_QUADS          0x0007
#define GL_QUAD_STRIP     0x0008
#define GL_POLYGON        0x0009

/* BlendingFactorDest */
#define GL_ZERO                0
#define GL_ONE                 1
#define GL_SRC_COLOR           0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA           0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA           0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305

/* BlendingFactorSrc */
#define GL_DST_COLOR           0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE  0x0308

/* DataType */
#define GL_BYTE           0x1400
#define GL_UNSIGNED_BYTE  0x1401
#define GL_SHORT          0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT            0x1404
#define GL_UNSIGNED_INT   0x1405
#define GL_FLOAT          0x1406
#define GL_2_BYTES        0x1407
#define GL_3_BYTES        0x1408
#define GL_4_BYTES        0x1409
#define GL_DOUBLE         0x140A

/* DrawBufferMode */
#define GL_NONE           0
#define GL_FRONT_LEFT     0x0400
#define GL_FRONT_RIGHT    0x0401
#define GL_BACK_LEFT      0x0402
#define GL_BACK_RIGHT     0x0403
#define GL_FRONT          0x0404
#define GL_BACK           0x0405
#define GL_LEFT           0x0406
#define GL_RIGHT          0x0407
#define GL_FRONT_AND_BACK 0x0408
#define GL_AUX0           0x0409
#define GL_AUX1           0x040A
#define GL_AUX2           0x040B
#define GL_AUX3           0x040C

/* FrontFaceDirection */
#define GL_CW  0x0900
#define GL_CCW 0x0901

/* LogicOp */
#define GL_CLEAR         0x1500
#define GL_AND           0x1501
#define GL_AND_REVERSE   0x1502
#define GL_COPY          0x1503
#define GL_AND_INVERTED  0x1504
#define GL_NOOP          0x1505
#define GL_XOR           0x1506
#define GL_OR            0x1507
#define GL_NOR           0x1508
#define GL_EQUIV         0x1509
#define GL_INVERT        0x150A
#define GL_OR_REVERSE    0x150B
#define GL_COPY_INVERTED 0x150C
#define GL_OR_INVERTED   0x150D
#define GL_NAND          0x150E
#define GL_SET           0x150F

/* MatrixMode */
#define GL_MODELVIEW  0x1700
#define GL_PROJECTION 0x1701

/* PixelFormat */
#define GL_COLOR_INDEX     0x1900
#define GL_STENCIL_INDEX   0x1901
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RED             0x1903
#define GL_GREEN           0x1904
#define GL_BLUE            0x1905
#define GL_ALPHA           0x1906
#define GL_RGB             0x1907
#define GL_RGBA            0x1908
#define GL_LUMINANCE       0x1909
#define GL_LUMINANCE_ALPHA 0x190A

/* polygon_offset */
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_POLYGON_OFFSET_UNITS  0x2A00
#define GL_POLYGON_OFFSET_POINT  0x2A01
#define GL_POLYGON_OFFSET_LINE   0x2A02
#define GL_POLYGON_OFFSET_FILL   0x8037

/* texture */
#define GL_RGB8  0x8051
#define GL_RGBA8 0x8058

/* TextureEnvMode */
#define GL_MODULATE 0x2100
#define GL_DECAL    0x2101

/* TextureEnvParameter */
#define GL_TEXTURE_ENV_MODE  0x2200
#define GL_TEXTURE_ENV_COLOR 0x2201

/* TextureEnvTarget */
#define GL_TEXTURE_ENV 0x2300

/* TextureMagFilter */
#define GL_NEAREST 0x2600
#define GL_LINEAR  0x2601

/* TextureMinFilter */
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST  0x2701
#define GL_NEAREST_MIPMAP_LINEAR  0x2702
#define GL_LINEAR_MIPMAP_LINEAR   0x2703

/* TextureParameterName */
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S     0x2802
#define GL_TEXTURE_WRAP_T     0x2803

/* StencilOp */
#define GL_KEEP    0x1E00
#define GL_REPLACE 0x1E01
#define GL_INCR    0x1E02
#define GL_DECR    0x1E03


#define GL_VENDOR     0x1F00
#define GL_RENDERER   0x1F01
#define GL_VERSION    0x1F02
#define GL_EXTENSIONS 0x1F03


#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_COLOR_BUFFER_BIT 0x00004000


typedef const GLubyte * APIENTRY GL_PROC(glGetString) (GLenum name);
typedef void APIENTRY GL_PROC(glClear) (GLbitfield mask);
typedef void APIENTRY GL_PROC(glClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void APIENTRY GL_PROC(glBlendFunc) (GLenum sfactor, GLenum dfactor);
typedef void APIENTRY GL_PROC(glDisable) (GLenum cap);
typedef void APIENTRY GL_PROC(glEnable) (GLenum cap);
typedef void APIENTRY GL_PROC(glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void APIENTRY GL_PROC(glGenTextures) (GLsizei n, GLuint *textures);
typedef void APIENTRY GL_PROC(glBindTexture) (GLenum target, GLuint texture);
typedef void APIENTRY GL_PROC(glTexImage2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void APIENTRY GL_PROC(glTexParameteri) (GLenum target, GLenum pname, GLint param);
typedef void APIENTRY GL_PROC(glDepthFunc) (GLenum func);
typedef void APIENTRY GL_PROC(glClearDepth) (GLclampd depth);
typedef void APIENTRY GL_PROC(glPolygonMode) (GLenum face, GLenum mode);
typedef void APIENTRY GL_PROC(glPolygonOffset) (GLfloat factor, GLfloat units);
typedef void APIENTRY GL_PROC(glDepthMask) (GLboolean flag);
typedef void APIENTRY GL_PROC(glDepthRange) (GLclampd zNear, GLclampd zFar);
typedef void APIENTRY GL_PROC(glCullFace) (GLenum mode);
typedef void APIENTRY GL_PROC(glLineWidth) (GLfloat width);
typedef void APIENTRY GL_PROC(glFrontFace) (GLenum mode);
typedef void APIENTRY GL_PROC(glScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void APIENTRY GL_PROC(glStencilFunc) (GLenum func, GLint ref, GLuint mask);
typedef void APIENTRY GL_PROC(glStencilMask) (GLuint mask);
typedef void APIENTRY GL_PROC(glStencilOp) (GLenum fail, GLenum zfail, GLenum zpass);
typedef void APIENTRY GL_PROC(glDrawBuffer) (GLenum mode);
typedef void APIENTRY GL_PROC(glColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void APIENTRY GL_PROC(glDrawArrays) (GLenum mode, GLint first, GLsizei count);
typedef void APIENTRY GL_PROC(glDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
typedef void APIENTRY GL_PROC(glDeleteTextures) (GLsizei n, const GLuint *textures);
typedef void APIENTRY GL_PROC(glClearStencil) (GLint s);
typedef void APIENTRY GL_PROC(glReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
typedef void APIENTRY GL_PROC(glPixelStoref) (GLenum pname, GLfloat param);
typedef void APIENTRY GL_PROC(glPixelStorei) (GLenum pname, GLint param);
typedef void APIENTRY GL_PROC(glReadBuffer) (GLenum mode);

GL_PROC(glGetString)     *glGetString;
GL_PROC(glClear)         *glClear;
GL_PROC(glClearColor)    *glClearColor;
GL_PROC(glBlendFunc)     *glBlendFunc;
GL_PROC(glDisable)       *glDisable;
GL_PROC(glEnable)        *glEnable;
GL_PROC(glViewport)      *glViewport;
GL_PROC(glGenTextures)   *glGenTextures;
GL_PROC(glBindTexture)   *glBindTexture;
GL_PROC(glTexImage2D)    *glTexImage2D;
GL_PROC(glTexParameteri) *glTexParameteri;
GL_PROC(glDepthFunc)     *glDepthFunc;
GL_PROC(glClearDepth)    *glClearDepth;
GL_PROC(glPolygonMode)   *glPolygonMode;
GL_PROC(glPolygonOffset) *glPolygonOffset;
GL_PROC(glDepthMask)     *glDepthMask;
GL_PROC(glDepthRange)    *glDepthRange;
GL_PROC(glCullFace)      *glCullFace;
GL_PROC(glLineWidth)     *glLineWidth;
GL_PROC(glFrontFace)     *glFrontFace;
GL_PROC(glScissor)       *glScissor;
GL_PROC(glStencilFunc)   *glStencilFunc;
GL_PROC(glStencilMask)   *glStencilMask;
GL_PROC(glStencilOp)     *glStencilOp;
GL_PROC(glDrawBuffer)    *glDrawBuffer;
GL_PROC(glColorMask)     *glColorMask;
GL_PROC(glDrawArrays)    *glDrawArrays;
GL_PROC(glDrawElements)  *glDrawElements;
GL_PROC(glDeleteTextures)*glDeleteTextures;
GL_PROC(glClearStencil)  *glClearStencil;
GL_PROC(glReadPixels)    *glReadPixels;
GL_PROC(glPixelStoref)   *glPixelStoref;
GL_PROC(glPixelStorei)   *glPixelStorei;
GL_PROC(glReadBuffer)    *glReadBuffer;

/***************** Legacy functions *********************/
typedef void APIENTRY GL_PROC(glBegin) (GLenum mode);
typedef void APIENTRY GL_PROC(glEnd) (void);
typedef void APIENTRY GL_PROC(glVertex3f) (GLfloat x, GLfloat y, GLfloat z);
typedef void APIENTRY GL_PROC(glColor3f) (GLfloat red, GLfloat green, GLfloat blue);
typedef void APIENTRY GL_PROC(glColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void APIENTRY GL_PROC(glMatrixMode) (GLenum mode);
typedef void APIENTRY GL_PROC(glLoadMatrixf) (const GLfloat *m);
typedef void APIENTRY GL_PROC(glTexEnvf) (GLenum target, GLenum pname, GLfloat param);
typedef void APIENTRY GL_PROC(glTexCoord2f) (GLfloat s, GLfloat t);
typedef void APIENTRY GL_PROC(glLoadIdentity) (void);

GL_PROC(glBegin)       *glBegin;
GL_PROC(glEnd)         *glEnd;
GL_PROC(glVertex3f)    *glVertex3f;
GL_PROC(glColor3f)     *glColor3f;
GL_PROC(glColor4f)     *glColor4f;
GL_PROC(glMatrixMode)  *glMatrixMode;
GL_PROC(glLoadMatrixf) *glLoadMatrixf;
GL_PROC(glTexEnvf)     *glTexEnvf;
GL_PROC(glTexCoord2f)  *glTexCoord2f;
GL_PROC(glLoadIdentity)*glLoadIdentity;
/***************** Legacy functions *********************/


typedef struct __GLXcontextRec *GLXContext;
typedef XID GLXDrawable;

/*
 * Tokens for glXChooseVisual and glXGetConfig:
 */
#define GLX_USE_GL           1
#define GLX_BUFFER_SIZE      2
#define GLX_LEVEL            3
#define GLX_RGBA             4
#define GLX_DOUBLEBUFFER     5
#define GLX_STEREO           6
#define GLX_AUX_BUFFERS      7
#define GLX_RED_SIZE         8
#define GLX_GREEN_SIZE       9
#define GLX_BLUE_SIZE        10
#define GLX_ALPHA_SIZE       11
#define GLX_DEPTH_SIZE       12
#define GLX_STENCIL_SIZE     13
#define GLX_ACCUM_RED_SIZE   14
#define GLX_ACCUM_GREEN_SIZE 15
#define GLX_ACCUM_BLUE_SIZE  16
#define GLX_ACCUM_ALPHA_SIZE 17

/*
 * GLX 1.4 and later:
 */
#define GLX_SAMPLE_BUFFERS 0x186a0 /*100000*/
#define GLX_SAMPLES        0x186a1 /*100001*/


typedef Bool GL_PROC(glXQueryVersion)( Display *dpy, int *maj, int *min );
typedef XVisualInfo* GL_PROC(glXChooseVisual)( Display *dpy, int screen, int *attribList );
typedef GLXContext GL_PROC(glXCreateContext)( Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct );
typedef void GL_PROC(glXDestroyContext)( Display *dpy, GLXContext ctx );
typedef Bool GL_PROC(glXMakeCurrent)( Display *dpy, GLXDrawable drawable, GLXContext ctx);
typedef void GL_PROC(glXSwapBuffers)( Display *dpy, GLXDrawable drawable );

typedef void (*__GLXextFuncPtr)(void);
typedef __GLXextFuncPtr GL_PROC(glXGetProcAddress) (const GLubyte *);

static GL_PROC(glXQueryVersion)   *glXQueryVersion;
static GL_PROC(glXChooseVisual)   *glXChooseVisual;
static GL_PROC(glXCreateContext)  *glXCreateContext;
static GL_PROC(glXDestroyContext) *glXDestroyContext;
static GL_PROC(glXMakeCurrent)    *glXMakeCurrent;
static GL_PROC(glXSwapBuffers)    *glXSwapBuffers;
static GL_PROC(glXGetProcAddress) *glXGetProcAddress;

#define GLX_LOAD_PROC(ident) ident = (GL_PROC(ident) *)dlsym(glx_module, #ident)
#define GL1_LOAD_PROC(ident) ident = (GL_PROC(ident) *)glXGetProcAddress((const GLubyte *)#ident)

bool glx_load(void) {
    void *glx_module = dlopen("libGLX.so.0", RTLD_LAZY);
    if (!glx_module) {
        glx_module = dlopen("libGLX.so", RTLD_LAZY);
    }

    if (!glx_module) {
        write_string("Failed to load GLX.\n", true);
        return false;
    }

    GLX_LOAD_PROC(glXQueryVersion);
    GLX_LOAD_PROC(glXChooseVisual);
    GLX_LOAD_PROC(glXCreateContext);
    GLX_LOAD_PROC(glXDestroyContext);
    GLX_LOAD_PROC(glXMakeCurrent);
    GLX_LOAD_PROC(glXSwapBuffers);

    glXGetProcAddress = (glXGetProcAddressPROC *)dlsym(glx_module, "glXGetProcAddressARB");
    if (!glXGetProcAddress) {
        GLX_LOAD_PROC(glXGetProcAddress);
    }


    return true;
}

bool gl_load(void) {
    GL1_LOAD_PROC(glGetString);
    GL1_LOAD_PROC(glClear);
    GL1_LOAD_PROC(glClearColor);
    GL1_LOAD_PROC(glBlendFunc);
    GL1_LOAD_PROC(glDisable);
    GL1_LOAD_PROC(glEnable);
    GL1_LOAD_PROC(glViewport);
    GL1_LOAD_PROC(glGenTextures);
    GL1_LOAD_PROC(glBindTexture);
    GL1_LOAD_PROC(glTexImage2D);
    GL1_LOAD_PROC(glTexParameteri);
    GL1_LOAD_PROC(glDepthFunc);
    GL1_LOAD_PROC(glClearDepth);
    GL1_LOAD_PROC(glPolygonMode);
    GL1_LOAD_PROC(glPolygonOffset);
    GL1_LOAD_PROC(glDepthMask);
    GL1_LOAD_PROC(glDepthRange);
    GL1_LOAD_PROC(glCullFace);
    GL1_LOAD_PROC(glLineWidth);
    GL1_LOAD_PROC(glFrontFace);
    GL1_LOAD_PROC(glScissor);
    GL1_LOAD_PROC(glStencilFunc);
    GL1_LOAD_PROC(glStencilMask);
    GL1_LOAD_PROC(glStencilOp);
    GL1_LOAD_PROC(glDrawBuffer);
    GL1_LOAD_PROC(glColorMask);
    GL1_LOAD_PROC(glDrawElements);
    GL1_LOAD_PROC(glDeleteTextures);
    GL1_LOAD_PROC(glClearStencil);
    GL1_LOAD_PROC(glReadPixels);
    GL1_LOAD_PROC(glPixelStoref);
    GL1_LOAD_PROC(glPixelStorei);
    GL1_LOAD_PROC(glReadBuffer);


    GL1_LOAD_PROC(glBegin);
    GL1_LOAD_PROC(glEnd);
    GL1_LOAD_PROC(glVertex3f);
    GL1_LOAD_PROC(glColor3f);
    GL1_LOAD_PROC(glColor4f);
    GL1_LOAD_PROC(glMatrixMode);
    GL1_LOAD_PROC(glLoadMatrixf);
    GL1_LOAD_PROC(glTexEnvf);
    GL1_LOAD_PROC(glTexCoord2f);
    GL1_LOAD_PROC(glLoadIdentity);

    return true;
}

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

GLuint last_bound_texture_id;

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
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
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
        glBindTexture(GL_TEXTURE_2D, texture->id);
    }

    last_bound_texture_id = texture->id;
}

void draw_quad(float x0, float y0, float x1, float y1, Vector4 c) {
    glBegin(GL_TRIANGLES);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(0, 1);
    glVertex3f(x0, y0, 0);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(1, 1);
    glVertex3f(x1, y0, 0);
    
    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(1, 0);
    glVertex3f(x1, y1,  0);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(0, 1);
    glVertex3f(x0, y0, 0);
    
    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(1, 0);
    glVertex3f(x1, y1,  0);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(0, 0);
    glVertex3f(x0, y1,  0);

    glEnd();
}

void draw_quad(float x0, float y0, float x1, float y1, 
               float u0, float v0, float u1, float v1,
               Vector4 c) {
    glBegin(GL_TRIANGLES);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(u0, v1);
    glVertex3f(x0, y0, 0);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(u1, v1);
    glVertex3f(x1, y0, 0);
    
    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(u1, v0);
    glVertex3f(x1, y1,  0);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(u0, v1);
    glVertex3f(x0, y0, 0);
    
    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(u1, v0);
    glVertex3f(x1, y1,  0);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(u0, v0);
    glVertex3f(x0, y1,  0);

    glEnd();
}

void draw_quad(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
               Vector4 c0, Vector4 c1, Vector4 c2, Vector4 c3) {
    glBegin(GL_TRIANGLES);

    glColor4f(c0.x, c0.y, c0.z, c0.w);
    glTexCoord2f(0, 1);
    glVertex3f(p0.x, p0.y, p0.z);

    glColor4f(c1.x, c1.y, c1.z, c1.w);
    glTexCoord2f(1, 1);
    glVertex3f(p1.x, p1.y, p1.z);
    
    glColor4f(c2.x, c2.y, c2.z, c2.w);
    glTexCoord2f(1, 0);
    glVertex3f(p2.x, p2.y, p2.z);

    glColor4f(c0.x, c0.y, c0.z, c0.w);
    glTexCoord2f(0, 1);
    glVertex3f(p0.x, p0.y, p0.z);

    glColor4f(c2.x, c2.y, c2.z, c2.w);
    glTexCoord2f(1, 0);
    glVertex3f(p2.x, p2.y, p2.z);

    glColor4f(c3.x, c3.y, c3.z, c3.w);
    glTexCoord2f(0, 0);
    glVertex3f(p3.x, p3.y, p3.z);

    glEnd();
}

void rendering_2d(int w, int h) {
    glMatrixMode(GL_PROJECTION);

    float proj[16] = {
        2.0f/w,  0,       0,   0,
        0,       2.0f/h,  0,   0,
        0,       0,       1,   0,
       -1,      -1,       0,   1
    };
    glLoadMatrixf(proj);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool should_quit = false;

int main(void) {
    Display *display = XOpenDisplay(null);
    if (!display) {
        write_string("Failed to XOpenDisplay.\n", true);
        return 0;
    }

    if (!glx_load()) return 0;

    // GLX version.
    GLint majorGLX, minorGLX;
    glXQueryVersion(display, &majorGLX, &minorGLX);
    if ((majorGLX == 1) && (minorGLX < 2)) {
        write_string("You need at least GLX 1.2.\n", true);
        return 0;
    }

    print("GLX Version: %d.%d\n", majorGLX, minorGLX);

    int screen_id   = XDefaultScreen(display);
    u64 black_color = BlackPixel(display, screen_id);
    u64 white_color = WhitePixel(display, screen_id);

    // Choose visual.
    GLint glx_attributes[] = {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_DEPTH_SIZE,    24,
        GLX_STENCIL_SIZE,  8,
        GLX_RED_SIZE,      8,
        GLX_GREEN_SIZE,    8,
        GLX_BLUE_SIZE,     8,
        GLX_SAMPLE_BUFFERS,0,
        GLX_SAMPLES,       0,
        None
    };

    XVisualInfo *visual_info = glXChooseVisual(display, screen_id, glx_attributes);
    if (!visual_info) {
        write_string("Failed to glXChooseVisual.\n", true);
        return 0;
    }

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

    const char *title = "Window Test (Linux)";
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

    GLXContext gl_context = glXCreateContext(display, visual_info, null, True);
    if (!gl_context) {
        write_string("Failed to glXCreateContext.\n", true);
        return 0;
    }

    if (!glXMakeCurrent(display, window, gl_context)) {
        write_string("Failed to glXMakeCurrent.\n", true);
        return 0;
    }

    if (!gl_load()) return 0;

    const GLubyte *gl_version  = glGetString(GL_VERSION);
    const GLubyte *gl_vendor   = glGetString(GL_VENDOR);
    const GLubyte *gl_renderer = glGetString(GL_RENDERER);

    print("OpenGL Version:  %s\n", gl_version);
    print("OpenGL Vendor:   %s\n", gl_vendor);
    print("OpenGL Renderer: %s\n", gl_renderer);

    Texture test = texture_load_from_file("data/textures/Texturtest planar.png");
    Texture cat  = texture_load_from_file("data/textures/cat.png");

    XClearWindow(display, window);
    XMapRaised(display, window);

    XWindowAttributes size_attributes;
    XGetWindowAttributes(display, window, &size_attributes);

    int back_buffer_width  = size_attributes.width;
    int back_buffer_height = size_attributes.height;

    // glDisable(GL_DEPTH_TEST);
    // Depth is mapped as near=-1 and far 1.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!should_quit) {
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

        draw_quad(500, 300, 650, 400, 
                  0, 0, 1, 0.5f,
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
        x0 = 400;
        y0 = 300;
        x1 = 500;
        y1 = 400;
        draw_quad(Vector3{x0,y0,depth}, Vector3{x1,y0,depth}, Vector3{x1,y1,depth}, Vector3{x0,y1,depth},
                  Vector4{1,1,1,1},   Vector4{1,1,1,1},   Vector4{1,1,1,1},   Vector4{1,1,1,1});

        glXSwapBuffers(display, window);
    }

    glXDestroyContext(display, gl_context);

    XFree(visual_info);
    XFreeColormap(display, colormap);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}





#define GENERAL_IMPLEMENTATION
#include "general.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"