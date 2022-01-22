#include <Lums/Core/Init.h>
#include <Lums/Window/Window.h>
#include <Lums/Draw/DrawRenderer.h>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    lm::init();

    lm::Window win("01_Cube", 800, 600);
    lm::DrawRenderer* renderer = lm::DrawRenderer::create(win);

    win.show();
    for (;;)
    {
        win.poll();
        renderer->clear(lm::DrawFramebuffer(), lm::DrawClearMask::Color, lm::Vector4f(1.f, 0.f, 0.f, 1.f), 1.f, 0);
        renderer->render();
    }

    delete renderer;

    lm::quit();

    return 0;
}
