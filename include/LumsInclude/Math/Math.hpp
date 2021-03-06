/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Math.hpp                                       oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_MATH_HPP
#define LUMS_MATH_HPP

#include <LumsInclude/Math/Vector.hpp>

namespace lm
{
    /**
     * Compute the square distance between two points
     * @param v1 The first point
     * @param v2 The second point
     * @return The square distance between two points
     */
    template <std::size_t N, typename T>
    float
    squareDist(const Vector<N, T>& v1, const Vector<N, T>& v2)
    {
        float d = 0;
        float buf;

        for (std::size_t i = 0; i < N; ++i)
        {
            buf = v1[i] - v2[i];
            d += buf * buf;
        }
        return d;
    }

    /**
     * Compute the distance between two points
     * @param v1 The first point
     * @param v2 The second point
     * @return The distance between two points
     */
    template <std::size_t N, typename T>
    float
    dist(const Vector<N, T>& v1, const Vector<N, T>& v2)
    {
        return std::sqrt(squareDist(v1, v2));
    }

    /**
     * Compute the square length of a vector
     * @param vect The vector
     * @return The square length of the vector
     */
    template <std::size_t N, typename T>
    float
    squareLength(const Vector<N, T>& vect)
    {
        float l = 0;
        float buf;

        for (std::size_t i = 0; i < N; ++i)
        {
            buf = vect[i];
            l += buf * buf;
        }
        return l;
    }

    /**
     * Compute the length of a vector
     * @param vect The vector
     * @return The length of the vector
     */
    template <std::size_t N, typename T>
    float
    length(const Vector<N, T>& vect)
    {
        return std::sqrt(squareLength(vect));
    }

    /**
     * Compute the normal of a vector
     * @param vect The vector
     * @return The normal of the vector
     */
    template <std::size_t N, typename T>
    Vector<N, T>
    normal(const Vector<N, T>& vect)
    {
        float l = length(vect);
        Vector<N, T> v(vect);

        for (std::size_t i = 0; i < N; ++i)
            v[i] /= l;
        return v;
    }

    /**
     * Compute the cross product between two vectors
     * @param u The first vector
     * @param v The second vector
     * @return The cross product between two vectors
     */
    template <typename T>
    Vector3<T>
    cross(const Vector3<T>& u, const Vector3<T>&v)
    {
        Vector3<T> vect;

        vect[0] = u[1] * v[2] - u[2] * v[1];
        vect[1] = u[2] * v[0] - u[0] * v[2];
        vect[2] = u[0] * v[1] - u[1] * v[0];
        return vect;
    }

    /**
     * Compute the dot product between two vectors
     * @param lhs The first vector
     * @param rhs The second vector
     * @return The dot product between two vectors
     */
    template <std::size_t N, typename T>
    T
    dot(const Vector<N, T>& lhs, const Vector<N, T>& rhs)
    {
        T val = T();

        for (std::size_t i = 0; i < N; ++i)
            val += lhs[i] * rhs[i];
        return val;
    }

    /**
     * Check wether a vector is the null vector
     * @param vect The vector
     * @return True if the vector is null
     */
    template <std::size_t N, typename T>
    bool
    null(const Vector<N, T>& vect)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            if (vect[i] != T())
                return false;
        }
        return true;
    }

    inline float
    bezier4p(const Vector2f& p1, const Vector2f& p2, float t)
    {
        Vector2f point = p1 * 3 * t * (1 - t) * (1 - t) + p2 * 3 * t * t * (1 - t) + Vector2f(1.f, 1.f) * t * t * t;
        return point.y;
    }

    static const int sample_size = 10;

    inline float
    bezier4(const Vector2f& p1, const Vector2f& p2, float t)
    {
        Vector2f samples[sample_size];
        for (int i = 0; i < sample_size; ++i)
            samples[i] = bezier4p(p1, p2, i * 0.1f);
        int i;
        for (i = 0; i < sample_size - 1; ++i)
        {
            if (samples[i].x <= t && samples[i + 1].x > t)
                break;
        }
        if (i == sample_size - 1)
            --i;
        float alpha = (t - samples[i].x) / (samples[i + 1].x - samples[i].x);
        return (1 - alpha) * samples[i].y + alpha * samples[i + 1].y;
    }

    inline float
    clampAngle(float angle)
    {
        float turn = angle * (1.f / 360.f);
        turn -= int(turn);
        return turn * 360.f;
    }

    inline float
    interpolate(float a, float b, float alpha)
    {
        return (1.f - alpha) * a + alpha * b;
    }

    inline float
    interpolateAngle(float a, float b, float alpha)
    {
        float ca = clampAngle(a);
        float cb = clampAngle(b);

        if (fabs(ca - cb) > 180.f)
        {
            if (ca < cb)
                ca += 360.f;
            else
                cb += 360.f;
        }
        return clampAngle(interpolate(ca, cb, alpha));
    }

}

#endif
