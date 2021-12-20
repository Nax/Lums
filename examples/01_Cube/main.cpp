#include <Lums/Window/Window.h>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    lm::Window win("01_Cube", 800, 600);

    win.show();
    for (;;)
    {
        win.poll();
    }

    return 0;
}
