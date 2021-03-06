#include "spectre_tests.h"

#ifdef __XCODE__
#import <XCTest/XCTest.h>
#elif defined(__VS__)
#include "CppUnitTest.h"
#else

#include <gtest/gtest.h>

#endif

#include "primitives/sphere.hpp"
#include "utility/utility.hpp"
#include <climits>

SPECTRE_TEST_INIT(Sphere_tests)

SPECTRE_TEST(Sphere, AABB_object_space)
{
    Sphere sphere;
    AABB box = sphere.compute_AABB();
    EXPECT_EQ(box.bounds[0].x, -1.f);
    EXPECT_EQ(box.bounds[0].y, -1.f);
    EXPECT_EQ(box.bounds[0].z, -1.f);
    EXPECT_EQ(box.bounds[1].x, 1.f);
    EXPECT_EQ(box.bounds[1].y, 1.f);
    EXPECT_EQ(box.bounds[1].z, 1.f);
}

SPECTRE_TEST(Sphere, AABB_world_space)
{
    //scale
    Sphere sphere;
    Matrix4 transform;
    transform.set_scale(3.f);
    AABB box = sphere.compute_AABB(&transform);
    EXPECT_EQ(box.bounds[0].x, -3.f);
    EXPECT_EQ(box.bounds[0].y, -3.f);
    EXPECT_EQ(box.bounds[0].z, -3.f);
    EXPECT_EQ(box.bounds[1].x, 3.f);
    EXPECT_EQ(box.bounds[1].y, 3.f);
    EXPECT_EQ(box.bounds[1].z, 3.f);

    //translation
    transform.set_translation(Vec3(-1, 10, 3.5));
    box = sphere.compute_AABB(&transform);
    EXPECT_EQ(box.bounds[0].x, -2.f);
    EXPECT_EQ(box.bounds[0].y, 9.f);
    EXPECT_EQ(box.bounds[0].z, 2.5f);
    EXPECT_EQ(box.bounds[1].x, 0.f);
    EXPECT_EQ(box.bounds[1].y, 11.f);
    EXPECT_EQ(box.bounds[1].z, 4.5f);

    //rotation
    transform.set_rotate_z(ONE_PI/2.f);
    box = sphere.compute_AABB(&transform);
    EXPECT_EQ(box.bounds[0].x, -1.f);
    EXPECT_EQ(box.bounds[0].y, -1.f);
    EXPECT_EQ(box.bounds[0].z, -1.f);
    EXPECT_EQ(box.bounds[1].x, 1.f);
    EXPECT_EQ(box.bounds[1].y, 1.f);
    EXPECT_EQ(box.bounds[1].z, 1.f);

    //null transform
    errors_count[ERROR_INDEX] = 0;
    box = sphere.compute_AABB(NULL);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
}

SPECTRE_TEST(Sphere, intersect)
{
    Sphere s;
    bool res;
    float distance;
    Ray r;
    HitPoint h;
    MaskBoolean mask;

    //distance < 0 This should NEVER happen and must be catched by kd-trees
    r = Ray(Point3(0, -10, 0), Vec3(0, 1, 0));
    distance = -1;
    errors_count[ERROR_INDEX] = 0;
    res = s.intersect(&r, &distance, &h, &mask);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;

    /*-----------------------------------------------------------------------.
    | I don't care about testing tangent hit. either the precision will      |
    | grant me at least 1 solution or 0. If I get two solutions instead of   |
    | one due to precision error this won't be a problem since the method    |
    | will always keep the closest                                           |
    `-----------------------------------------------------------------------*/


    //hit origin before the sphere
    r = Ray(Point3(0, -10, 0), Vec3(0, 1, 0));
    distance = FLT_MAX;
    res = s.intersect(&r, &distance, &h, &mask);
    ASSERT_TRUE(res);
    EXPECT_EQ(distance, 9.f);
    EXPECT_EQ(h.point_h.x, 0.f);
    EXPECT_EQ(h.point_h.y, -1.f);
    EXPECT_EQ(h.point_h.z, 0.f);
    EXPECT_EQ(h.normal_h.x, 0.f);
    EXPECT_EQ(h.normal_h.y, -1.f);
    EXPECT_EQ(h.normal_h.z, 0.f);
    h.dpdu.normalize();
    EXPECT_EQ(h.dpdu.x, 1.f);
    EXPECT_EQ(h.dpdu.y, 0.f);
    EXPECT_EQ(h.dpdu.z, 0.f);
    EXPECT_EQ(h.uv.x, 0.75f);
    EXPECT_EQ(h.uv.y, 0.5f);

    //hit origin inside the sphere
    r = Ray(Point3(0, 0, 0), Vec3(1, 0, 0));
    distance = FLT_MAX;
    res = s.intersect(&r, &distance, &h, &mask);
    ASSERT_TRUE(res);
    EXPECT_EQ(distance, 1.f);
    EXPECT_EQ(h.point_h.x, 1.f);
    EXPECT_EQ(h.point_h.y, 0.f);
    EXPECT_EQ(h.point_h.z, 0.f);
    EXPECT_EQ(h.normal_h.x, 1.f);
    EXPECT_EQ(h.normal_h.y, 0.f);
    EXPECT_EQ(h.normal_h.z, 0.f);
    h.dpdu.normalize();
    EXPECT_EQ(h.dpdu.x, 0.f);
    EXPECT_EQ(h.dpdu.y, 1.f);
    EXPECT_EQ(h.dpdu.z, 0.f);
    EXPECT_EQ(h.uv.x, 0.f);
    EXPECT_EQ(h.uv.y, 0.5f);

    //hit but origin after the sphere (= miss)
    r = Ray(Point3(0, 10, 0), Vec3(0, 1, 0));
    distance = FLT_MAX;
    res = s.intersect(&r, &distance, &h, &mask);
    ASSERT_FALSE(res);

    //hit in the exact vertical axis of the sphere
    r = Ray(Point3(0, 0, 0), Vec3(0, 0, 1));
    distance = FLT_MAX;
    res = s.intersect(&r, &distance, &h, &mask);
    ASSERT_TRUE(res);
    EXPECT_EQ(distance, 1.f);
    EXPECT_EQ(h.point_h.x, SELF_INTERSECT_ERROR);
    EXPECT_EQ(h.point_h.y, 0.f);
    EXPECT_EQ(h.point_h.z, 1.f);
    EXPECT_EQ(h.normal_h.x, 0.f);
    EXPECT_EQ(h.normal_h.y, 0.f);
    EXPECT_EQ(h.normal_h.z, 1.f);
    h.dpdu.normalize();
    EXPECT_EQ(h.dpdu.x, 0.f);
    EXPECT_EQ(h.dpdu.y, 1.f);
    EXPECT_EQ(h.dpdu.z, 0.f);
    EXPECT_EQ(h.uv.x, 0.f);
    EXPECT_EQ(h.uv.y, 1.f);
    //complete miss
    r = Ray(Point3(-2, -2, -10), Vec3(0, 0, 1));
    distance = FLT_MAX;
    res = s.intersect(&r, &distance, &h, &mask);
    ASSERT_FALSE(res);

    //better solution already found for origin outside
    r = Ray(Point3(0, -10, 0), Vec3(0, 1, 0));
    distance = 8;
    res = s.intersect(&r, &distance, &h, &mask);
    ASSERT_FALSE(res);

    //better solution already found for origin inside
    r = Ray(Point3(0, 0, 0), Vec3(0, 1, 0));
    distance = 0.5f;
    res = s.intersect(&r, &distance, &h, &mask);
    ASSERT_FALSE(res);
}

SPECTRE_TEST(Sphere, masked_hit)
{
    Sphere s;
    bool res;
    float distance;
    Ray r;
    HitPoint h;
    Vec2 shift(0.f);
    Vec2 scale(1.f);
    TextureImage tx(TEST_ASSETS "images/correct.bmp", shift, scale, UNFILTERED);
    MaskBoolean mask(&tx, RED, false);

    //point invalidated but second one is ok (with fixed point_h)
    distance = INFINITY;
    r = Ray(Point3(0.f, 0.f, 10.f), Vec3(0, 0, -1));
    res = s.intersect(&r, &distance, &h, &mask);
    EXPECT_TRUE(res);

    //same as before but a closer solution was found
    distance = 1e-5f;
    res = s.intersect(&r, &distance, &h, &mask);
    EXPECT_FALSE(res);

    //both points invalidated
    distance = INFINITY;
    tx = TextureImage(TEST_ASSETS "images/black.bmp", shift, scale, UNFILTERED);
    mask = MaskBoolean(&tx, RED, false);
    r = Ray(Point3(0.f, -10.f, 0.f), Vec3(0, 1, 0));
    res = s.intersect(&r, &distance, &h, &mask);
    EXPECT_FALSE(res);

    //starting from inside, only viable point invalidated
    distance = INFINITY;
    tx = TextureImage(TEST_ASSETS "images/black.bmp", shift, scale, UNFILTERED);
    mask = MaskBoolean(&tx, RED, false);
    r = Ray(Point3(0.f, 0.f, 0.f), Vec3(0, 1, 0));
    res = s.intersect(&r, &distance, &h, &mask);
    EXPECT_FALSE(res);
}

SPECTRE_TEST(Sphere, surface_object)
{
    Sphere s;
    //4*pi*r*r where r = 1
    EXPECT_EQ(s.surface(), FOUR_PI);
}

SPECTRE_TEST(Sphere, surface_world)
{
    Sphere s;
    Matrix4 m;
    m.set_scale(2.5f);
    EXPECT_NEAR(s.surface(&m), FOUR_PI*2.5f*2.5f, 1e-5f);
}

SPECTRE_TEST(Sphere, get_random_point)
{
    //just checking if both hemisphere are referenced, not a test to assert
    //if the values are uniformly distribuited
    Point3 p;
    Normal n;
    Sphere s;

    //upper hemisphere
    for(float i = 0.05f; i<0.5f; i += 0.05f)
    {
        s.sample_point(i, 0.05f, NULL, &p, &n);
        EXPECT_GT(p.z, 0);
    }

    //lower hemisphere
    for(float i = 0.55f; i<1.f; i += 0.05f)
    {
        s.sample_point(i, 0.05f, NULL, &p, &n);
        EXPECT_LT(p.z, 0);
    }

    //corner cases
    //r0 less than 0
    s.sample_point(0.f-FLT_EPSILON, 0.0f, NULL, &p, &n);
    EXPECT_EQ(p.x, 0.f);
    EXPECT_EQ(p.y, 0.f);
    EXPECT_NEAR(p.z, 1.f, 1e-5f);
    EXPECT_EQ(n.x, 0.f);
    EXPECT_EQ(n.y, 0.f);
    EXPECT_NEAR(n.z, 1.f, 1e-5f);

    //r0 greater than 1
    s.sample_point(1.f+FLT_EPSILON, 0.0f, NULL, &p, &n);
    EXPECT_EQ(p.x, 0.f);
    EXPECT_EQ(p.y, 0.f);
    EXPECT_NEAR(p.z, -1.f, 1e-5f);
    EXPECT_EQ(n.x, 0.f);
    EXPECT_EQ(n.y, 0.f);
    EXPECT_NEAR(n.z, -1.f, 1e-5f);

    //r1 less than 0
    s.sample_point(0.f, 0.f-FLT_EPSILON, NULL, &p, &n);
    EXPECT_EQ(p.x, 0.f);
    EXPECT_EQ(p.y, 0.f);
    EXPECT_NEAR(p.z, 1.f, 1e-5f);
    EXPECT_EQ(n.x, 0.f);
    EXPECT_EQ(n.y, 0.f);
    EXPECT_NEAR(n.z, 1.f, 1e-5f);

    //r1 greater than 0
    s.sample_point(0.f, 0.f+FLT_EPSILON, NULL, &p, &n);
    EXPECT_EQ(p.x, 0.f);
    EXPECT_EQ(p.y, 0.f);
    EXPECT_NEAR(p.z, 1.f, 1e-5f);
    EXPECT_EQ(n.x, 0.f);
    EXPECT_EQ(n.y, 0.f);
    EXPECT_NEAR(n.z, 1.f, 1e-5f);
}

SPECTRE_TEST(Sphere, inherited_methods)
{
    Sphere s;
    EXPECT_GT(s.get_id(), 0); //depends on the tests orderq
    EXPECT_EQ(s.get_faces_number(), 1);
    s.get_densities_array(NULL, NULL);
}

SPECTRE_TEST_END(Sphere_tests)
