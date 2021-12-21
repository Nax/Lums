#include <Lums/Window/Window.h>
#include <Lums/Draw/DrawRendererOpenGL.h>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    lm::Window win("01_Cube", 800, 600);
    lm::DrawRendererOpenGL gl(win);

    win.show();
    for (;;)
    {
        win.poll();
        gl.clear(lm::DrawFramebuffer(), lm::DrawClearMask::Color, lm::Vector4f(1.f, 0.f, 0.f, 1.f), 1.f, 0);
        gl.render();
    }

    return 0;
}
