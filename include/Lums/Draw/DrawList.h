#ifndef LUMS_INCLUDED_DRAW_DRAW_LIST_H
#define LUMS_INCLUDED_DRAW_DRAW_LIST_H

#include <vector>
#include <Lums/Draw/DrawCall.h>
#include <Lums/Math/Rect.h>

namespace lm
{

class DrawList
{
public:
    DrawList();

    auto begin() const { return _calls.begin(); }
    auto cbegin() const { return _calls.cbegin(); }
    auto end() const { return _calls.end(); }
    auto cend() const { return _calls.cend(); }

    DrawFramebuffer framebuffer() const { return _framebuffer; }
    Rect2i          viewport() const { return _viewport; }

    void setFramebuffer(DrawFramebuffer fb) { _framebuffer = fb; }
    void setViewport(Rect2i viewport) { _viewport = viewport; }

    void add(const DrawCall& call);
    void reset();

private:
    DrawFramebuffer         _framebuffer;
    Rect2i                  _viewport;

    std::vector<DrawCall>   _calls;
};

}

#endif /* LUMS_INCLUDED_DRAW_DRAW_LIST_H */
