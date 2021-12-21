#include <Lums/Draw/DrawList.h>

using namespace lm;

DrawList::DrawList()
{

}

void DrawList::add(const DrawCall& call)
{
    _calls.push_back(call);
}

void DrawList::reset()
{
    *this = DrawList();
}
