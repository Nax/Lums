#include <LumsTest/LumsTest.h>
#include <Lums/Math/Vector.h>

using namespace lm;

LT_TEST_SUITE("Vector")
{

LT_TEST("Vector::Vector()")
{
    Vector4f v1;
    LT_ASSERT_EQ(v1.x, 0.f);
    LT_ASSERT_EQ(v1.y, 0.f);
    LT_ASSERT_EQ(v1.z, 0.f);
    LT_ASSERT_EQ(v1.w, 0.f);
}

LT_TEST("Vector::Vector(x,y)")
{
    Vector2i v1(1, 2);
    LT_ASSERT_EQ(v1.x, 1);
    LT_ASSERT_EQ(v1.y, 2);
}

LT_TEST("Vector::Vector(x,y,z)")
{
    Vector3i v1(1, 2, 3);
    LT_ASSERT_EQ(v1.x, 1);
    LT_ASSERT_EQ(v1.y, 2);
    LT_ASSERT_EQ(v1.z, 3);
}

LT_TEST("Vector::Vector(x,y,z,w)")
{
    Vector4i v1(1, 2, 3, 4);
    LT_ASSERT_EQ(v1.x, 1);
    LT_ASSERT_EQ(v1.y, 2);
    LT_ASSERT_EQ(v1.z, 3);
    LT_ASSERT_EQ(v1.w, 4);
}

LT_TEST("Vector::data()")
{
    Vector4i v1(1, 2, 3, 4);
    auto d = v1.data();

    LT_ASSERT_EQ(d[0], 1);
    LT_ASSERT_EQ(d[1], 2);
    LT_ASSERT_EQ(d[2], 3);
    LT_ASSERT_EQ(d[3], 4);
}

LT_TEST("Vector::operator[]")
{
    Vector4i v1(1, 2, 3, 4);

    LT_ASSERT_EQ(v1.data(), &v1[0]);

    LT_ASSERT_EQ(v1[0], 1);
    LT_ASSERT_EQ(v1[1], 2);
    LT_ASSERT_EQ(v1[2], 3);
    LT_ASSERT_EQ(v1[3], 4);
}

LT_TEST("Vector::operator[] (assign)")
{
    Vector4i v1(1, 2, 3, 4);

    v1[0] = 5;
    v1[1] = 6;
    v1[2] = 7;
    v1[3] = 8;

    LT_ASSERT_EQ(v1[0], 5);
    LT_ASSERT_EQ(v1[1], 6);
    LT_ASSERT_EQ(v1[2], 7);
    LT_ASSERT_EQ(v1[3], 8);
}

LT_TEST("Vector::operator+=(scalar)")
{
    Vector4i v1(1, 2, 3, 4);
    v1 += 1;

    LT_ASSERT_EQ(v1[0], 2);
    LT_ASSERT_EQ(v1[1], 3);
    LT_ASSERT_EQ(v1[2], 4);
    LT_ASSERT_EQ(v1[3], 5);
}

LT_TEST("Vector::operator-=(scalar)")
{
    Vector4i v1(1, 2, 3, 4);
    v1 -= 1;

    LT_ASSERT_EQ(v1[0], 0);
    LT_ASSERT_EQ(v1[1], 1);
    LT_ASSERT_EQ(v1[2], 2);
    LT_ASSERT_EQ(v1[3], 3);
}

LT_TEST("Vector::operator*=(scalar)")
{
    Vector4i v1(1, 2, 3, 4);
    v1 *= 10;

    LT_ASSERT_EQ(v1[0], 10);
    LT_ASSERT_EQ(v1[1], 20);
    LT_ASSERT_EQ(v1[2], 30);
    LT_ASSERT_EQ(v1[3], 40);
}

LT_TEST("Vector::operator/=(scalar)")
{
    Vector4i v1(5, 10, 15, 20);
    v1 /= 5;

    LT_ASSERT_EQ(v1[0], 1);
    LT_ASSERT_EQ(v1[1], 2);
    LT_ASSERT_EQ(v1[2], 3);
    LT_ASSERT_EQ(v1[3], 4);
}

LT_TEST("Vector::operator%=(scalar)")
{
    Vector4i v1(5, 10, 15, 20);
    v1 %= 10;

    LT_ASSERT_EQ(v1[0], 5);
    LT_ASSERT_EQ(v1[1], 0);
    LT_ASSERT_EQ(v1[2], 5);
    LT_ASSERT_EQ(v1[3], 0);
}

LT_TEST("Vector::operator+=(vector)")
{
    Vector4i v1(1, 2, 3, 4);
    v1 += Vector4i(2, 4, 6, 8);

    LT_ASSERT_EQ(v1[0], 3);
    LT_ASSERT_EQ(v1[1], 6);
    LT_ASSERT_EQ(v1[2], 9);
    LT_ASSERT_EQ(v1[3], 12);
}

LT_TEST("Vector::operator-=(vector)")
{
    Vector4i v1(1, 2, 3, 4);
    v1 -= Vector4i(0, 0, 1, 1);

    LT_ASSERT_EQ(v1[0], 1);
    LT_ASSERT_EQ(v1[1], 2);
    LT_ASSERT_EQ(v1[2], 2);
    LT_ASSERT_EQ(v1[3], 3);
}

LT_TEST("Vector::operator*=(vector)")
{
    Vector4i v1(1, 2, 3, 4);
    v1 *= Vector4i(1, 2, 3, 4);

    LT_ASSERT_EQ(v1[0], 1);
    LT_ASSERT_EQ(v1[1], 4);
    LT_ASSERT_EQ(v1[2], 9);
    LT_ASSERT_EQ(v1[3], 16);
}

LT_TEST("Vector::operator/=(vector)")
{
    Vector4i v1(10, 20, 30, 40);
    v1 /= Vector4i(1, 2, 5, 10);

    LT_ASSERT_EQ(v1[0], 10);
    LT_ASSERT_EQ(v1[1], 10);
    LT_ASSERT_EQ(v1[2], 6);
    LT_ASSERT_EQ(v1[3], 4);
}

LT_TEST("Vector::operator%=(vector)")
{
    Vector4i v1(10, 20, 30, 40);
    v1 %= Vector4i(6, 12, 7, 3);

    LT_ASSERT_EQ(v1[0], 4);
    LT_ASSERT_EQ(v1[1], 8);
    LT_ASSERT_EQ(v1[2], 2);
    LT_ASSERT_EQ(v1[3], 1);
}

LT_TEST("operator==(v1,v2)")
{
    Vector4i v1(1, 2, 3, 4);
    Vector4i v2(1, 2, 3, 4);
    Vector4i v3(1, 2, 3, 5);

    LT_ASSERT_TRUE(v1 == v2);
    LT_ASSERT_FALSE(v1 == v3);
}

LT_TEST("operator!=(v1,v2)")
{
    Vector4i v1(1, 2, 3, 4);
    Vector4i v2(1, 2, 3, 4);
    Vector4i v3(1, 2, 3, 5);

    LT_ASSERT_FALSE(v1 != v2);
    LT_ASSERT_TRUE(v1 != v3);
}

LT_TEST("abs(vector)")
{
    Vector2f v1(0.f, 0.f);
    Vector2f v2(1.f, 1.f);
    Vector2f v3(3.f, 4.f);

    LT_ASSERT_EQF(abs(v1), 0.f, 0.01f);
    LT_ASSERT_EQF(abs(v2), 1.414f, 0.01f);
    LT_ASSERT_EQF(abs(v3), 5.f, 0.01f);
}

LT_TEST("normal(vector)")
{
    Vector2f v1(4.f, 0.f);
    Vector2f v2(1.f, 1.f);
    Vector2f v3(4.f, 1.f);

    LT_ASSERT_EQF(normal(v1).x, 1.f, 0.01f);
    LT_ASSERT_EQF(normal(v1).y, 0.f, 0.01f);

    LT_ASSERT_EQF(normal(v2).x, 0.7071f, 0.01f);
    LT_ASSERT_EQF(normal(v2).y, 0.7071f, 0.01f);

    LT_ASSERT_EQF(normal(v3).x, 0.9701f, 0.01f);
    LT_ASSERT_EQF(normal(v3).y, 0.2425f, 0.01f);
}

}
