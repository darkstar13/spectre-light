#include "spectre_tests.h"

#ifdef __XCODE__
#import <XCTest/XCTest.h>
#elif defined(__VS__)
#include "CppUnitTest.h"
#else

#include <gtest/gtest.h>

#endif

#include "geometry/vec3.hpp"
#include "geometry/vec2.hpp"
#include "utility/utility.hpp"
#include <climits>

SPECTRE_TEST_INIT(Vec3_tests)

SPECTRE_TEST(Vec3, empty_constructor)
{
    Vec3 v;
    EXPECT_EQ(v.x, 0.0f);
    EXPECT_EQ(v.y, 0.0f);
    EXPECT_EQ(v.z, 0.0f);
}

SPECTRE_TEST(Vec3, given_constructor)
{
    Vec3 v(1.0, 0.0, -1.0);
    EXPECT_EQ(v.x, 1.0f);
    EXPECT_EQ(v.y, 0.0f);
    EXPECT_EQ(v.z, -1.0f);

    errors_count[ERROR_INDEX] = 0;
    v = Vec3(NAN, 0.f, 0.f);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
    v = Vec3(0.f, NAN, 0.f);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
    v = Vec3(0.f, 0.f, NAN);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
    v = Vec3(INFINITY, 0.f, 0.f);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
    v = Vec3(0.f, INFINITY, 0.f);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
    v = Vec3(0.f, 0.f, INFINITY);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
}

SPECTRE_TEST(Vec3, same_value_constructor)
{
    float f = 12.5;
    Vec3 v(f);
    EXPECT_EQ(v.x, f);
    EXPECT_EQ(v.y, f);
    EXPECT_EQ(v.z, f);

    errors_count[ERROR_INDEX] = 0;
    Vec3 v2(NAN);
    EXPECT_GT(errors_count[ERROR_INDEX], 0);

    errors_count[ERROR_INDEX] = 0;
    Vec3 v3(INFINITY);
    EXPECT_GT(errors_count[ERROR_INDEX], 0);
}

SPECTRE_TEST(Vec3, vec2_constructor)
{
    Vec2 v2(1.0f, -1.0f);
    float f = 17.11;
    Vec3 v3(v2, f);
    EXPECT_EQ(v3.x, v2.x);
    EXPECT_EQ(v3.y, v2.y);
    EXPECT_EQ(v3.z, f);

    errors_count[ERROR_INDEX] = 0;
    Vec2 v4a(NAN, 0);
    Vec3 v4(v4a, 1.f);
    EXPECT_GT(errors_count[ERROR_INDEX], 0);
    errors_count[ERROR_INDEX] = 0;
    Vec2 v4b(0, NAN);
    v4 = Vec3(v4b, 1.f);
    EXPECT_GT(errors_count[ERROR_INDEX], 0);
    errors_count[ERROR_INDEX] = 0;
    Vec2 v4o(0.f, 0.f);
    v4 = Vec3(v4o, NAN);
    EXPECT_GT(errors_count[ERROR_INDEX], 0);

    errors_count[ERROR_INDEX] = 0;
    v4a = Vec2(INFINITY, 0);
    v4 = Vec3(v4a, 1.f);
    EXPECT_GT(errors_count[ERROR_INDEX], 0);
    errors_count[ERROR_INDEX] = 0;
    v4b = Vec2(0, INFINITY);
    v4 = Vec3(v4b, 1.f);
    EXPECT_GT(errors_count[ERROR_INDEX], 0);
    errors_count[ERROR_INDEX] = 0;
    v4o = Vec2(0.f, 0.f);
    v4 = Vec3(v4o, INFINITY);
    EXPECT_GT(errors_count[ERROR_INDEX], 0);
}

SPECTRE_TEST(Vec3, array_constructor)
{
    float array[] = {1.2f, 1.5f, 1.8f};
    Vec3 v(array);
    EXPECT_EQ(v.x, array[0]);
    EXPECT_EQ(v.y, array[1]);
    EXPECT_EQ(v.z, array[2]);

    errors_count[ERROR_INDEX] = 0;
    array[0] = NAN;
    v = Vec3(array);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
    array[0] = INFINITY;
    v = Vec3(array);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
    array[0] = 0.f;
    array[1] = NAN;
    v = Vec3(array);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
    array[1] = INFINITY;
    v = Vec3(array);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
    array[1] = 0.f;
    array[2] = NAN;
    v = Vec3(array);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
    array[2] = INFINITY;
    v = Vec3(array);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
}

SPECTRE_TEST(Vec3, array_constructor_null_pointer)
{
    float* array = NULL;
    Vec3 v(array);
    EXPECT_EQ(v.x, 0.0f);
    EXPECT_EQ(v.y, 0.0f);
    EXPECT_EQ(v.z, 0.0f);
}

SPECTRE_TEST(Vec3, normal_constructor)
{
    Normal n(1.0, 2.0, 3.0);
    Vec3 v(n);

    EXPECT_EQ(v.x, n.x);
    EXPECT_EQ(v.y, n.y);
    EXPECT_EQ(v.z, n.z);

    Normal n1(2.0, 5.0, 4.0);
    Vec3 v1(n1);
    EXPECT_EQ(v.x, n.x);
    EXPECT_EQ(v.y, n.y);
    EXPECT_EQ(v.z, n.z);

    n1 = Normal(NAN, 0.f, 0.f);
    errors_count[ERROR_INDEX] = 0;
    v1 = Vec3(n1);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    n1 = Normal(0.f, NAN, 0.f);
    errors_count[ERROR_INDEX] = 0;
    v1 = Vec3(n1);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    n1 = Normal(0.f, 0.f, NAN);
    errors_count[ERROR_INDEX] = 0;
    v1 = Vec3(n1);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    n1 = Normal(INFINITY, 0.f, 0.f);
    errors_count[ERROR_INDEX] = 0;
    v1 = Vec3(n1);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    n1 = Normal(0.f, INFINITY, 0.f);
    errors_count[ERROR_INDEX] = 0;
    v1 = Vec3(n1);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    n1 = Normal(0.f, 0.f, INFINITY);
    errors_count[ERROR_INDEX] = 0;
    v1 = Vec3(n1);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
}

SPECTRE_TEST(Vec3, clone)
{
    Vec3 v(-83.27705, 79.29129, -51.32018);
    Vec3 v2 = v;

    EXPECT_EQ(v.x, v2.x);
    EXPECT_EQ(v.y, v2.y);
    EXPECT_EQ(v.z, v2.z);
}

SPECTRE_TEST(Vec3, dot_noinline)
{
    Vec3 v(1, 2, 3);
    Vec3 v2(4, -5, 6);
    EXPECT_EQ(v.dot(v2), 12.f);

    Vec3 v3(6, -1, 3);
    Vec3 v4(4, 18, -2);
    EXPECT_EQ(v3.dot(v4), 0.f);

    Normal n1(6, -1, 3);
    EXPECT_EQ(v2.dot(n1), 47.f);
}

SPECTRE_TEST(Vec3, cross_noinline)
{
    Vec3 v0(3, -3, 1);
    Vec3 v1(4, 9, 2);
    Vec3 outv;
    Vec3 out2;

    outv = v0.cross(v1);
    v0.cross(v1, &out2);

    Vec3 res(-15, -2, 39);

    EXPECT_EQ(outv.x, -15.f);
    EXPECT_EQ(outv.y, -2.f);
    EXPECT_EQ(outv.z, 39.f);
    EXPECT_EQ(outv.x, out2.x);
    EXPECT_EQ(outv.y, out2.y);
    EXPECT_EQ(outv.z, out2.z);
}

SPECTRE_TEST(Vec3, length)
{
    Vec3 v(-15, -2, 39);
    float length = v.length();
    EXPECT_NEAR(length, 41.833f, 1e-5f);

    Vec3 v1(-32, -53, 23);
    EXPECT_EQ(v1.length2(), 4362.f);
    EXPECT_NEAR(v1.length()*v1.length(), v1.length2(), 1e-5f);

    Vec3 v2;
    EXPECT_EQ(v2.length(), 0.f);
}

SPECTRE_TEST(Vec3, normalize_noinline)
{
    Vec3 v1(3, 1, 2);
    EXPECT_NEAR(v1.length(), 3.7416575f, 1e-5f);
    EXPECT_FALSE(v1.is_normalized());

    Vec3 v2 = normalize(v1);

    v1.normalize();
    EXPECT_EQ(v1.x, v2.x);
    EXPECT_EQ(v1.y, v2.y);
    EXPECT_EQ(v1.z, v2.z);
    EXPECT_TRUE(v1.is_normalized());
    EXPECT_TRUE(v2.is_normalized());

    Vec3 vzero;
    vzero.normalize();
    EXPECT_EQ(errors_count[CRITICAL_INDEX], 1);
    errors_count[CRITICAL_INDEX] = 0;

    //already normalized
    EXPECT_TRUE(v1.is_normalized());
    v1.normalize();
    EXPECT_TRUE(v1.is_normalized());
}

SPECTRE_TEST(Vec3, toArray)
{
    Vec3 v(0, 10, -1);
    float f[3];
    v.to_array(f);

    EXPECT_EQ(v.x, f[0]);
    EXPECT_EQ(v.y, f[1]);
    EXPECT_EQ(v.z, f[2]);
}

SPECTRE_TEST(Vec3, toString)
{
    Vec3 v(0, 1, 0);
    std::string str = v.to_string();
    EXPECT_STREQ(str.c_str(), "Vec3[0.000000, 1.000000, 0.000000]");
}

SPECTRE_TEST(Vec3, sum_vector)
{
    Vec3 v1(1, 2, 3);
    Vec3 v2(4, 5.5, -3);

    Vec3 res = v1+v2;
    EXPECT_NEAR(res.x, 5.f, 1e-5f);
    EXPECT_NEAR(res.y, 7.5f, 1e-5f);
    EXPECT_NEAR(res.z, 0.f, 1e-5f);
}

SPECTRE_TEST(Vec3, sum_float)
{
    Vec3 v1(1, 2, -3);
    float f = -0.75;

    Vec3 res = v1+f;
    EXPECT_NEAR(res.x, 0.25f, 1e-5f);
    EXPECT_NEAR(res.y, 1.25f, 1e-5f);
    EXPECT_NEAR(res.z, -3.75f, 1e-5f);
}

SPECTRE_TEST(Vec3, sum_vector_this)
{
    Vec3 v1(1, 2, 3);
    Vec3 v2(4, 5.5, -3);

    v1 += v2;
    EXPECT_NEAR(v1.x, 5.f, 1e-5f);
    EXPECT_NEAR(v1.y, 7.5f, 1e-5f);
    EXPECT_NEAR(v1.z, 0.f, 1e-5f);
}

SPECTRE_TEST(Vec3, sum_float_this)
{
    Vec3 v1(3.3, 1.2, -1.5);
    float f = 0.5;
    v1 += f;
    EXPECT_NEAR(v1.x, 3.8f, 1e-5f);
    EXPECT_NEAR(v1.y, 1.7f, 1e-5f);
    EXPECT_NEAR(v1.z, -1.f, 1e-5f);
}

SPECTRE_TEST(Vec3, sub_vector)
{
    Vec3 v1(7, 6.5, -5);
    Vec3 v2(7, 5.3, -4);

    Vec3 res = v1-v2;
    EXPECT_NEAR(res.x, 0.f, 1e-5f);
    EXPECT_NEAR(res.y, 1.2f, 1e-5f);
    EXPECT_NEAR(res.z, -1.f, 1e-5f);
}

SPECTRE_TEST(Vec3, sub_float)
{
    Vec3 v1(3.3, 1.2, -1.5);
    float f = 0.5;
    Vec3 res = v1-f;
    EXPECT_NEAR(res.x, 2.8f, 1e-5f);
    EXPECT_NEAR(res.y, 0.7f, 1e-5f);
    EXPECT_NEAR(res.z, -2.0f, 1e-5f);
}

SPECTRE_TEST(Vec3, sub_vector_this)
{
    Vec3 v1(5, 3, 1);
    Vec3 v2(7, 3.3, 4);

    v1 -= v2;
    EXPECT_NEAR(v1.x, -2.f, 1e-5f);
    EXPECT_NEAR(v1.y, -0.3f, 1e-5f);
    EXPECT_NEAR(v1.z, -3.f, 1e-5f);
}

SPECTRE_TEST(Vec3, sub_float_this)
{
    Vec3 v1(3.3, 1.2, -1.5);
    float f = 0.5;
    v1 -= f;
    EXPECT_NEAR(v1.x, 2.8f, 1e-5f);
    EXPECT_NEAR(v1.y, 0.7f, 1e-5f);
    EXPECT_NEAR(v1.z, -2.0f, 1e-5f);
}

SPECTRE_TEST(Vec3, mul_vector)
{
    Vec3 v1(1.2, 5.6, 87.3);
    Vec3 v2(1.4, 45, -.004);

    Vec3 res = v1*v2;
    EXPECT_NEAR(res.x, 1.68f, 1e-5f);
    EXPECT_NEAR(res.y, 252.f, 1e-5f);
    EXPECT_NEAR(res.z, -0.3492f, 1e-5f);
}

SPECTRE_TEST(Vec3, mul_float)
{
    Vec3 v1(3.3, 1.2, -1.5);
    float f = 7.33;
    Vec3 res = v1*f;
    EXPECT_NEAR(res.x, 24.189f, 1e-5f);
    EXPECT_NEAR(res.y, 8.796f, 1e-5f);
    EXPECT_NEAR(res.z, -10.995f, 1e-5f);
}

SPECTRE_TEST(Vec3, mul_vector_this)
{
    Vec3 v1(1, 0, 3);
    Vec3 v2(2, 3, 7);

    v1 *= v2;
    EXPECT_NEAR(v1.x, 2.f, 1e-5f);
    EXPECT_NEAR(v1.y, 0.f, 1e-5f);
    EXPECT_NEAR(v1.z, 21.f, 1e-5f);
}

SPECTRE_TEST(Vec3, mul_float_this)
{
    Vec3 v1(3.3, 1.2, -1.5);
    float f = -53.477;
    v1 *= f;
    EXPECT_NEAR(v1.x, -176.4741f, 1e-5f);
    EXPECT_NEAR(v1.y, -64.1724f, 1e-5f);
    EXPECT_NEAR(v1.z, 80.2155f, 1e-5f);
}

SPECTRE_TEST(Vec3, div_vector)
{
    Vec3 v1(7, 8.4, 7);
    Vec3 v2(-6, -0.4, 2);

    Vec3 res = v1/v2;
    EXPECT_NEAR(res.x, (7.0f/-6.0f), 1e-5f);
    EXPECT_NEAR(res.y, (8.4f/-0.4f), 1e-5f);
    EXPECT_NEAR(res.z, (7.0f/2.0f), 1e-5f);

    Vec3 vzerox(0, 1, 1);
    Vec3 vzeroy(1, 0, 1);
    Vec3 vzeroz(1, 1, 0);

    v1/vzerox;
    v1/vzeroy;
    v1/vzeroz;

    EXPECT_EQ(errors_count[CRITICAL_INDEX], 3);
    errors_count[CRITICAL_INDEX] = 0;
}

SPECTRE_TEST(Vec3, div_float)
{
    Vec3 v1(3.3, 1.2, -1.5);
    float f = 7.33;
    Vec3 res = v1/f;
    EXPECT_NEAR(res.x, 3.3f/7.33f, 1e-5f);
    EXPECT_NEAR(res.y, 1.2f/7.33f, 1e-5f);
    EXPECT_NEAR(res.z, -1.5f/7.33f, 1e-5f);

    float zero = 0.f;
    v1/zero;
    EXPECT_EQ(errors_count[CRITICAL_INDEX], 1);
    errors_count[CRITICAL_INDEX] = 0;
}

SPECTRE_TEST(Vec3, div_vector_this)
{
    Vec3 v1(1, 0, 3);
    Vec3 v2(2, 3, 7);

    v1 /= v2;
    EXPECT_NEAR(v1.x, 1.0f/2.0f, 1e-5f);
    EXPECT_NEAR(v1.y, 0.0f/3.0f, 1e-5f);
    EXPECT_NEAR(v1.z, 3.0f/7.0f, 1e-5f);

    Vec3 vzerox(0, 1, 1);
    Vec3 vzeroy(1, 0, 1);
    Vec3 vzeroz(1, 1, 0);

    v2 = v1;
    v2 /= vzerox;
    v2 = v1;
    v2 /= vzeroy;
    v2 = v1;
    v2 /= vzeroz;

    EXPECT_EQ(errors_count[CRITICAL_INDEX], 3);
    errors_count[CRITICAL_INDEX] = 0;
}

SPECTRE_TEST(Vec3, div_float_this)
{
    Vec3 v1(3.3, 1.2, -1.5);
    float f = -53.477;
    v1 /= f;
    EXPECT_NEAR(v1.x, 3.3f/-53.477f, 1e-5f);
    EXPECT_NEAR(v1.y, 1.2f/-53.477f, 1e-5f);
    EXPECT_NEAR(v1.z, -1.5f/-53.477f, 1e-5f);


    float zero = 0.f;
    v1 /= zero;
    EXPECT_EQ(errors_count[CRITICAL_INDEX], 1);
    errors_count[CRITICAL_INDEX] = 0;
}

SPECTRE_TEST(Vec3, not_vector)
{
    Vec3 v1(3, 4.55556, -18);
    Vec3 res = !v1;

    EXPECT_NEAR(res.x, -3.f, 1e-5f);
    EXPECT_NEAR(res.y, -4.55556f, 1e-5f);
    EXPECT_NEAR(res.z, 18.f, 1e-5f);
}

SPECTRE_TEST(Vec3, minus_vector)
{
    Vec3 v1(6, 8.5, -3.76);
    Vec3 res = -v1;

    EXPECT_NEAR(res.x, -6.f, 1e-5f);
    EXPECT_NEAR(res.y, -8.5f, 1e-5f);
    EXPECT_NEAR(res.z, 3.76f, 1e-5f);
}

SPECTRE_TEST(Vec3, equal_vector)
{
    //[X VALUE] [Y VALUE] [Z VALUE]
    //[F] [F] [F]
    Vec3 v1(1.4, 2.5, 3.6);
    Vec3 v2(4.9, -5.8, 6.7);
    bool res = v1 == v2;
    EXPECT_EQ(res, false);

    //[T] [F] [F]
    v1 = Vec3(4.9, 2.5, 3.6);
    v2 = Vec3(4.9, -5.8, 6.7);
    res = v1 == v2;
    EXPECT_EQ(res, false);

    //[F] [T] [F]
    v1 = Vec3(1.2, -5.8, 3.6);
    v2 = Vec3(4.9, -5.8, 6.7);
    res = v1 == v2;
    EXPECT_EQ(res, false);

    //[F] [F] [T]
    v1 = Vec3(4.5, 2.5, 6.7);
    v2 = Vec3(4.9, -5.8, 6.7);
    res = v1 == v2;
    EXPECT_EQ(res, false);

    //[T] [T] [F]
    v1 = Vec3(4.9, -5.8, 3.6);
    v2 = Vec3(4.9, -5.8, 6.7);
    res = v1 == v2;
    EXPECT_EQ(res, false);

    //[T] [F] [T]
    v1 = Vec3(4.9, 2.5, 3.6);
    v2 = Vec3(4.9, -5.8, 3.6);
    res = v1 == v2;
    EXPECT_EQ(res, false);

    //[F] [T] [T]
    v1 = Vec3(2.9, 2.5, 3.6);
    v2 = Vec3(4.9, 2.5, 3.6);
    res = v1 == v2;
    EXPECT_EQ(res, false);

    //[T] [T] [T]
    v1 = Vec3(4.9, -5.8, 6.7);
    v2 = Vec3(4.9, -5.8, 6.7);
    res = v1 == v2;
    EXPECT_EQ(res, true);
}

SPECTRE_TEST(Vec3, not_equal_vector)
{
    //[X VALUE] [Y VALUE] [Z VALUE]
    //[F] [F] [F]
    Vec3 v1(1.4, 2.5, 3.6);
    Vec3 v2(4.9, -5.8, 6.7);
    bool res = v1 != v2;
    EXPECT_EQ(res, true);

    //[T] [F] [F]
    v1 = Vec3(4.9, 2.5, 3.6);
    v2 = Vec3(4.9, -5.8, 6.7);
    res = v1 != v2;
    EXPECT_EQ(res, true);

    //[F] [T] [F]
    v1 = Vec3(1.2, -5.8, 3.6);
    v2 = Vec3(4.9, -5.8, 6.7);
    res = v1 != v2;
    EXPECT_EQ(res, true);

    //[F] [F] [T]
    v1 = Vec3(4.5, 2.5, 6.7);
    v2 = Vec3(4.9, -5.8, 6.7);
    res = v1 != v2;
    EXPECT_EQ(res, true);

    //[T] [T] [F]
    v1 = Vec3(4.9, -5.8, 3.6);
    v2 = Vec3(4.9, -5.8, 6.7);
    res = v1 != v2;
    EXPECT_EQ(res, true);

    //[T] [F] [T]
    v1 = Vec3(4.9, 2.5, 3.6);
    v2 = Vec3(4.9, -5.8, 3.6);
    res = v1 != v2;
    EXPECT_EQ(res, true);

    //[F] [T] [T]
    v1 = Vec3(2.9, 2.5, 3.6);
    v2 = Vec3(4.9, 2.5, 3.6);
    res = v1 != v2;
    EXPECT_EQ(res, true);

    //[T] [T] [T]
    v1 = Vec3(4.9, -5.8, 6.7);
    v2 = Vec3(4.9, -5.8, 6.7);
    res = v1 != v2;
    EXPECT_EQ(res, false);
}

SPECTRE_TEST(Vec3, access)
{
    Vec3 v(-72.74395f, -6.04488f, -51.11608f);
    EXPECT_EQ(v.x, v[0]);
    EXPECT_EQ(v.y, v[1]);
    EXPECT_EQ(v.z, v[2]);
}

SPECTRE_TEST(Vec3, const_access)
{
    const Vec3 v2(-72.74395f, -6.04488f, -51.11608f);
    EXPECT_EQ(v2.x, v2[0]);
    EXPECT_EQ(v2.y, v2[1]);
    EXPECT_EQ(v2.z, v2[2]);
}

SPECTRE_TEST(Vec3, clamp_noinline)
{
    Vec3 sample(4.9, -5.8, 3.6);;
    Vec3 v1 = sample;
    Vec3 min(-10, -10, -10);
    Vec3 max(10, 10, 10);

    //in range x
    v1.clamp(min, max);
    EXPECT_EQ(v1.x, sample.x);
    //min x
    v1 = sample;
    min.x = 5;
    v1.clamp(min, max);
    EXPECT_EQ(v1.x, min.x);
    min.x = -10;
    //max x
    v1 = sample;
    max.x = 3;
    v1.clamp(min, max);
    EXPECT_EQ(v1.x, max.x);
    max.x = 10;

    //in range y
    v1.clamp(min, max);
    EXPECT_EQ(v1.y, sample.y);
    //min y
    v1 = sample;
    min.y = 5;
    v1.clamp(min, max);
    EXPECT_EQ(v1.y, min.y);
    min.y = -10;
    //max y
    v1 = sample;
    max.y = -6;
    v1.clamp(min, max);
    EXPECT_EQ(v1.y, max.y);
    max.y = 10;

    //in range z
    v1.clamp(min, max);
    EXPECT_EQ(v1.z, sample.z);
    //min z
    v1 = sample;
    min.z = 5;
    v1.clamp(min, max);
    EXPECT_EQ(v1.z, min.z);
    min.z = -10;
    //max z
    v1 = sample;
    max.z = 3;
    v1.clamp(min, max);
    EXPECT_EQ(v1.z, max.z);
    max.z = 10;
}

SPECTRE_TEST(Vec3, saturate_noinline)
{
    Vec3 sample(0.9, 0.5, 0.3);;
    Vec3 v1 = sample;

    //in range x
    v1.saturate();
    EXPECT_EQ(v1.x, sample.x);
    //min x
    v1 = sample;
    v1.x = -0.1;
    v1.saturate();
    EXPECT_EQ(v1.x, 0.f);
    //max x
    v1 = sample;
    v1.x = 1.5;
    v1.saturate();
    EXPECT_EQ(v1.x, 1.f);

    //in range y
    v1.saturate();
    EXPECT_EQ(v1.y, sample.y);
    //min y
    v1 = sample;
    v1.y = -0.1;
    v1.saturate();
    EXPECT_EQ(v1.y, 0.f);
    //max y
    v1 = sample;
    v1.y = 1.5;
    v1.saturate();
    EXPECT_EQ(v1.y, 1.f);

    //in range z
    v1.saturate();
    EXPECT_EQ(v1.z, sample.z);
    //min z
    v1 = sample;
    v1.z = -0.1;
    v1.saturate();
    EXPECT_EQ(v1.z, 0.f);
    //max z
    v1 = sample;
    v1.z = 1.5;
    v1.saturate();
    EXPECT_EQ(v1.z, 1.f);
}

SPECTRE_TEST(Vec3, distance_noinline)
{
    Vec3 a(1.5f, -2.f, 1.7f);
    Vec3 b(3.7f, 2.5f, -4.6f);
    float dist = a.distance(b);
    EXPECT_EQ(8.0486023631f, dist);
}

SPECTRE_TEST(Vec3, max_noinline)
{
    Vec3 sample(0.5, 1.5, -3.5);
    Vec3 value;
    Vec3 compare;

    //x is max
    value = sample;
    compare = Vec3(0.2f, 0.f, 0.f);
    value.max(compare);
    EXPECT_EQ(value.x, sample.x);
    //x is not max
    value = sample;
    compare = Vec3(1.0, 0.f, 0.f);
    value.max(compare);
    EXPECT_EQ(value.x, compare.x);

    //y is max
    value = sample;
    compare = Vec3(0.f, 1.f, 0.f);
    value.max(compare);
    EXPECT_EQ(value.y, sample.y);
    //y is not max
    value = sample;
    compare = Vec3(0.f, 10.f, 0.f);
    value.max(compare);
    EXPECT_EQ(value.y, compare.y);

    //z is max
    value = sample;
    compare = Vec3(0.f, 0.f, -5.f);
    value.max(compare);
    EXPECT_EQ(value.z, sample.z);
    //z is not max
    value = sample;
    compare = Vec3(0.f, 0.f, 0.f);
    value.max(compare);
    EXPECT_EQ(value.z, compare.z);
}

SPECTRE_TEST(Vec3, min_noinline)
{
    Vec3 sample(0.5, 1.5, -3.5);
    Vec3 value;
    Vec3 compare;

    //x is max
    value = sample;
    compare = Vec3(0.2f, 0.f, 0.f);
    value.min(compare);
    EXPECT_EQ(value.x, compare.x);
    //x is not max
    value = sample;
    compare = Vec3(1.0, 0.f, 0.f);
    value.min(compare);
    EXPECT_EQ(value.x, sample.x);

    //y is max
    value = sample;
    compare = Vec3(0.f, 1.f, 0.f);
    value.min(compare);
    EXPECT_EQ(value.y, compare.y);
    //y is not max
    value = sample;
    compare = Vec3(0.f, 10.f, 0.f);
    value.min(compare);
    EXPECT_EQ(value.y, sample.y);

    //z is max
    value = sample;
    compare = Vec3(0.f, 0.f, -5.f);
    value.min(compare);
    EXPECT_EQ(value.z, compare.z);
    //z is not max
    value = sample;
    compare = Vec3(0.f, 0.f, 0.f);
    value.min(compare);
    EXPECT_EQ(value.z, sample.z);
}

SPECTRE_TEST(Vec3, reflect_vector_noinline)
{
    errors_count[WARNING_INDEX] = 0;
    Vec3 sample = Vec3(0.5f, 0.3f, -0.5f);
    Vec3 n(0.f, 0.f, 1.f);
    Vec3 v = sample;
    v.reflect(n);
    EXPECT_EQ(v.x, sample.x);
    EXPECT_EQ(v.y, sample.y);
    EXPECT_EQ(v.z, -sample.z);
    EXPECT_EQ(errors_count[WARNING_INDEX], 0);
    errors_count[WARNING_INDEX] = 0;

    n = Vec3(1, 1, 1);
    v.reflect(n);
    EXPECT_EQ(errors_count[WARNING_INDEX], 1);
    errors_count[WARNING_INDEX] = 0;
}

SPECTRE_TEST(Vec3, reflect_normal_noinline)
{
    errors_count[WARNING_INDEX] = 0;
    Vec3 sample = Vec3(0.5f, 0.3f, -0.5f);
    Normal n(0.f, 0.f, 1.f);
    Vec3 v = sample;
    v.reflect(n);
    EXPECT_EQ(v.x, sample.x);
    EXPECT_EQ(v.y, sample.y);
    EXPECT_EQ(v.z, -sample.z);
    EXPECT_EQ(errors_count[WARNING_INDEX], 0);
    errors_count[WARNING_INDEX] = 0;

    n = Normal(1, 1, 1);
    v.reflect(n);
    EXPECT_EQ(errors_count[WARNING_INDEX], 1);
    errors_count[WARNING_INDEX] = 0;
}

SPECTRE_TEST(Vec3, refract_vector_noinline)
{
    Vec3 v;
    Vec3 n(0.f, 0.f, 1.f);
    float eta = 1.45f;

    //Total Internal Reflection
    v = Vec3(0.5f, 0.3f, -0.5f);
    bool no_tir = v.refract(n, eta);
    EXPECT_FALSE(no_tir);

    v = Vec3(0.3f, 0.1f, 0.8f);
    v.refract(n, eta);
    EXPECT_TRUE(flt_equal(v.x, -0.435));
    EXPECT_TRUE(flt_equal(v.y, -0.145));
    EXPECT_TRUE(flt_equal(v.z, -0.493051767f));

    n = Vec3(1, 1, 1);
    v.refract(n, eta);
    EXPECT_EQ(errors_count[WARNING_INDEX], 1);
    errors_count[WARNING_INDEX] = 0;
}

SPECTRE_TEST(Vec3, refract_normal_noinline)
{
    Vec3 v;
    Normal n(0.f, 0.f, 1.f);
    float eta = 1.45f;

    //Total Internal Reflection
    v = Vec3(0.5f, 0.3f, -0.5f);
    bool no_tir = v.refract(n, eta);
    EXPECT_FALSE(no_tir);

    v = Vec3(0.3f, 0.1f, 0.8f);
    v.refract(n, eta);
    EXPECT_TRUE(flt_equal(v.x, -0.435));
    EXPECT_TRUE(flt_equal(v.y, -0.145));
    EXPECT_TRUE(flt_equal(v.z, -0.493051767f));

    n = Normal(1, 1, 1);
    v.refract(n, eta);
    EXPECT_EQ(errors_count[WARNING_INDEX], 1);
    errors_count[WARNING_INDEX] = 0;
}

// ------

SPECTRE_TEST(Vec3, dotvecvec_inline)
{
    Vec3 v(1, 2, 3);
    Vec3 v2(4, -5, 6);
    EXPECT_EQ(dot(v, v2), 12.f);
}

SPECTRE_TEST(Vec3, dotvecnormal_inline)
{
    Vec3 v(1, 2, 3);
    Normal v2(4, -5, 6);
    EXPECT_EQ(dot(v, v2), 12.f);
}

SPECTRE_TEST(Vec3, dotnormalvec_inline)
{
    Normal v(1, 2, 3);
    Vec3 v2(4, -5, 6);
    EXPECT_EQ(dot(v, v2), 12.f);
}

SPECTRE_TEST(Vec3, dotnormalnormal_inline)
{
    Normal v(1, 2, 3);
    Normal v2(4, -5, 6);
    EXPECT_EQ(dot(v, v2), 12.f);
}

SPECTRE_TEST(Vec3, absdotvecvec_inline)
{
    Vec3 v(1, 2, 3);
    Vec3 v2(4, -5, 6);
    EXPECT_EQ(absdot(v, v2), fabsf(dot(v, v2)));
}

SPECTRE_TEST(Vec3, absdotvecnormal_inline)
{
    Vec3 v(1, 2, 3);
    Normal v2(4, -5, 6);
    EXPECT_EQ(absdot(v, v2), fabsf(dot(v, v2)));
}

SPECTRE_TEST(Vec3, absdotnormalvec_inline)
{
    Normal v(1, 2, 3);
    Vec3 v2(4, -5, 6);
    EXPECT_EQ(absdot(v, v2), fabsf(dot(v, v2)));
}

SPECTRE_TEST(Vec3, absdotnormalnormal_inline)
{
    Normal v(1, 2, 3);
    Normal v2(4, -5, 6);
    EXPECT_EQ(absdot(v, v2), fabsf(dot(v, v2)));
}

SPECTRE_TEST(Vec3, cross_inline)
{
    Vec3 v0(3, -3, 1);
    Vec3 v1(4, 9, 2);
    Vec3 outv;

    outv = cross(v0, v1);

    EXPECT_EQ(outv.x, -15.f);
    EXPECT_EQ(outv.y, -2.f);
    EXPECT_EQ(outv.z, 39.f);
}

SPECTRE_TEST(Vec3, normalize_inline)
{
    errors_count[CRITICAL_INDEX] = 0;
    Vec3 v1(3, 1, 2);
    EXPECT_NEAR(v1.length(), 3.7416575f, 1e-5f);
    EXPECT_FALSE(v1.is_normalized());

    Vec3 v2 = normalize(v1);

    v1.normalize();
    EXPECT_EQ(v1.x, v2.x);
    EXPECT_EQ(v1.y, v2.y);
    EXPECT_EQ(v1.z, v2.z);
    EXPECT_TRUE(v1.is_normalized());
    EXPECT_TRUE(v2.is_normalized());

    Vec3 vzero;
    normalize(vzero);
    EXPECT_EQ(errors_count[CRITICAL_INDEX], 1);
    errors_count[CRITICAL_INDEX] = 0;

    //already normalized
    EXPECT_TRUE(v1.is_normalized());
    v2 = normalize(v1);
    EXPECT_TRUE(v2.is_normalized());
}

SPECTRE_TEST(Vec3, clamp_inline)
{
    Vec3 sample(4.9, -5.8, 3.6);;
    Vec3 v1 = sample;
    Vec3 min(-10, -10, -10);
    Vec3 max(10, 10, 10);
    Vec3 out;

    //in range x
    out = clamp(v1, min, max);
    EXPECT_EQ(out.x, sample.x);
    //min x
    v1 = sample;
    min.x = 5;
    out = clamp(v1, min, max);
    EXPECT_EQ(out.x, min.x);
    min.x = -10;
    //max x
    v1 = sample;
    max.x = 3;
    out = clamp(v1, min, max);
    EXPECT_EQ(out.x, max.x);
    max.x = 10;

    //in range y
    out = clamp(v1, min, max);
    EXPECT_EQ(out.y, sample.y);
    //min y
    v1 = sample;
    min.y = 5;
    out = clamp(v1, min, max);
    EXPECT_EQ(out.y, min.y);
    min.y = -10;
    //max y
    v1 = sample;
    max.y = -6;
    out = clamp(v1, min, max);
    EXPECT_EQ(out.y, max.y);
    max.y = 10;

    //in range z
    out = clamp(v1, min, max);
    EXPECT_EQ(out.z, sample.z);
    //min z
    v1 = sample;
    min.z = 5;
    out = clamp(v1, min, max);
    EXPECT_EQ(out.z, min.z);
    min.z = -10;
    //max z
    v1 = sample;
    max.z = 3;
    out = clamp(v1, min, max);
    EXPECT_EQ(out.z, max.z);
    max.z = 10;
}

SPECTRE_TEST(Vec3, saturate_inline)
{
    Vec3 sample(0.9, 0.5, 0.3);;
    Vec3 v1 = sample;
    Vec3 out;
    //in range x
    out = saturate(v1);;
    EXPECT_EQ(out.x, sample.x);
    //min x
    v1 = sample;
    v1.x = -0.1;
    out = saturate(v1);;
    EXPECT_EQ(out.x, 0.f);
    //max x
    v1 = sample;
    v1.x = 1.5;
    out = saturate(v1);;
    EXPECT_EQ(out.x, 1.f);

    //in range y
    out = saturate(v1);;
    EXPECT_EQ(out.y, sample.y);
    //min y
    v1 = sample;
    v1.y = -0.1;
    out = saturate(v1);;
    EXPECT_EQ(out.y, 0.f);
    //max y
    v1 = sample;
    v1.y = 1.5;
    out = saturate(v1);;
    EXPECT_EQ(out.y, 1.f);

    //in range z
    out = saturate(v1);;
    EXPECT_EQ(out.z, sample.z);
    //min z
    v1 = sample;
    v1.z = -0.1;
    out = saturate(v1);;
    EXPECT_EQ(out.z, 0.f);
    //max z
    v1 = sample;
    v1.z = 1.5;
    out = saturate(v1);;
    EXPECT_EQ(out.z, 1.f);
}

SPECTRE_TEST(Vec3, distance_inline)
{
    Vec3 a(1.5f, -2.f, 1.7f);
    Vec3 b(3.7f, 2.5f, -4.6f);
    float dist = distance(a, b);
    EXPECT_EQ(8.0486023631f, dist);
}

SPECTRE_TEST(Vec3, max_inline)
{
    Vec3 sample(0.5, 1.5, -3.5);
    Vec3 value;
    Vec3 compare;
    Vec3 out;

    //x is max
    value = sample;
    compare = Vec3(0.2f, 0.f, 0.f);
    out = max(value, compare);;
    EXPECT_EQ(out.x, sample.x);
    //x is not max
    value = sample;
    compare = Vec3(1.0, 0.f, 0.f);
    out = max(value, compare);;
    EXPECT_EQ(out.x, compare.x);

    //y is max
    value = sample;
    compare = Vec3(0.f, 1.f, 0.f);
    out = max(value, compare);;
    EXPECT_EQ(out.y, sample.y);
    //y is not max
    value = sample;
    compare = Vec3(0.f, 10.f, 0.f);
    out = max(value, compare);;
    EXPECT_EQ(out.y, compare.y);

    //z is max
    value = sample;
    compare = Vec3(0.f, 0.f, -5.f);
    out = max(value, compare);;
    EXPECT_EQ(out.z, sample.z);
    //z is not max
    value = sample;
    compare = Vec3(0.f, 0.f, 0.f);
    out = max(value, compare);;
    EXPECT_EQ(out.z, compare.z);
}

SPECTRE_TEST(Vec3, min_inline)
{
    Vec3 sample(0.5, 1.5, -3.5);
    Vec3 value;
    Vec3 compare;
    Vec3 out;

    //x is max
    value = sample;
    compare = Vec3(0.2f, 0.f, 0.f);
    out = min(value, compare);;
    EXPECT_EQ(out.x, compare.x);
    //x is not max
    value = sample;
    compare = Vec3(1.0, 0.f, 0.f);
    out = min(value, compare);;
    EXPECT_EQ(out.x, sample.x);

    //y is max
    value = sample;
    compare = Vec3(0.f, 1.f, 0.f);
    out = min(value, compare);;
    EXPECT_EQ(out.y, compare.y);
    //y is not max
    value = sample;
    compare = Vec3(0.f, 10.f, 0.f);
    out = min(value, compare);;
    EXPECT_EQ(out.y, sample.y);

    //z is max
    value = sample;
    compare = Vec3(0.f, 0.f, -5.f);
    out = min(value, compare);;
    EXPECT_EQ(out.z, compare.z);
    //z is not max
    value = sample;
    compare = Vec3(0.f, 0.f, 0.f);
    out = min(value, compare);;
    EXPECT_EQ(out.z, sample.z);
}

SPECTRE_TEST(Vec3, reflect_vector_inline)
{
    errors_count[WARNING_INDEX] = 0;
    Vec3 sample = Vec3(0.5f, 0.3f, -0.5f);
    Vec3 n(0.f, 0.f, 1.f);
    Vec3 v = sample;
    Vec3 reflected;
    reflected = reflect(v, n);
    EXPECT_EQ(reflected.x, sample.x);
    EXPECT_EQ(reflected.y, sample.y);
    EXPECT_EQ(reflected.z, -sample.z);
    EXPECT_EQ(errors_count[WARNING_INDEX], 0);
    errors_count[WARNING_INDEX] = 0;

    n = Vec3(1, 1, 1);
    reflected = reflect(v, n);
    EXPECT_EQ(errors_count[WARNING_INDEX], 1);
    errors_count[WARNING_INDEX] = 0;
}

SPECTRE_TEST(Vec3, reflect_normal_inline)
{
    errors_count[WARNING_INDEX] = 0;
    Vec3 sample = Vec3(0.5f, 0.3f, -0.5f);
    Normal n(0.f, 0.f, 1.f);
    Vec3 v = sample;
    Vec3 reflected;
    reflected = reflect(v, n);
    EXPECT_EQ(reflected.x, sample.x);
    EXPECT_EQ(reflected.y, sample.y);
    EXPECT_EQ(reflected.z, -sample.z);
    EXPECT_EQ(errors_count[WARNING_INDEX], 0);
    errors_count[WARNING_INDEX] = 0;

    n = Normal(1, 1, 1);
    reflected = reflect(v, n);
    EXPECT_EQ(errors_count[WARNING_INDEX], 1);
    errors_count[WARNING_INDEX] = 0;
}

SPECTRE_TEST(Vec3, refract_vector_inline)
{
    Vec3 v;
    Vec3 n(0.f, 0.f, 1.f);
    Vec3 refracted;
    float eta = 1.45f;

    //Total Internal Reflection
    v = Vec3(0.5f, 0.3f, -0.5f);
    refract(v, n, eta);
    EXPECT_EQ(refracted.x, 0.f);
    EXPECT_EQ(refracted.y, 0.f);
    EXPECT_EQ(refracted.z, 0.f);

    v = Vec3(0.3f, 0.1f, 0.8f);
    refracted = refract(v, n, eta);
    EXPECT_TRUE(flt_equal(refracted.x, -0.435));
    EXPECT_TRUE(flt_equal(refracted.y, -0.145));
    EXPECT_TRUE(flt_equal(refracted.z, -0.493051767f));

    n = Vec3(1, 1, 1);
    refracted = refract(v, n, eta);
    EXPECT_EQ(errors_count[WARNING_INDEX], 1);
    errors_count[WARNING_INDEX] = 0;
}

SPECTRE_TEST(Vec3, refract_normal_inline)
{
    Vec3 v;
    Normal n(0.f, 0.f, 1.f);
    Vec3 refracted;
    float eta = 1.45f;

    //Total Internal Reflection
    v = Vec3(0.5f, 0.3f, -0.5f);
    refracted = refract(v, n, eta);
    EXPECT_EQ(refracted.x, 0.f);
    EXPECT_EQ(refracted.y, 0.f);
    EXPECT_EQ(refracted.z, 0.f);

    v = Vec3(0.3f, 0.1f, 0.8f);
    refracted = refract(v, n, eta);
    EXPECT_TRUE(flt_equal(refracted.x, -0.435));
    EXPECT_TRUE(flt_equal(refracted.y, -0.145));
    EXPECT_TRUE(flt_equal(refracted.z, -0.493051767f));

    n = Normal(1, 1, 1);
    refracted = refract(v, n, eta);
    EXPECT_EQ(errors_count[WARNING_INDEX], 1);
    errors_count[WARNING_INDEX] = 0;
}


SPECTRE_TEST_END(Vec3_tests)
