#ifndef LUMS_MATH_ALGEBRA_H
#define LUMS_MATH_ALGEBRA_H

namespace lm
{

template <typename T>
constexpr const T& clamp(const T& value, const T& min, const T& max)
{
    return value < min ? min : (value > max ? max : value);
}

}

#endif /* LUMS_MATH_ALGEBRA_H */
