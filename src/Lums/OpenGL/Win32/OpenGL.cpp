#include <Lums/OpenGL/GL.h>

void* lm::getOpenGLProcAddr(const char* name)
{
    void* addr = wglGetProcAddress(name);
    if (addr == nullptr || addr == (void*)1 || addr == (void*)2 || addr == (void*)3 || addr == (void*)-1)
    {
        HMODULE mod = LoadLibrary(TEXT("opengl32.dll"));
        addr = (void*)GetProcAddress(mod, name);
    }

    return addr;
}
