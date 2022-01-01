#include <Lums/Window/Window.h>
#include <Lums/Draw/DrawRendererVulkan.h>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    lm::Window win("01_Cube", 800, 600);
    lm::DrawRendererVulkan renderer(win);

    win.show();
    for (;;)
    {
        win.poll();
        renderer.clear(lm::DrawFramebuffer(), lm::DrawClearMask::Color, lm::Vector4f(1.f, 0.f, 0.f, 1.f), 1.f, 0);
        renderer.render();
    }

    return 0;
}
