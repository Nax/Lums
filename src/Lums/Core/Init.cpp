#include <Lums/Core/Init.h>

namespace lm
{

void platformInit();
void platformQuit();

void init()
{
    platformInit();
}

void quit()
{
    platformQuit();
}

}
