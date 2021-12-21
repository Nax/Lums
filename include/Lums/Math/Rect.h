#ifndef LUMS_INCLUDED_MATH_RECT_H
#define LUMS_INCLUDED_MATH_RECT_H

#include <Lums/Math/Vector.h>

namespace lm
{

template <typename T, std::size_t N>
class Rect
{
public:
    Rect() {}
    Rect(Vector<T, N> origin, Vector<T, N> size) : origin{origin}, size{size} {}

    Vector<T, N>    origin;
    Vector<T, N>    size;
};

template <typename T> using Rect2 = Rect<T, 2>;
template <typename T> using Rect3 = Rect<T, 3>;
template <typename T> using Rect4 = Rect<T, 4>;

using Rect2i = Rect2<int>;
using Rect3i = Rect3<int>;
using Rect4i = Rect4<int>;

using Rect2f = Rect2<float>;
using Rect3f = Rect3<float>;
using Rect4f = Rect4<float>;

}

#endif
