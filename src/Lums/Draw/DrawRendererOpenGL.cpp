#include <cstdio>
#include <Lums/Draw/DrawRendererOpenGL.h>
#include <Lums/Window/Window.h>
#include <Lums/OpenGL/GL.h>

using namespace lm;

namespace
{

#if !(defined(LUMS_OS_MACOS))
/* Shader */
PFNGLCREATESHADERPROC       glCreateShader;
PFNGLDELETESHADERPROC       glDeleteShader;
PFNGLSHADERSOURCEPROC       glShaderSource;
PFNGLCOMPILESHADERPROC      glCompileShader;
PFNGLGETSHADERIVPROC        glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC   glGetShaderInfoLog;

/* Program */
PFNGLCREATEPROGRAMPROC      glCreateProgram;
PFNGLUSEPROGRAMPROC         glUseProgram;
PFNGLDELETEPROGRAMPROC      glDeleteProgram;
PFNGLATTACHSHADERPROC       glAttachShader;
PFNGLDETACHSHADERPROC       glDetachShader;
PFNGLLINKPROGRAMPROC        glLinkProgram;
PFNGLGETPROGRAMIVPROC       glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC  glGetProgramInfoLog;

/* Framebuffer */
PFNGLGENFRAMEBUFFERSPROC    glGenFramebuffers;
PFNGLBINDFRAMEBUFFERPROC    glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
PFNGLDRAWBUFFERSPROC        glDrawBuffers;

# define LOAD_GL(x)  do { (*((void**)(&x))) = getOpenGLProcAddr(#x); } while (0)
#else
# define LOAD_GL(x)  do {} while (0)
#endif

GLenum texType2gl(DrawTextureType t)
{
    switch (t)
    {
    case DrawTextureType::Texture2D:
        return GL_TEXTURE_2D;
    }
    return 0;
}

GLenum texFormat2gl(DrawTextureFormat t)
{
    switch (t)
    {
    case DrawTextureFormat::RGBA:
        return GL_RGBA;
    }
    return 0;
}

GLint texFormat2glInternal(DrawTextureFormat t)
{
    switch (t)
    {
    case DrawTextureFormat::RGBA:
        return GL_RGBA8;
    }
    return 0;
}

GLuint compileShader(const char* src, GLenum type)
{
    GLuint shader = glCreateShader(type);
    GLint length = (GLint)std::strlen(src);
    glShaderSource(shader, 1, &src, &length);
    glCompileShader(shader);

    GLint tmp;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &tmp);
    if (tmp != GL_TRUE)
    {
        char* buffer;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &tmp);
        buffer = (char*)std::calloc(1, tmp + 1);
        glGetShaderInfoLog(shader, tmp + 1, nullptr, buffer);
        std::printf("%s\n", buffer);
        std::free(buffer);
    }

    return shader;
}

}

DrawRendererOpenGL::DrawRendererOpenGL(Window& win)
: DrawRenderer(win)
, _context{}
{
    /* Bootstrap OpenGL */
    platformCreateContext();

    /* Print the context infos */
    std::printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    std::printf("OpenGL vendor : %s\n", glGetString(GL_VENDOR));

    /* Load the required GL funcs */
    LOAD_GL(glCreateShader);
    LOAD_GL(glDeleteShader);
    LOAD_GL(glShaderSource);
    LOAD_GL(glCompileShader);
    LOAD_GL(glGetShaderiv);
    LOAD_GL(glGetShaderInfoLog);
    LOAD_GL(glCreateProgram);
    LOAD_GL(glUseProgram);
    LOAD_GL(glDeleteProgram);
    LOAD_GL(glAttachShader);
    LOAD_GL(glDetachShader);
    LOAD_GL(glLinkProgram);
    LOAD_GL(glGetProgramiv);
    LOAD_GL(glGetProgramInfoLog);
    LOAD_GL(glGenFramebuffers);
    LOAD_GL(glBindFramebuffer);
    LOAD_GL(glDeleteFramebuffers);
    LOAD_GL(glFramebufferTexture);
    LOAD_GL(glDrawBuffers);

    /* Register handlers */
#define HANDLER(member)                 ([](void* r, const priv::DrawCommand* cmd) { ((DrawRendererOpenGL*)r)->member(cmd); })
#define REGISTER_HANDLER(cmd, member)   _handlers[(int)priv::DrawCommandType::cmd] = HANDLER(member)

    REGISTER_HANDLER(CreateShader, implCreateShader);
    REGISTER_HANDLER(DestroyShader, implDestroyShader);
    REGISTER_HANDLER(CreateTexture, implCreateTexture);
    REGISTER_HANDLER(DestroyTexture, implDestroyTexture);
    REGISTER_HANDLER(CreateFramebuffer, implCreateFramebuffer);
    REGISTER_HANDLER(DestroyFramebuffer, implDestroyFramebuffer);
    REGISTER_HANDLER(Clear, implClear);
    REGISTER_HANDLER(Draw, implDraw);

#undef REGISTER_HANDLER
#undef HANDLER

    /* Default state */
    _clearColor = Vector4f(0.f, 0.f, 0.f, 1.f);
    _clearDepth = 1.f;
    _clearStencil = 0;

    glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
    glClearDepth(_clearDepth);
    glClearStencil(_clearStencil);
}

DrawRendererOpenGL::~DrawRendererOpenGL()
{
    platformDestroyContext();
}

bool DrawRendererOpenGL::valid() const
{
    return !!_context;
}

void DrawRendererOpenGL::swap()
{
    platformSwap();
}

void DrawRendererOpenGL::implCreateShader(const priv::DrawCommand* cmd)
{
    GLuint program;
    GLuint shaderVertex;
    GLuint shaderFragment;

    shaderVertex = compileShader(cmd->shader.vertexSrc, GL_VERTEX_SHADER);
    shaderFragment = compileShader(cmd->shader.fragmentSrc, GL_FRAGMENT_SHADER);

    std::free(cmd->shader.vertexSrc);
    std::free(cmd->shader.fragmentSrc);

    program = glCreateProgram();
    glAttachShader(program, shaderVertex);
    glAttachShader(program, shaderFragment);
    glLinkProgram(program);

    GLint tmp;
    glGetProgramiv(program, GL_LINK_STATUS, &tmp);
    if (tmp != GL_TRUE)
    {
        char* buffer;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &tmp);
        buffer = (char*)std::calloc(1, tmp + 1);
        glGetProgramInfoLog(program, tmp + 1, nullptr, buffer);
        std::printf("%s\n", buffer);
        std::free(buffer);
    }

    glDetachShader(program, shaderVertex);
    glDetachShader(program, shaderFragment);

    glDeleteShader(shaderVertex);
    glDeleteShader(shaderFragment);

    auto i = cmd->shader.shader.value();
    if (_programs.size() <= i)
        _programs.resize(i + 1);
    _programs[i] = program;
}

void DrawRendererOpenGL::implDestroyShader(const priv::DrawCommand* cmd)
{
    auto i = cmd->shader.shader.value();
    glDeleteProgram(_programs[i]);
    _programs[i] = 0;
}

void DrawRendererOpenGL::implCreateTexture(const priv::DrawCommand* cmd)
{
    auto i = cmd->texture.texture.value();
    auto target = texType2gl(cmd->texture.type);
    auto format = texFormat2gl(cmd->texture.format);
    auto internalFormat = texFormat2glInternal(cmd->texture.format);

    GLuint tex;
    if (_textures.size() <= i)
        _textures.resize(i + 1);
    glGenTextures(1, &tex);
    glBindTexture(target, tex);

    switch (target)
    {
    case GL_TEXTURE_2D:
        glTexImage2D(target, 0, internalFormat, cmd->texture.width, cmd->texture.height, 0, format, GL_UNSIGNED_BYTE, cmd->texture.data);
        break;
    }

    std::free(cmd->texture.data);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    _textures[i] = tex;
}

void DrawRendererOpenGL::implDestroyTexture(const priv::DrawCommand* cmd)
{
    auto i = cmd->texture.texture.value();
    glDeleteTextures(1, &_textures[i]);
    _textures[i] = 0;
}

void DrawRendererOpenGL::implCreateFramebuffer(const priv::DrawCommand* cmd)
{
    auto i = cmd->framebuffer.framebuffer.value();

    GLuint fb;
    GLuint colors[priv::kMaxFramebufferColors] = {0};

    if (_framebuffers.size() <= i)
        _framebuffers.resize(i + 1);

    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    for (int j = 0; j < priv::kMaxFramebufferColors; ++j)
    {
        if (cmd->framebuffer.color[i])
        {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, _textures[cmd->framebuffer.color[j].value()], 0);
            colors[i] = GL_COLOR_ATTACHMENT0 + j;
        }
    }
    glDrawBuffers(priv::kMaxFramebufferColors, colors);

    if (cmd->framebuffer.depth)
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _textures[cmd->framebuffer.depth.value()], 0);

    if (cmd->framebuffer.stencil)
        glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, _textures[cmd->framebuffer.stencil.value()], 0);

    _framebuffers[i] = fb;
}

void DrawRendererOpenGL::implDestroyFramebuffer(const priv::DrawCommand* cmd)
{
    auto i = cmd->framebuffer.framebuffer.value();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, _framebuffers.data() + i);
    _framebuffers[i] = 0;
}

void DrawRendererOpenGL::implClear(const priv::DrawCommand* cmd)
{
    const auto& clear = cmd->clear;
    GLbitfield clearBits{};

    /* Color buffer */
    if (clear.flags & DrawClearMask::Color)
    {
        clearBits |= GL_COLOR_BUFFER_BIT;
        if (_clearColor != clear.color)
        {
            _clearColor = clear.color;
            glClearColor(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);
        }
    }

    /* Depth buffer */
    if (clear.flags & DrawClearMask::Depth)
    {
        clearBits |= GL_DEPTH_BUFFER_BIT;
        if (_clearDepth != clear.depth)
        {
            _clearDepth = clear.depth;
            glClearDepth(_clearDepth);
        }
    }

    /* Stencil buffer */
    if (clear.flags & DrawClearMask::Stencil)
    {
        clearBits |= GL_STENCIL_BUFFER_BIT;
        if (_clearStencil != clear.stencil)
        {
            _clearStencil = clear.stencil;
            glClearStencil(_clearStencil);
        }
    }

    if (clearBits)
        glClear(clearBits);
}

void DrawRendererOpenGL::implDraw(const priv::DrawCommand* cmd)
{
    /* TODO: Actually draw anything */
}
