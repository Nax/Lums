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
    }

    return 0;
}
