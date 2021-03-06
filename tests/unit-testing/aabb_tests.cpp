#include "spectre_tests.h"

#ifdef __XCODE__
#import <XCTest/XCTest.h>
#elif defined(__VS__)
#include "CppUnitTest.h"
#else
#include <gtest/gtest.h>
#endif

#include "geometry/AABB.hpp"
#include "utility/utility.hpp"
#include <climits>
#include <cmath>

SPECTRE_TEST_INIT(AABB_tests)

SPECTRE_TEST(AABB, default_constructor)
{
    AABB box;
    EXPECT_TRUE(std::isinf(box.bounds[0].x));
    EXPECT_TRUE(std::isinf(box.bounds[0].y));
    EXPECT_TRUE(std::isinf(box.bounds[0].z));
    EXPECT_TRUE(std::isinf(box.bounds[1].x));
    EXPECT_TRUE(std::isinf(box.bounds[1].y));
    EXPECT_TRUE(std::isinf(box.bounds[1].z));
}

SPECTRE_TEST(AABB, single_point_constructor)
{
    Point3 p(76.9907f, -31.02559f, 64.63251f);
    AABB box(p);
    EXPECT_EQ(box.bounds[0].x, p.x);
    EXPECT_EQ(box.bounds[0].y, p.y);
    EXPECT_EQ(box.bounds[0].z, p.z);
    EXPECT_EQ(box.bounds[1].x, p.x);
    EXPECT_EQ(box.bounds[1].y, p.y);
    EXPECT_EQ(box.bounds[1].z, p.z);
}

SPECTRE_TEST(AABB, double_point_constructor_reference)
{
    Point3 pmin(-60.06347f, 33.59238f, -37.23738f);
    Point3 pmax(-21.18293f, 50.33405f, 9.33384f);
    AABB box(pmin, pmax);
    EXPECT_EQ(box.bounds[0].x, pmin.x);
    EXPECT_EQ(box.bounds[0].y, pmin.y);
    EXPECT_EQ(box.bounds[0].z, pmin.z);
    EXPECT_EQ(box.bounds[1].x, pmax.x);
    EXPECT_EQ(box.bounds[1].y, pmax.y);
    EXPECT_EQ(box.bounds[1].z, pmax.z);

    //assert that the min-max are not respected, otherwise I have to change
    //the doc
    pmin = Point3(-60.06347f, 33.59238f, -37.23738f);
    pmax = Point3(-21.18293f, -50.33405f, 9.33384f);
    box = AABB(&pmin, &pmax);
    EXPECT_EQ(box.bounds[0].x, pmin.x);
    EXPECT_EQ(box.bounds[0].y, pmin.y);
    EXPECT_EQ(box.bounds[0].z, pmin.z);
    EXPECT_EQ(box.bounds[1].x, pmax.x);
    EXPECT_EQ(box.bounds[1].y, pmax.y);
    EXPECT_EQ(box.bounds[1].z, pmax.z);
}

SPECTRE_TEST(AABB, double_point_constructor_pointer)
{
    Point3 pmin(-60.06347f, 33.59238f, -37.23738f);
    Point3 pmax(-21.18293f, 50.33405f, 9.33384f);
    AABB box(pmin, pmax);
    EXPECT_EQ(box.bounds[0].x, pmin.x);
    EXPECT_EQ(box.bounds[0].y, pmin.y);
    EXPECT_EQ(box.bounds[0].z, pmin.z);
    EXPECT_EQ(box.bounds[1].x, pmax.x);
    EXPECT_EQ(box.bounds[1].y, pmax.y);
    EXPECT_EQ(box.bounds[1].z, pmax.z);

    //assert that the min-max are not respected, otherwise I have to change
    //the doc
    pmin = Point3(-60.06347f, 33.59238f, -37.23738f);
    pmax = Point3(-21.18293f, -50.33405f, 9.33384f);
    box = AABB(&pmin, &pmax);
    EXPECT_EQ(box.bounds[0].x, pmin.x);
    EXPECT_EQ(box.bounds[0].y, pmin.y);
    EXPECT_EQ(box.bounds[0].z, pmin.z);
    EXPECT_EQ(box.bounds[1].x, pmax.x);
    EXPECT_EQ(box.bounds[1].y, pmax.y);
    EXPECT_EQ(box.bounds[1].z, pmax.z);
}

SPECTRE_TEST(AABB, expand)
{
    //expand positive
    Point3 p;
    AABB box(p);
    box.expand(50.5f);
    EXPECT_NEAR(box.bounds[0].x, -50.5f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, -50.5f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, -50.5f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, 50.5f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, 50.5f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, 50.5f, 1e-5f);

    //expand 0
    box.expand(0.f);
    EXPECT_NEAR(box.bounds[0].x, -50.5f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, -50.5f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, -50.5f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, 50.5f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, 50.5f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, 50.5f, 1e-5f);

    //expand negative
    box.expand(-0.5f);
    EXPECT_NEAR(box.bounds[0].x, -50.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, -50.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, -50.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, 50.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, 50.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, 50.f, 1e-5f);

    //expand degenerate
    box.expand(-200.f);
    EXPECT_NEAR(box.bounds[0].x, 150.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, 150.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, 150.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, -150.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, -150.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, -150.f, 1e-5f);
}

SPECTRE_TEST(AABB, engulf_Point3_pointer)
{
    Point3 p;
    AABB box(p);
    Point3 addme(10, -15, 20);
    box.engulf(&addme);
    EXPECT_NEAR(box.bounds[0].x, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, -15.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, 10.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, 20.f, 1e-5f);

    //already inside
    addme = Point3(2, -3, 15);
    box.engulf(&addme);
    EXPECT_NEAR(box.bounds[0].x, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, -15.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, 10.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, 20.f, 1e-5f);

    //inf
    addme = Point3(INFINITY, 0, 0);
    box.engulf(&addme);
    EXPECT_NEAR(box.bounds[0].x, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, -15.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, 0.f, 1e-5f);
    EXPECT_EQ(box.bounds[1].x, INFINITY);
    EXPECT_NEAR(box.bounds[1].y, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, 20.f, 1e-5f);
}

SPECTRE_TEST(AABB, engulf_Point3_reference)
{
    Point3 p;
    AABB box(p);
    Point3 addme(10, -15, 20);
    box.engulf(addme);
    EXPECT_NEAR(box.bounds[0].x, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, -15.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, 10.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, 20.f, 1e-5f);

    //already inside
    addme = Point3(2, -3, 15);
    box.engulf(addme);
    EXPECT_NEAR(box.bounds[0].x, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, -15.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, 10.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, 20.f, 1e-5f);

    //inf
    addme = Point3(INFINITY, -INFINITY, 0);
    box.engulf(addme);
    EXPECT_NEAR(box.bounds[0].x, 0.f, 1e-5f);
    EXPECT_EQ(box.bounds[0].y, -INFINITY);
    EXPECT_NEAR(box.bounds[0].z, 0.f, 1e-5f);
    EXPECT_EQ(box.bounds[1].x, INFINITY);
    EXPECT_NEAR(box.bounds[1].y, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, 20.f, 1e-5f);
}

SPECTRE_TEST(AABB, engulf_aabb_pointer)
{
    Point3 p;
    AABB box(p);

    //default
    AABB def;
    box.engulf(&def);
    EXPECT_EQ(box.bounds[0].x, 0.f);
    EXPECT_EQ(box.bounds[0].y, 0.f);
    EXPECT_EQ(box.bounds[0].z, 0.f);
    EXPECT_EQ(box.bounds[1].x, 0.f);
    EXPECT_EQ(box.bounds[1].y, 0.f);
    EXPECT_EQ(box.bounds[1].z, 0.f);

    //outside
    AABB addme(Point3(-59.28244f, -3.01509f, -47.61078f),
               Point3(67.30925f, 53.29163f, 82.07844f));
    box.engulf(&addme);
    EXPECT_NEAR(box.bounds[0].x, addme.bounds[0].x, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, addme.bounds[0].y, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, addme.bounds[0].z, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, addme.bounds[1].x, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, addme.bounds[1].y, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, addme.bounds[1].z, 1e-5f);

    //already inside
    AABB addme2(Point3(-9.30374f, 8.49896f, -35.41399f),
                Point3(58.56126f, 18.59649f, 37.76507f));
    box.engulf(&addme2);
    EXPECT_NEAR(box.bounds[0].x, addme.bounds[0].x, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, addme.bounds[0].y, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, addme.bounds[0].z, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, addme.bounds[1].x, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, addme.bounds[1].y, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, addme.bounds[1].z, 1e-5f);

    //inf
    AABB addme3(Point3(-138.73003f, -73.22298f, INFINITY),
                Point3(-INFINITY, INFINITY, 53.70019f));
    box.engulf(&addme3);
    EXPECT_NEAR(box.bounds[0].x, addme3.bounds[0].x, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, addme3.bounds[0].y, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, addme.bounds[0].z, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, addme.bounds[1].x, 1e-5f);
    EXPECT_EQ(box.bounds[1].y, INFINITY);
    EXPECT_NEAR(box.bounds[1].z, addme.bounds[1].z, 1e-5f);
}

SPECTRE_TEST(AABB, engulf_aabb_reference)
{
    Point3 p;
    AABB box(p);

    //default
    AABB def;
    box.engulf(def);
    EXPECT_EQ(box.bounds[0].x, 0.f);
    EXPECT_EQ(box.bounds[0].y, 0.f);
    EXPECT_EQ(box.bounds[0].z, 0.f);
    EXPECT_EQ(box.bounds[1].x, 0.f);
    EXPECT_EQ(box.bounds[1].y, 0.f);
    EXPECT_EQ(box.bounds[1].z, 0.f);

    //outside
    AABB addme(Point3(-59.28244f, -3.01509f, -47.61078f),
               Point3(67.30925f, 53.29163f, 82.07844f));
    box.engulf(addme);
    EXPECT_NEAR(box.bounds[0].x, addme.bounds[0].x, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, addme.bounds[0].y, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, addme.bounds[0].z, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, addme.bounds[1].x, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, addme.bounds[1].y, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, addme.bounds[1].z, 1e-5f);

    //already inside
    AABB addme2(Point3(-9.30374f, 8.49896f, -35.41399f),
                Point3(58.56126f, 18.59649f, 37.76507f));
    box.engulf(addme2);
    EXPECT_NEAR(box.bounds[0].x, addme.bounds[0].x, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, addme.bounds[0].y, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, addme.bounds[0].z, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, addme.bounds[1].x, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, addme.bounds[1].y, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, addme.bounds[1].z, 1e-5f);

    //inf
    AABB addme3(Point3(-138.73003f, -73.22298f, INFINITY),
                Point3(-INFINITY, INFINITY, 53.70019f));
    box.engulf(addme3);
    EXPECT_NEAR(box.bounds[0].x, addme3.bounds[0].x, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, addme3.bounds[0].y, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, addme.bounds[0].z, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, addme.bounds[1].x, 1e-5f);
    EXPECT_EQ(box.bounds[1].y, INFINITY);
    EXPECT_NEAR(box.bounds[1].z, addme.bounds[1].z, 1e-5f);
}

SPECTRE_TEST(AABB, inside)
{
    AABB box(Point3(-10, -10, -10), Point3(10, 10, 10));

    //point inside
    Point3 ins(-5, -5, -5);
    EXPECT_TRUE(box.inside(&ins));

    //point inside border
    Point3 border(10, 10, 10);
    EXPECT_TRUE(box.inside(&border));

    //point outside only x
    Point3 outx(-10.000001f, -5, -5);
    EXPECT_FALSE(box.inside(&outx));

    //point outside only y
    Point3 outy(-5, -10.000001f, -5);
    EXPECT_FALSE(box.inside(&outy));

    //point outside only z
    Point3 outz(-5, -5, -10.000001f);
    EXPECT_FALSE(box.inside(&outz));

    //point outside every point
    Point3 outall(11, 11, 11);
    EXPECT_FALSE(box.inside(&outall));

    //point outside infinite
    Point3 inf(INFINITY, 0, 0);
    Point3 minf(0, -INFINITY, 0);
    Point3 inf2(0, 0, INFINITY);
    EXPECT_FALSE(box.inside(&inf));
    EXPECT_FALSE(box.inside(&minf));
    EXPECT_FALSE(box.inside(&inf2));
}

SPECTRE_TEST(AABB, surface)
{
    AABB box;

    //zero length
    Point3 p(-0.53123f, -24.29362f, 84.26433f);
    box = AABB(p);
    EXPECT_EQ(box.surface(), 0.f);

    //normal length
    box = AABB(Point3(-1, -1, -1),
               Point3(3, 4, 5));
    EXPECT_EQ(box.surface(), 148.f);

    //infinite length
    box = AABB(Point3(-1, -1, -1), Point3(1, 1, INFINITY));
    EXPECT_EQ(box.surface(), INFINITY);
    box = AABB(Point3(-INFINITY, -INFINITY, -INFINITY),
               Point3(INFINITY, INFINITY, INFINITY));
    EXPECT_EQ(box.surface(), INFINITY);
}

SPECTRE_TEST(AABB, volume)
{
    AABB box;

    //zero length
    Point3 p(-0.53123f, -24.29362f, 84.26433f);
    box = AABB(p);
    EXPECT_EQ(box.volume(), 0.f);

    //normal length
    box = AABB(Point3(-1, -1, -1), Point3(3, 4, 5));
    EXPECT_EQ((int)box.volume(), 120);

    //infinite length
    box = AABB(Point3(-1, -1, -1), Point3(1, 1, INFINITY));
    EXPECT_EQ(box.volume(), INFINITY);
    box = AABB(Point3(-INFINITY, -INFINITY, -INFINITY),
               Point3(INFINITY, INFINITY, INFINITY));
    EXPECT_EQ(box.volume(), INFINITY);
}

SPECTRE_TEST(AABB, longest_axis)
{
    AABB box;

    //zero length -> return x
    Point3 p(-0.53123f, -24.29362f, 84.26433f);
    box = AABB(p);
    EXPECT_EQ(box.longest_axis(), (char)0);

    //longest x - non inf
    box = AABB(Point3(-85.77731f, 5.98468f, -10.75332f),
               Point3(74.13619f, 99.79995f, 37.72758f));
    EXPECT_EQ(box.longest_axis(), (char)0);

    //longest y - non inf
    box = AABB(Point3(-27.68684f, -73.58186f, -69.54105f),
               Point3(65.46841f, 95.43746f, -51.04507f));
    EXPECT_EQ(box.longest_axis(), (char)1);

    //longest z - non inf
    box = AABB(Point3(17.90233f, -6.71415f, -88.93419f),
               Point3(76.75507f, 50.73106f, 95.81359f));
    EXPECT_EQ(box.longest_axis(), (char)2);

    //longest x - inf
    box = AABB(Point3(-INFINITY, 5.98468f, -10.75332f),
               Point3(74.13619f, 99.79995f, 37.72758f));
    EXPECT_EQ(box.longest_axis(), (char)0);

    //longest y - inf
    box = AABB(Point3(-27.68684f, -73.58186f, -69.54105f),
               Point3(65.46841f, INFINITY, -51.04507f));
    EXPECT_EQ(box.longest_axis(), (char)1);

    //longest z - inf
    box = AABB(Point3(17.90233f, -46.71415f, -INFINITY),
               Point3(76.75507f, 90.73106f, 95.81359f));
    EXPECT_EQ(box.longest_axis(), (char)2);

    //everything infinite
    box = AABB(Point3(-INFINITY, -INFINITY, -INFINITY),
               Point3(INFINITY, INFINITY, INFINITY));
    EXPECT_EQ(box.longest_axis(), (char)0);
}

SPECTRE_TEST(AABB, center)
{
    AABB box;
    Point3 center;

    //zero length
    Point3 p(-0.53123f, -24.29362f, 84.26433f);
    box = AABB(p);
    center = box.center();
    EXPECT_NEAR(center.x, p.x, 1e-5f);
    EXPECT_NEAR(center.y, p.y, 1e-5f);
    EXPECT_NEAR(center.z, p.z, 1e-5f);

    //normal box
    box = AABB(Point3(-1, -1, -1), Point3(1, 1, 1));
    center = box.center();
    EXPECT_NEAR(center.x, 0.f, 1e-5f);
    EXPECT_NEAR(center.y, 0.f, 1e-5f);
    EXPECT_NEAR(center.z, 0.f, 1e-5f);

    //1 inf val box
    box = AABB(Point3(-1, -INFINITY, -1), Point3(1, -1, 1));
    center = box.center();
    EXPECT_NEAR(center.x, 0.f, 1e-5f);
    EXPECT_EQ(center.y, -INFINITY);
    EXPECT_NEAR(center.z, 0.f, 1e-5f);
    box = AABB(Point3(-1, -1, -1), Point3(1, 1, INFINITY));
    center = box.center();
    EXPECT_NEAR(center.x, 0.f, 1e-5f);
    EXPECT_NEAR(center.y, 0.f, 1e-5f);
    EXPECT_EQ(center.z, INFINITY);

    //infinite box
    box = AABB(Point3(-INFINITY, -INFINITY, -INFINITY),
               Point3(INFINITY, INFINITY, INFINITY));
    center = box.center();
    EXPECT_TRUE(std::isnan(center.x));
    EXPECT_TRUE(std::isnan(center.y));
    EXPECT_TRUE(std::isnan(center.z));
}

SPECTRE_TEST(AABB, sum_point)
{
    Point3 p;
    AABB box(p);
    Point3 addme(10, -15, 20);
    AABB res = box+addme;
    EXPECT_NEAR(res.bounds[0].x, 0.f, 1e-5f);
    EXPECT_NEAR(res.bounds[0].y, -15.f, 1e-5f);
    EXPECT_NEAR(res.bounds[0].z, 0.f, 1e-5f);
    EXPECT_NEAR(res.bounds[1].x, 10.f, 1e-5f);
    EXPECT_NEAR(res.bounds[1].y, 0.f, 1e-5f);
    EXPECT_NEAR(res.bounds[1].z, 20.f, 1e-5f);

    //already inside
    addme = Point3(2, -3, 15);
    res = res+addme;
    EXPECT_NEAR(res.bounds[0].x, 0.f, 1e-5f);
    EXPECT_NEAR(res.bounds[0].y, -15.f, 1e-5f);
    EXPECT_NEAR(res.bounds[0].z, 0.f, 1e-5f);
    EXPECT_NEAR(res.bounds[1].x, 10.f, 1e-5f);
    EXPECT_NEAR(res.bounds[1].y, 0.f, 1e-5f);
    EXPECT_NEAR(res.bounds[1].z, 20.f, 1e-5f);

    //inf
    addme = Point3(INFINITY, -INFINITY, 0);
    res = res+addme;
    EXPECT_NEAR(res.bounds[0].x, 0.f, 1e-5f);
    EXPECT_EQ(res.bounds[0].y, -INFINITY);
    EXPECT_NEAR(res.bounds[0].z, 0.f, 1e-5f);
    EXPECT_EQ(res.bounds[1].x, INFINITY);
    EXPECT_NEAR(res.bounds[1].y, 0.f, 1e-5f);
    EXPECT_NEAR(res.bounds[1].z, 20.f, 1e-5f);
}

SPECTRE_TEST(AABB, sum_point_this)
{
    Point3 p;
    AABB box(p);
    Point3 addme(10, -15, 20);
    box += addme;
    EXPECT_NEAR(box.bounds[0].x, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, -15.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, 10.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, 20.f, 1e-5f);

    //already inside
    addme = Point3(2, -3, 15);
    box += addme;
    EXPECT_NEAR(box.bounds[0].x, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, -15.f, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, 10.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, 20.f, 1e-5f);

    //inf
    addme = Point3(INFINITY, -INFINITY, 0);
    box += addme;
    EXPECT_NEAR(box.bounds[0].x, 0.f, 1e-5f);
    EXPECT_EQ(box.bounds[0].y, -INFINITY);
    EXPECT_NEAR(box.bounds[0].z, 0.f, 1e-5f);
    EXPECT_EQ(box.bounds[1].x, INFINITY);
    EXPECT_NEAR(box.bounds[1].y, 0.f, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, 20.f, 1e-5f);
}

SPECTRE_TEST(AABB, sum_aabb)
{
    Point3 p;
    AABB box(p);
    AABB res;

    //default
    AABB def;
    res = box+def;
    EXPECT_EQ(res.bounds[0].x, 0.f);
    EXPECT_EQ(res.bounds[0].y, 0.f);
    EXPECT_EQ(res.bounds[0].z, 0.f);
    EXPECT_EQ(res.bounds[1].x, 0.f);
    EXPECT_EQ(res.bounds[1].y, 0.f);
    EXPECT_EQ(res.bounds[1].z, 0.f);

    //outside
    AABB addme(Point3(-59.28244f, -3.01509f, -47.61078f),
               Point3(67.30925f, 53.29163f, 82.07844f));
    res = res+addme;
    EXPECT_NEAR(res.bounds[0].x, addme.bounds[0].x, 1e-5f);
    EXPECT_NEAR(res.bounds[0].y, addme.bounds[0].y, 1e-5f);
    EXPECT_NEAR(res.bounds[0].z, addme.bounds[0].z, 1e-5f);
    EXPECT_NEAR(res.bounds[1].x, addme.bounds[1].x, 1e-5f);
    EXPECT_NEAR(res.bounds[1].y, addme.bounds[1].y, 1e-5f);
    EXPECT_NEAR(res.bounds[1].z, addme.bounds[1].z, 1e-5f);

    //already inside
    AABB addme2(Point3(-9.30374f, 8.49896f, -35.41399f),
                Point3(58.56126f, 18.59649f, 37.76507f));
    res = res+addme2;
    EXPECT_NEAR(res.bounds[0].x, addme.bounds[0].x, 1e-5f);
    EXPECT_NEAR(res.bounds[0].y, addme.bounds[0].y, 1e-5f);
    EXPECT_NEAR(res.bounds[0].z, addme.bounds[0].z, 1e-5f);
    EXPECT_NEAR(res.bounds[1].x, addme.bounds[1].x, 1e-5f);
    EXPECT_NEAR(res.bounds[1].y, addme.bounds[1].y, 1e-5f);
    EXPECT_NEAR(res.bounds[1].z, addme.bounds[1].z, 1e-5f);

    //inf
    AABB addme3(Point3(-138.73003f, -73.22298f, INFINITY),
                Point3(-INFINITY, INFINITY, 53.70019f));
    res = res+addme3;
    EXPECT_NEAR(res.bounds[0].x, addme3.bounds[0].x, 1e-5f);
    EXPECT_NEAR(res.bounds[0].y, addme3.bounds[0].y, 1e-5f);
    EXPECT_NEAR(res.bounds[0].z, addme.bounds[0].z, 1e-5f);
    EXPECT_NEAR(res.bounds[1].x, addme.bounds[1].x, 1e-5f);
    EXPECT_EQ(res.bounds[1].y, INFINITY);
    EXPECT_NEAR(res.bounds[1].z, addme.bounds[1].z, 1e-5f);
}

SPECTRE_TEST(AABB, sum_aabb_this)
{
    Point3 p;
    AABB box(p);

    //default
    AABB def;
    box += def;
    EXPECT_EQ(box.bounds[0].x, 0.f);
    EXPECT_EQ(box.bounds[0].y, 0.f);
    EXPECT_EQ(box.bounds[0].z, 0.f);
    EXPECT_EQ(box.bounds[1].x, 0.f);
    EXPECT_EQ(box.bounds[1].y, 0.f);
    EXPECT_EQ(box.bounds[1].z, 0.f);

    //outside
    AABB addme(Point3(-59.28244f, -3.01509f, -47.61078f),
               Point3(67.30925f, 53.29163f, 82.07844f));
    box += addme;
    EXPECT_NEAR(box.bounds[0].x, addme.bounds[0].x, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, addme.bounds[0].y, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, addme.bounds[0].z, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, addme.bounds[1].x, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, addme.bounds[1].y, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, addme.bounds[1].z, 1e-5f);

    //already inside
    AABB addme2(Point3(-9.30374f, 8.49896f, -35.41399f),
                Point3(58.56126f, 18.59649f, 37.76507f));
    box += addme2;
    EXPECT_NEAR(box.bounds[0].x, addme.bounds[0].x, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, addme.bounds[0].y, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, addme.bounds[0].z, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, addme.bounds[1].x, 1e-5f);
    EXPECT_NEAR(box.bounds[1].y, addme.bounds[1].y, 1e-5f);
    EXPECT_NEAR(box.bounds[1].z, addme.bounds[1].z, 1e-5f);

    //inf
    AABB addme3(Point3(-138.73003f, -73.22298f, INFINITY),
                Point3(-INFINITY, INFINITY, 53.70019f));
    box += addme3;
    EXPECT_NEAR(box.bounds[0].x, addme3.bounds[0].x, 1e-5f);
    EXPECT_NEAR(box.bounds[0].y, addme3.bounds[0].y, 1e-5f);
    EXPECT_NEAR(box.bounds[0].z, addme.bounds[0].z, 1e-5f);
    EXPECT_NEAR(box.bounds[1].x, addme.bounds[1].x, 1e-5f);
    EXPECT_EQ(box.bounds[1].y, INFINITY);
    EXPECT_NEAR(box.bounds[1].z, addme.bounds[1].z, 1e-5f);
}

SPECTRE_TEST(AABB, intersect)
{
    //hit diagonally, dir positive
    AABB box(Point3(1, 1, 1), Point3(2, 2, 2));
    Ray ray(Point3(0, 0, 0), Vec3(1, 1, 1));
    float near, far;
    EXPECT_FALSE(box.inside(&(ray.origin)));
    bool res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit diagonally, dir negative
    ray = Ray(Point3(3, 3, 3), Vec3(-1, -1, -1));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit x, point left
    ray = Ray(Point3(0, 1.5f, 1.5f), Vec3(1, 0, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit x, point right
    ray = Ray(Point3(3, 1.5f, 1.5f), Vec3(-1, 0, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit y, point front
    ray = Ray(Point3(1.5f, 0, 1.5f), Vec3(0, 1, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit y, point back
    ray = Ray(Point3(1.5f, 3, 1.5f), Vec3(0, -1, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit z, point above
    ray = Ray(Point3(1.5f, 1.5f, 0), Vec3(0, 0, 1));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit z, point below
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(0, 0, -1));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit in line with the aabb, but wrong direction of the ray (x-)
    ray = Ray(Point3(0, 1.5f, 1.5f), Vec3(-1, 0, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_LT(far, 0);

    //hit in line with the aabb, but wrong direction of the ray (x+)
    ray = Ray(Point3(3, 1.5f, 1.5f), Vec3(1, 0, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_LT(far, 0);

    //hit in line with the aabb, but wrong direction of the ray (y-)
    ray = Ray(Point3(1.5f, 0, 1.5f), Vec3(0, -1, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_LT(far, 0);

    //hit in line with the aabb, but wrong direction of the ray (y+)
    ray = Ray(Point3(1.5f, 3, 1.5f), Vec3(0, 1, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_LT(far, 0);

    //hit in line with the aabb, but wrong direction of the ray (z-)
    ray = Ray(Point3(1.5f, 1.5f, 0), Vec3(0, 0, -1));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_LT(far, 0);

    //hit in line with the aabb, but wrong direction of the ray (z+)
    ray = Ray(Point3(1.5, 1.5, 3), Vec3(0, 0, 1));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_LT(far, 0);

    //complete miss, point back, x+ direction
    ray = Ray(Point3(1.5f, 3, 1.5f), Vec3(1, 0, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point back, x- direction
    ray = Ray(Point3(1.5f, 3, 1.5f), Vec3(-1, 0, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point below, x+ direction
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(1, 0, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point below, x- direction
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(-1, 0, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point front, x+ direction
    ray = Ray(Point3(1.5, 0, 1.5), Vec3(1, 0, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point front, x- direction
    ray = Ray(Point3(1.5f, 0, 1.5f), Vec3(-1, 0, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point above, x+ direction
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(1, 0, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point above, x- direction
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(-1, 0, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point left, y+ direction
    ray = Ray(Point3(0, 1.5f, 1.5f), Vec3(0, 1, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point left, y- direction
    ray = Ray(Point3(0, 1.5f, 1.5f), Vec3(0, -1, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point below, y+ direction
    ray = Ray(Point3(1.5f, 1.5f, 0), Vec3(0, 1, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point below, y- direction
    ray = Ray(Point3(1.5f, 1.5f, 0), Vec3(0, -1, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point right, y+ direction
    ray = Ray(Point3(3, 1.5f, 1.5f), Vec3(0, 1, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point right, y- direction
    ray = Ray(Point3(3, 1.5f, 1.5f), Vec3(0, -1, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point above, y+ direction
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(0, 1, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point above, y- direction
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(0, -1, 0));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point left, z+ direction
    ray = Ray(Point3(0, 1.5, 1.5), Vec3(0, 0, 1));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point left, z- direction
    ray = Ray(Point3(0, 1.5f, 1.5f), Vec3(0, 0, -1));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point back, z+ direction
    ray = Ray(Point3(1.5f, 3, 1.5f), Vec3(0, 0, 1));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point back, z- direction
    ray = Ray(Point3(1.5, 3, 1.5), Vec3(0, 0, -1));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point right, z+ direction
    ray = Ray(Point3(3, 1.5f, 1.5f), Vec3(0, 0, 1));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point right, z- direction
    ray = Ray(Point3(3, 1.5f, 1.5f), Vec3(0, 0, -1));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point front, z direction
    ray = Ray(Point3(1.5f, 0, 1.5f), Vec3(0, 0, 1));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point front, z- direction
    ray = Ray(Point3(1.5f, 0, 1.5f), Vec3(0, 0, -1));
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, false);

    //start inside, x+
    ray = Ray(Point3(1.5f, 1.5f, 1.5f), Vec3(1, 0, 0));
    EXPECT_TRUE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_GT(far, 0);

    //start inside, x-
    ray = Ray(Point3(1.5f, 1.5f, 1.5f), Vec3(-1, 0, 0));
    EXPECT_TRUE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_GT(far, 0);

    //start inside, y+
    ray = Ray(Point3(1.5f, 1.5f, 1.5f), Vec3(0, 1, 0));
    EXPECT_TRUE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_GT(far, 0);

    //start inside, y-
    ray = Ray(Point3(1.5f, 1.5f, 1.5f), Vec3(0, -1, 0));
    EXPECT_TRUE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_GT(far, 0);

    //start inside, z+
    ray = Ray(Point3(1.5f, 1.5f, 1.5f), Vec3(0, 0, 1));
    EXPECT_TRUE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_GT(far, 0);

    //start inside, z-
    ray = Ray(Point3(1.5f, 1.5f, 1.5f), Vec3(0, 0, -1));
    EXPECT_TRUE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_GT(far, 0);
}

SPECTRE_TEST(AABB, intersect_fast)
{
    //hit diagonally, dir positive
    AABB box(Point3(1, 1, 1), Point3(2, 2, 2));
    Ray ray(Point3(0, 0, 0), Vec3(1, 1, 1));
    RayProperties rp = RayProperties(ray);
    float near, far;
    EXPECT_FALSE(box.inside(&(ray.origin)));
    bool res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit diagonally, dir negative
    ray = Ray(Point3(3, 3, 3), Vec3(-1, -1, -1));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit x, point left
    ray = Ray(Point3(0, 1.5f, 1.5f), Vec3(1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit x, point right
    ray = Ray(Point3(3, 1.5f, 1.5f), Vec3(-1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit y, point front
    ray = Ray(Point3(1.5f, 0, 1.5f), Vec3(0, 1, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit y, point back
    ray = Ray(Point3(1.5f, 3, 1.5f), Vec3(0, -1, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit z, point above
    ray = Ray(Point3(1.5f, 1.5f, 0), Vec3(0, 0, 1));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit z, point below
    ray = Ray(Point3(1.5, 1.5, 3), Vec3(0, 0, -1));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_GT(near, 0);
    EXPECT_GT(far, 0);

    //hit in line with the aabb, but wrong direction of the ray (x-)
    ray = Ray(Point3(0, 1.5f, 1.5f), Vec3(-1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_LT(far, 0);

    //hit in line with the aabb, but wrong direction of the ray (x+)
    ray = Ray(Point3(3, 1.5f, 1.5f), Vec3(1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_LT(far, 0);

    //hit in line with the aabb, but wrong direction of the ray (y-)
    ray = Ray(Point3(1.5f, 0, 1.5f), Vec3(0, -1, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_LT(far, 0);

    //hit in line with the aabb, but wrong direction of the ray (y+)
    ray = Ray(Point3(1.5f, 3, 1.5f), Vec3(0, 1, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_LT(far, 0);

    //hit in line with the aabb, but wrong direction of the ray (z-)
    ray = Ray(Point3(1.5f, 1.5f, 0), Vec3(0, 0, -1));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_LT(far, 0);

    //hit in line with the aabb, but wrong direction of the ray (z+)
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(0, 0, 1));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_LT(far, 0);

    //complete miss, point back, x+ direction
    ray = Ray(Point3(1.5f, 3, 1.5f), Vec3(1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point front, x- direction
    ray = Ray(Point3(1.5f, 3, 1.5f), Vec3(-1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point above, x+ direction
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point above, x- direction
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(-1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point back, x+ direction
    ray = Ray(Point3(1.5f, 0, 1.5f), Vec3(1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point back, x- direction
    ray = Ray(Point3(1.5f, 0, 1.5f), Vec3(-1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point below, x+ direction
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point below, x- direction
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(-1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point left, y+ direction
    ray = Ray(Point3(0, 1.5f, 1.5f), Vec3(0, 1, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point left, y- direction
    ray = Ray(Point3(0, 1.5f, 1.5f), Vec3(0, -1, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point below, y+ direction
    ray = Ray(Point3(1.5f, 1.5f, 0), Vec3(0, 1, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point below, y- direction
    ray = Ray(Point3(1.5f, 1.5f, 0), Vec3(0, -1, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point right, y+ direction
    ray = Ray(Point3(3, 1.5f, 1.5f), Vec3(0, 1, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point right, y- direction
    ray = Ray(Point3(3, 1.5f, 1.5f), Vec3(0, -1, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point above, y+ direction
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(0, 1, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point above, y- direction
    ray = Ray(Point3(1.5f, 1.5f, 3), Vec3(0, -1, 0));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point left, z+ direction
    ray = Ray(Point3(0, 1.5f, 1.5f), Vec3(0, 0, 1));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point left, z- direction
    ray = Ray(Point3(0, 1.5f, 1.5f), Vec3(0, 0, -1));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point back, z+ direction
    ray = Ray(Point3(1.5f, 3, 1.5f), Vec3(0, 0, 1));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point back, z- direction
    ray = Ray(Point3(1.5f, 3, 1.5f), Vec3(0, 0, -1));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point right, z+ direction
    ray = Ray(Point3(3, 1.5f, 1.5f), Vec3(0, 0, 1));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point right, z- direction
    ray = Ray(Point3(3, 1.5f, 1.5f), Vec3(0, 0, -1));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point front, z direction
    ray = Ray(Point3(1.5f, 0, 1.5f), Vec3(0, 0, 1));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //complete miss, point front, z- direction
    ray = Ray(Point3(1.5f, 0, 1.5f), Vec3(0, 0, -1));
    rp = RayProperties(ray);
    EXPECT_FALSE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, false);

    //start inside, x+
    ray = Ray(Point3(1.5f, 1.5f, 1.5f), Vec3(1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_TRUE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_GT(far, 0);

    //start inside, x-
    ray = Ray(Point3(1.5f, 1.5f, 1.5f), Vec3(-1, 0, 0));
    rp = RayProperties(ray);
    EXPECT_TRUE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_GT(far, 0);

    //start inside, y+
    ray = Ray(Point3(1.5f, 1.5f, 1.5f), Vec3(0, 1, 0));
    rp = RayProperties(ray);
    EXPECT_TRUE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_GT(far, 0);

    //start inside, y-
    ray = Ray(Point3(1.5f, 1.5f, 1.5f), Vec3(0, -1, 0));
    rp = RayProperties(ray);
    EXPECT_TRUE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_GT(far, 0);

    //start inside, z+
    ray = Ray(Point3(1.5f, 1.5f, 1.5f), Vec3(0, 0, 1));
    rp = RayProperties(ray);
    EXPECT_TRUE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_GT(far, 0);

    //start inside, z-
    ray = Ray(Point3(1.5f, 1.5f, 1.5f), Vec3(0, 0, -1));
    rp = RayProperties(ray);
    EXPECT_TRUE(box.inside(&(ray.origin)));
    res = box.intersect(&ray, &rp, &near, &far);
    EXPECT_EQ(res, true);
    EXPECT_LT(near, 0);
    EXPECT_GT(far, 0);
}

SPECTRE_TEST(AABB, less)
{
    AABB small(Point3(-0.53123f, -0.29362f, -0.26433f),
               Point3(0.14842f, 0.6715f, 0.59818f));
    AABB big(Point3(-1, -1, -1), Point3(3, 4, 5));
    EXPECT_TRUE(small<big);
    EXPECT_FALSE(big<small);
    EXPECT_FALSE(small<small);
}

SPECTRE_TEST(AABB, greater)
{
    AABB small(Point3(-0.53123f, -0.29362f, -0.26433f),
               Point3(0.14842f, 0.6715f, 0.59818f));
    AABB big(Point3(-1, -1, -1), Point3(3, 4, 5));
    EXPECT_TRUE(big>small);
    EXPECT_FALSE(small>big);
    EXPECT_FALSE(big>big);
}

SPECTRE_TEST(AABB, less_equal)
{
    AABB small(Point3(-0.53123f, -0.29362f, -0.26433f),
               Point3(0.14842f, 0.6715f, 0.59818f));
    AABB big(Point3(-1, -1, -1), Point3(3, 4, 5));
    EXPECT_TRUE(small<big);
    EXPECT_FALSE(big<small);
    EXPECT_TRUE(small<=small);
    EXPECT_FALSE(big<=small);
}

SPECTRE_TEST(AABB, greater_equal)
{
    AABB small(Point3(-0.53123f, -0.29362f, -0.26433f),
               Point3(0.14842f, 0.6715f, 0.59818f));
    AABB big(Point3(-1, -1, -1), Point3(3, 4, 5));
    EXPECT_TRUE(big>small);
    EXPECT_FALSE(small>big);
    EXPECT_TRUE(big>=big);
    EXPECT_FALSE(small>=big);
}

SPECTRE_TEST(AABB, equal)
{
    AABB small(Point3(-1, 0, -2), Point3(1, 1, 1));
    AABB small_again(Point3(2, 2, 2), Point3(5, 4, 3));
    AABB big(Point3(-100, 0, -2), Point3(1, 1, 100));
    EXPECT_TRUE(small == small);
    EXPECT_TRUE(small == small_again);
    EXPECT_FALSE(small == big);
}

SPECTRE_TEST(AABB, not_equal)
{
    AABB small(Point3(-1, 0, -2), Point3(1, 1, 1));
    AABB small_again(Point3(2, 2, 2), Point3(5, 4, 3));
    AABB big(Point3(-100, 0, -2), Point3(1, 1, 100));
    EXPECT_FALSE(small != small);
    EXPECT_FALSE(small != small_again);
    EXPECT_TRUE(small != big);
}

SPECTRE_TEST_END(AABB_tests)
