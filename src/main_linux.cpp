#include "general.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <dlfcn.h>
#include <time.h>

#include <math.h>

#include "stb_image.h"


#define GL_PROC(ident) CONCAT(ident,PROC)

#ifndef APIENTRY
#define APIENTRY
#endif

#ifdef GL_USE_LEGACY_PROCS
#undef GL_USE_LEGACY_PROCS
#define GL_USE_LEGACY_PROCS 1
#else
#define GL_USE_LEGACY_PROCS 0
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
typedef smm           GLsizeiptr;
typedef char          GLchar;


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

/* Boolean */
#define GL_TRUE  1
#define GL_FALSE 0

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

// GLCOREARB.
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_ARRAY_BUFFER 0x8892
#define GL_STREAM_DRAW  0x88E0
#define GL_STATIC_DRAW  0x88E4
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER   0x8B31
#define GL_COMPILE_STATUS  0x8B81
#define GL_LINK_STATUS     0x8B82
#define GL_TEXTURE0 0x84C0

typedef void APIENTRY GL_PROC(glGenVertexArrays) (GLsizei n, GLuint *arrays);
typedef void APIENTRY GL_PROC(glBindVertexArray) (GLuint array);
typedef void APIENTRY GL_PROC(glGenBuffers) (GLsizei n, GLuint *buffers);
typedef void APIENTRY GL_PROC(glBindBuffer) (GLenum target, GLuint buffer);
typedef void APIENTRY GL_PROC(glBufferData) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void APIENTRY GL_PROC(glCompileShader) (GLuint shader);
typedef GLuint APIENTRY GL_PROC(glCreateProgram) (void);
typedef GLuint APIENTRY GL_PROC(glCreateShader) (GLenum type);
typedef void APIENTRY GL_PROC(glDeleteProgram) (GLuint program);
typedef void APIENTRY GL_PROC(glDeleteShader) (GLuint shader);
typedef void APIENTRY GL_PROC(glDetachShader) (GLuint program, GLuint shader);
typedef void APIENTRY GL_PROC(glDisableVertexAttribArray) (GLuint index);
typedef void APIENTRY GL_PROC(glEnableVertexAttribArray) (GLuint index);
typedef void APIENTRY GL_PROC(glLinkProgram) (GLuint program);
typedef void APIENTRY GL_PROC(glShaderSource) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void APIENTRY GL_PROC(glUseProgram) (GLuint program);
typedef void APIENTRY GL_PROC(glAttachShader) (GLuint program, GLuint shader);
typedef void APIENTRY GL_PROC(glVertexAttribPointer) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void APIENTRY GL_PROC(glGetShaderiv) (GLuint shader, GLenum pname, GLint *params);
typedef void APIENTRY GL_PROC(glGetShaderInfoLog) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void APIENTRY GL_PROC(glUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef GLint APIENTRY GL_PROC(glGetUniformLocation) (GLuint program, const GLchar *name);
typedef void APIENTRY GL_PROC(glActiveTexture) (GLenum texture);
typedef void APIENTRY GL_PROC(glUniform1i) (GLint location, GLint v0);

GL_PROC(glGenVertexArrays) *glGenVertexArrays;
GL_PROC(glBindVertexArray) *glBindVertexArray;
GL_PROC(glGenBuffers)      *glGenBuffers;
GL_PROC(glBindBuffer)      *glBindBuffer;
GL_PROC(glBufferData)      *glBufferData;
GL_PROC(glCompileShader)   *glCompileShader;
GL_PROC(glCreateProgram)   *glCreateProgram;
GL_PROC(glCreateShader)    *glCreateShader;
GL_PROC(glDeleteProgram)   *glDeleteProgram;
GL_PROC(glDeleteShader)    *glDeleteShader;
GL_PROC(glDetachShader)    *glDetachShader;
GL_PROC(glDisableVertexAttribArray) *glDisableVertexAttribArray;
GL_PROC(glEnableVertexAttribArray)  *glEnableVertexAttribArray;
GL_PROC(glLinkProgram)         *glLinkProgram;
GL_PROC(glShaderSource)        *glShaderSource;
GL_PROC(glUseProgram)          *glUseProgram;
GL_PROC(glAttachShader)        *glAttachShader;
GL_PROC(glVertexAttribPointer) *glVertexAttribPointer;
GL_PROC(glGetShaderiv)         *glGetShaderiv;
GL_PROC(glGetShaderInfoLog)    *glGetShaderInfoLog;
GL_PROC(glUniformMatrix4fv)    *glUniformMatrix4fv;
GL_PROC(glGetUniformLocation)  *glGetUniformLocation;
GL_PROC(glActiveTexture)       *glActiveTexture;
GL_PROC(glUniform1i)           *glUniform1i;


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

#if GL_USE_LEGACY_PROCS
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
#endif


// GLX.
typedef struct __GLXcontextRec *GLXContext;
typedef struct __GLXFBConfigRec *GLXFBConfig;
typedef XID GLXDrawable;


#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092
#define GLX_CONTEXT_FLAGS_ARB         0x2094
#define GLX_CONTEXT_PROFILE_MASK_ARB  0x9126

#define GLX_CONTEXT_DEBUG_BIT_ARB              0x0001
#define GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002

#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002


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

#define GLX_X_VISUAL_TYPE 0x22
#define GLX_WINDOW_BIT    0x00000001
#define GLX_RGBA_BIT      0x00000001
#define GLX_TRUE_COLOR    0x8002
#define GLX_DRAWABLE_TYPE 0x8010
#define GLX_RENDER_TYPE   0x8011
#define GLX_X_RENDERABLE  0x8012
#define GLX_RGBA_TYPE     0x8014

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

typedef GLXContext GL_PROC(glXCreateContextAttribsARB)(Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list);
typedef GLXFBConfig *GL_PROC(glXChooseFBConfig)(Display *dpy, int screen, const int *attribList, int *nitems);
typedef int GL_PROC(glXGetFBConfigAttrib)(Display *dpy, GLXFBConfig config, int attribute, int *value);
typedef XVisualInfo *GL_PROC(glXGetVisualFromFBConfig)(Display *dpy, GLXFBConfig config);
typedef const char *GL_PROC(glXQueryExtensionsString)(Display *dpy, int screen);

static GL_PROC(glXQueryVersion)   *glXQueryVersion;
static GL_PROC(glXChooseVisual)   *glXChooseVisual;
static GL_PROC(glXCreateContext)  *glXCreateContext;
static GL_PROC(glXDestroyContext) *glXDestroyContext;
static GL_PROC(glXMakeCurrent)    *glXMakeCurrent;
static GL_PROC(glXSwapBuffers)    *glXSwapBuffers;
static GL_PROC(glXGetProcAddress) *glXGetProcAddress;
static GL_PROC(glXCreateContextAttribsARB) *glXCreateContextAttribsARB;
static GL_PROC(glXChooseFBConfig) *glXChooseFBConfig;
static GL_PROC(glXGetFBConfigAttrib) *glXGetFBConfigAttrib;
static GL_PROC(glXGetVisualFromFBConfig) *glXGetVisualFromFBConfig;
static GL_PROC(glXQueryExtensionsString) *glXQueryExtensionsString;

#define GLX_LOAD_PROC(ident) ident = (GL_PROC(ident) *)dlsym(glx_module, #ident)
#define GL1_LOAD_PROC(ident) ident = (GL_PROC(ident) *)glXGetProcAddress((const GLubyte *)#ident)
#define GL_LOAD_PROC(ident) ident = (GL_PROC(ident) *)glXGetProcAddress((const GLubyte *)#ident)

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
    // @Todo: Fallback to dlsym when glXGetProcAddress Fails?

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
    GL1_LOAD_PROC(glDrawArrays);


#if GL_USE_LEGACY_PROCS
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
#endif

    return true;
}

bool gl_load_extensions(void) {
    GL_LOAD_PROC(glGenVertexArrays);
    GL_LOAD_PROC(glBindVertexArray);
    GL_LOAD_PROC(glGenBuffers);
    GL_LOAD_PROC(glBindBuffer);
    GL_LOAD_PROC(glBufferData);
    GL_LOAD_PROC(glCompileShader);
    GL_LOAD_PROC(glCreateProgram);
    GL_LOAD_PROC(glCreateShader);
    GL_LOAD_PROC(glDeleteProgram);
    GL_LOAD_PROC(glDeleteShader);
    GL_LOAD_PROC(glDetachShader);
    GL_LOAD_PROC(glDisableVertexAttribArray);
    GL_LOAD_PROC(glEnableVertexAttribArray);
    GL_LOAD_PROC(glLinkProgram);
    GL_LOAD_PROC(glShaderSource);
    GL_LOAD_PROC(glUseProgram);
    GL_LOAD_PROC(glAttachShader);
    GL_LOAD_PROC(glVertexAttribPointer);
    GL_LOAD_PROC(glGetShaderiv);
    GL_LOAD_PROC(glGetShaderInfoLog);
    GL_LOAD_PROC(glUniformMatrix4fv);
    GL_LOAD_PROC(glGetUniformLocation);
    GL_LOAD_PROC(glActiveTexture);
    GL_LOAD_PROC(glUniform1i);

    return true;
}

// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
static bool isExtensionSupported(const char *extList, const char *extension) {
    const char *start;
    const char *where, *terminator;

    /* Extension names should not have spaces. */
    where = strchr(extension, ' ');
    if (where || *extension == '\0')
    return false;

    /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
    for (start=extList;;) {
        where = strstr(start, extension);

        if (!where)
            break;

        terminator = where + strlen(extension);

        if ( where == start || *(where - 1) == ' ' )
            if ( *terminator == ' ' || *terminator == '\0' )
                return true;

        start = terminator;
    }

    return false;
}

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

    GL1_LOAD_PROC(glXCreateContextAttribsARB);
    GL1_LOAD_PROC(glXChooseFBConfig);
    GL1_LOAD_PROC(glXGetFBConfigAttrib);
    GL1_LOAD_PROC(glXGetVisualFromFBConfig);
    GL1_LOAD_PROC(glXQueryExtensionsString);

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
    if (!isExtensionSupported(glx_extensions, "GLX_ARB_create_context")) {
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

        print("current_dt = %f, frames = %d\n", current_dt, (s32)(1.0f/current_dt));

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