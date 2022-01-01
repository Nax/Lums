#ifndef LUMS_INCLUDED_OPENGL_GL_H
#define LUMS_INCLUDED_OPENGL_GL_H

#include <Lums/Util/Common.h>
#include <Lums/OpenGL/ABI.h>

#if defined(LUMS_OS_WINDOWS)
# include <windows.h>
# include <GL/gl.h>
# include <Lums/OpenGL/GL/glext.h>
# include <Lums/OpenGL/GL/wglext.h>
#elif defined(LUMS_OS_MACOS)
# include <OpenGL/gl3.h>
#endif

namespace lm
{

LUMS_API_OPENGL void* getOpenGLProcAddr(const char* name);

}

#endif /* LUMS_INCLUDED_OPENGL_OPENGL_H */
