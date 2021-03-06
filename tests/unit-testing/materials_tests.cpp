#include "spectre_tests.h"

#ifdef __XCODE__
#import <XCTest/XCTest.h>
#elif defined(__VS__)
#include "CppUnitTest.h"
#else

#include <gtest/gtest.h>

#endif

#include "materials/lambertian.hpp"
#include "materials/oren_nayar.hpp"
#include "materials/reflection.hpp"
#include "materials/refraction.hpp"
#include "materials/microfacet.hpp"
#include "materials/microfacet_distributions.hpp"
#include "materials/fresnel_conditions.hpp"
#include "materials/metals.hpp"
#include "materials/bdf.hpp"
#include "materials/multi_bsdf.hpp"
#include "materials/single_brdf.hpp"
#include "materials/dual.hpp"
#include "primitives/asset.hpp"
#include "primitives/shape.hpp"
#include "primitives/sphere.hpp"
#include "textures/texture_uniform.hpp"
#include "utility/spectrum.hpp"

SPECTRE_TEST_INIT(Material_tests)

SPECTRE_TEST(Material, Lambertian_flags)
{
    Lambertian mat;
    EXPECT_EQ(mat.get_flags(), (char)FLAG_BRDF);
    EXPECT_TRUE(mat.matches(FLAG_BRDF));
    EXPECT_FALSE(mat.matches(FLAG_BTDF));
    EXPECT_FALSE(mat.matches(FLAG_SPEC));
    EXPECT_TRUE(mat.matches(FLAG_BRDF | FLAG_BTDF));
    EXPECT_TRUE(mat.matches(FLAG_BRDF | FLAG_SPEC));
    EXPECT_TRUE(mat.matches(FLAG_BRDF | FLAG_BTDF | FLAG_SPEC));
    EXPECT_FALSE(mat.matches(FLAG_BTDF | FLAG_SPEC));

}

SPECTRE_TEST(Material, Lambertian_value)
{
    Lambertian mat;
    const Vec3 wo(-1.f, 0.f, .5f);
    const Vec3 wi(1.f, 0.f, .5f);
    Spectrum res = mat.value(&wo, &wi);
    EXPECT_NEAR(res.w[0], .318310f, 1e-5f);
    EXPECT_NEAR(res.w[1], .318310f, 1e-5f);
    EXPECT_NEAR(res.w[2], .318310f, 1e-5f);
}

SPECTRE_TEST(Material, Lambertian_sample_value)
{
    Lambertian mat;
    const Vec3 wo(-1.f, 0.f, .5f);
    Vec3 wi;
    float pdf;
    Spectrum res = mat.sample_value(&wo, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_NEAR(res.w[0], .318310f, 1e-5f);
    EXPECT_NEAR(res.w[1], .318310f, 1e-5f);
    EXPECT_NEAR(res.w[2], .318310f, 1e-5f);
    EXPECT_EQ(sign(wo.z), sign(wi.z));
    EXPECT_NEAR(pdf, 0.22507906f, 1e-5f);

    const Vec3 wo2(-1.f, 0.f, -0.5f);
    res = mat.sample_value(&wo2, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_NEAR(res.w[0], .318310f, 1e-5f);
    EXPECT_NEAR(res.w[1], .318310f, 1e-5f);
    EXPECT_NEAR(res.w[2], .318310f, 1e-5f);
    EXPECT_EQ(sign(wo2.z), sign(wi.z));
    EXPECT_NEAR(pdf, 0.22507906f, 1e-5f);
}

SPECTRE_TEST(Material, Lambertian_pdf)
{
    Lambertian mat;

    //same hemisphere
    const Vec3 wo(-1.f, 0.f, .5f);
    const Vec3 wi(-0.707106769f, -0.0000000618172393f, 0.707106769f);
    float pdf;
    pdf = mat.pdf(&wo, &wi);
    EXPECT_NEAR(pdf, 0.22507906f, 1e-5f);

    const Vec3 wo2(-1.f, 0.f, -0.5f);
    pdf = mat.pdf(&wo2, &wi);
    EXPECT_EQ(pdf, 0.f);
}

SPECTRE_TEST(Material, OrenNayar_flags)
{
    OrenNayar mat(15);
    EXPECT_EQ(mat.get_flags(), (char)FLAG_BRDF);
    EXPECT_TRUE(mat.matches(FLAG_BRDF));
    EXPECT_FALSE(mat.matches(FLAG_BTDF));
    EXPECT_FALSE(mat.matches(FLAG_SPEC));
    EXPECT_TRUE(mat.matches(FLAG_BRDF | FLAG_BTDF));
    EXPECT_TRUE(mat.matches(FLAG_BRDF | FLAG_SPEC));
    EXPECT_TRUE(mat.matches(FLAG_BRDF | FLAG_BTDF | FLAG_SPEC));
    EXPECT_FALSE(mat.matches(FLAG_BTDF | FLAG_SPEC));
}

SPECTRE_TEST(Material, OrenNayar_value)
{
    OrenNayar mat(15.f);

    Vec3 wo(-1.f, 0.f, .4f);
    Vec3 wi(1.f, 0.f, .6f);
    Spectrum res = mat.value(&wo, &wi);
    EXPECT_NEAR(res.w[0], 0.159388f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.159388f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.159388f, 1e-5f);

    wo = Vec3(-1.f, 0.f, 0.6f);
    wi = Vec3(1.f, 0.f, 0.4f);
    res = mat.value(&wo, &wi);
    EXPECT_NEAR(res.w[0], 0.159388f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.159388f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.159388f, 1e-5f);

    //assert different results by changing incident angles
    wo = Vec3(-.2f, 0.5f, 0.2f);
    wi = Vec3(0.3f, 0.3f, 0.3f);
    res = mat.value(&wo, &wi);
    EXPECT_NE(res.w[0], 0.0657406151f);
    EXPECT_NE(res.w[1], 0.0338975154f);
    EXPECT_NE(res.w[2], 0.0030815925f);

    wo = Vec3(-1.f, 0.f, 1.f);
    wi = Vec3(1.f, 0.f, 0.5f);
    res = mat.value(&wo, &wi);
    EXPECT_NEAR(res.w[0], 0.159388f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.159388f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.159388f, 1e-5f);

    wo = Vec3(-1.f, 0.f, 0.5f);
    wi = Vec3(1.f, 0.f, 1.f);
    res = mat.value(&wo, &wi);
    EXPECT_NEAR(res.w[0], 0.159388f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.159388f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.159388f, 1e-5f);
}

SPECTRE_TEST(Material, OrenNayar_sample_value)
{
    Spectrum white(ColorRGB(1.f, 1.f, 1.f));
    OrenNayar mat(15);
    const Vec3 wo(-1.f, 0.f, .5f);
    Vec3 wi;
    float pdf;
    Spectrum res = mat.sample_value(&wo, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_EQ(sign(wo.z), sign(wi.z));
    EXPECT_NEAR(res.w[0], 0.283387f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.283387f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.283387f, 1e-5f);
    EXPECT_NEAR(pdf, 0.22507906f, 1e-5f);

    const Vec3 wo2(-1.f, 0.f, -0.5f);
    res = mat.sample_value(&wo2, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_NEAR(res.w[0], 0.283387f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.283387f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.283387f, 1e-5f);
    EXPECT_EQ(sign(wo2.z), sign(wi.z));
    EXPECT_NEAR(pdf, 0.22507906f, 1e-5f);
}

SPECTRE_TEST(Material, OrenNayar_pdf)
{
    Spectrum white(ColorRGB(1.f, 1.f, 1.f));
    OrenNayar mat(15);

    //same hemisphere
    const Vec3 wo(-1.f, 0.f, .5f);
    const Vec3 wi(-0.707106769f, -0.0000000618172393f, 0.707106769f);
    float pdf;
    pdf = mat.pdf(&wo, &wi);
    EXPECT_NEAR(pdf, 0.22507906f, 1e-5f);

    const Vec3 wo2(-1.f, 0.f, -0.5f);
    pdf = mat.pdf(&wo2, &wi);
    EXPECT_EQ(pdf, 0.f);
}

SPECTRE_TEST(Material, SpecularReflection_flags)
{
    ConductorReflection cond(METALS[METAL_SILVER].n, METALS[METAL_SILVER].k);
    DielectricReflection diel(1.f, 1.33f);
    EXPECT_EQ(cond.get_flags(), (char)(FLAG_BRDF | FLAG_SPEC));
    EXPECT_FALSE(cond.matches(FLAG_BRDF));
    EXPECT_FALSE(cond.matches(FLAG_BTDF));
    EXPECT_FALSE(cond.matches(FLAG_SPEC));
    EXPECT_FALSE(cond.matches(FLAG_BRDF | FLAG_BTDF));
    EXPECT_TRUE(cond.matches(FLAG_BRDF | FLAG_SPEC));
    EXPECT_TRUE(cond.matches(FLAG_BRDF | FLAG_BTDF | FLAG_SPEC));
    EXPECT_FALSE(cond.matches(FLAG_BTDF | FLAG_SPEC));
    EXPECT_EQ(diel.get_flags(), (char)(FLAG_BRDF | FLAG_SPEC));
    EXPECT_FALSE(diel.matches(FLAG_BRDF));
    EXPECT_FALSE(diel.matches(FLAG_BTDF));
    EXPECT_FALSE(diel.matches(FLAG_SPEC));
    EXPECT_FALSE(diel.matches(FLAG_BRDF | FLAG_BTDF));
    EXPECT_TRUE(diel.matches(FLAG_BRDF | FLAG_SPEC));
    EXPECT_TRUE(diel.matches(FLAG_BRDF | FLAG_BTDF | FLAG_SPEC));
    EXPECT_FALSE(diel.matches(FLAG_BTDF | FLAG_SPEC));
}

SPECTRE_TEST(Material, SpecularReflection_value)
{
    ConductorReflection cond(METALS[METAL_COPPER].n, METALS[METAL_COPPER].k);
    DielectricReflection diel(1.f, 1.33f);
    const Vec3 wo(-1.f, 0.f, .4f);
    const Vec3 wi(1.f, 0.f, .6f);
    Spectrum res1 = cond.value(&wo, &wi);
    Spectrum res2 = diel.value(&wo, &wi);
    EXPECT_TRUE(res1.is_black());
    EXPECT_TRUE(res2.is_black());
}

SPECTRE_TEST(Material, SpecularReflection_sample_value)
{
    ConductorReflection cond(METALS[METAL_COPPER].n, METALS[METAL_COPPER].k);
    DielectricReflection diel(1.f, 1.33f);

    //outside, conductor
    const Vec3 wo(-1.f, 0.f, .4f);
    Vec3 wi;
    float pdf;
    Spectrum res = cond.sample_value(&wo, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_EQ(wi.x, -wo.x);
    EXPECT_EQ(wi.y, -wo.y);
    EXPECT_EQ(wi.z, wo.z);
    EXPECT_EQ(pdf, 1.f);
    //TODO: fix blue copper before 0.2.0
//    EXPECT_EQ(res.w[0],0.116884954f);
//    EXPECT_EQ(res.w[1],0.0602688268f);
//    EXPECT_EQ(res.w[2],0.00547898421f);

    //inside conductor
    const Vec3 wo2(-1.f, 0.f, -0.5f);
    res = cond.sample_value(&wo2, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_EQ(wi.x, -wo2.x);
    EXPECT_EQ(wi.y, -wo2.y);
    EXPECT_EQ(wi.z, wo2.z);
    EXPECT_EQ(sign(wo2.z), sign(wi.z));
    EXPECT_EQ(pdf, 1.f);

    //outside dielectric
    res = diel.sample_value(&wo, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_EQ(wi.x, -wo.x);
    EXPECT_EQ(wi.y, -wo.y);
    EXPECT_EQ(wi.z, wo.z);
    EXPECT_EQ(wi.z, wo.z);
    EXPECT_EQ(pdf, 1.f);

    //inside dielectric, tir
    res = diel.sample_value(&wo2, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_EQ(wi.x, -wo2.x);
    EXPECT_EQ(wi.y, -wo2.y);
    EXPECT_EQ(wi.z, wo2.z);
    EXPECT_EQ(sign(wo2.z), sign(wi.z));
    EXPECT_EQ(pdf, 1.f);

    //inside dielectric, no tir
    const Vec3 wo3(-1.f, 0.f, -0.9f);
    res = diel.sample_value(&wo3, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_EQ(wi.x, -wo3.x);
    EXPECT_EQ(wi.y, -wo3.y);
    EXPECT_EQ(wi.z, wo3.z);
    EXPECT_EQ(sign(wo3.z), sign(wi.z));
    EXPECT_EQ(pdf, 1.f);
}

SPECTRE_TEST(Material, SpecularReflection_pdf)
{
    ConductorReflection cond(METALS[METAL_COPPER].n, METALS[METAL_COPPER].k);
    DielectricReflection diel(1.f, 1.33f);

    //same hemisphere
    const Vec3 wo(-1.f, 0.f, .4f);
    const Vec3 wi(1.f, 0.f, .6f);
    float pdf;
    pdf = cond.pdf(&wo, &wi);
    EXPECT_EQ(pdf, 0.f);
    pdf = diel.pdf(&wo, &wi);
    EXPECT_EQ(pdf, 0.f);

    const Vec3 wo2(-1.f, 0.f, -0.5f);
    pdf = cond.pdf(&wo2, &wi);
    EXPECT_EQ(pdf, 0.f);
    pdf = diel.pdf(&wo, &wi);
    EXPECT_EQ(pdf, 0.f);
}

SPECTRE_TEST(Material, SpecularRefraction_flags)
{
    Refraction diel(cauchy(1.f, 0, 0), cauchy(1.33f, 0, 0));
    EXPECT_EQ(diel.get_flags(), (char)(FLAG_BTDF | FLAG_SPEC));
    EXPECT_FALSE(diel.matches(FLAG_BRDF));
    EXPECT_FALSE(diel.matches(FLAG_BTDF));
    EXPECT_FALSE(diel.matches(FLAG_SPEC));
    EXPECT_FALSE(diel.matches(FLAG_BRDF | FLAG_BTDF));
    EXPECT_FALSE(diel.matches(FLAG_BRDF | FLAG_SPEC));
    EXPECT_TRUE(diel.matches(FLAG_BRDF | FLAG_BTDF | FLAG_SPEC));
    EXPECT_TRUE(diel.matches(FLAG_BTDF | FLAG_SPEC));
}

SPECTRE_TEST(Material, SpecularRefraction_value)
{
    Refraction diel(cauchy(1.f, 0, 0), cauchy(1.33f, 0, 0));
    const Vec3 wo(-1.f, 0.f, .4f);
    const Vec3 wi(1.f, 0.f, .6f);
    Spectrum res = diel.value(&wo, &wi);
    EXPECT_TRUE(res.is_black());
}

SPECTRE_TEST(Material, SpecularRefraction_cauchy_sellmeier_equations)
{
    Spectrum c = cauchy(1.33f, 0, 0);
    EXPECT_EQ(c.w[0], 1.33f);

    Spectrum sm = sellmeier(1.03961212f, 0.231792344f, 1.01046945f,
                            6.00069867E-3f, 2.00179144E-2f, 103.560653f);
    EXPECT_EQ(sm.w[0], 1.5197562f);

}

SPECTRE_TEST(Material, SpecularRefraction_sample_value)
{
    Refraction diel(cauchy(1.f, 0, 0), cauchy(1.33f, 0, 0));
    const Vec3 wo(-1.f, 0.f, .4f);
    Vec3 wi;
    float pdf;

    //outside dielectric
    Spectrum res = diel.sample_value(&wo, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_EQ(wi.x, 0.751879692f);
    EXPECT_EQ(wi.y, 0.f);
    EXPECT_EQ(wi.z, -0.724657595f);
    EXPECT_EQ(res.w[0], 0.704294562f);
    EXPECT_EQ(res.w[1], 0.704294562f);
    EXPECT_EQ(res.w[2], 0.704294562f);
    EXPECT_NE(sign(wo.z), sign(wi.z));
    EXPECT_EQ(pdf, 1.f);

    //inside dielectric, tir
    const Vec3 wo2(-1.f, 0.f, -.5f);
    res = diel.sample_value(&wo2, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_TRUE(res.is_black());
    EXPECT_EQ(pdf, 0.f);

    //inside dielectric, no tir
    const Vec3 wo3(-1.f, 0.f, -0.9f);
    res = diel.sample_value(&wo3, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_EQ(wi.x, 1.33000004f);
    EXPECT_EQ(wi.y, 0.f);
    EXPECT_EQ(wi.z, 0.814805984f);
    EXPECT_EQ(res.w[0], 2.12246299f);
    EXPECT_EQ(res.w[1], 2.12246299f);
    EXPECT_EQ(res.w[2], 2.12246299f);
    EXPECT_NE(sign(wo3.z), sign(wi.z));
    EXPECT_EQ(pdf, 1.f);
}

SPECTRE_TEST(Material, SpecularRefraction_pdf)
{
    Refraction diel(cauchy(1.f, 0, 0), cauchy(1.33f, 0, 0));

    //same hemisphere
    const Vec3 wo(-1.f, 0.f, .4f);
    const Vec3 wi(1.f, 0.f, .6f);
    float pdf;
    pdf = diel.pdf(&wo, &wi);
    EXPECT_EQ(pdf, 0.f);

    const Vec3 wo2(-1.f, 0.f, -0.5f);
    pdf = diel.pdf(&wo, &wi);
    EXPECT_EQ(pdf, 0.f);
}

SPECTRE_TEST(Material, MicrofacetR_flags)
{
    Fresnel* fresnel = new Dielectric(cauchy(1.f, 0.f, 0.f),
                                      cauchy(1.33f, 0.f, 0.f));
    MicrofacetDist* blinn = new Blinn(100.f);
    MicrofacetR mat(blinn, fresnel);
    EXPECT_EQ(mat.get_flags(), (char)FLAG_BRDF);
    EXPECT_TRUE(mat.matches(FLAG_BRDF));
    EXPECT_FALSE(mat.matches(FLAG_BTDF));
    EXPECT_FALSE(mat.matches(FLAG_SPEC));
    EXPECT_TRUE(mat.matches(FLAG_BRDF | FLAG_BTDF));
    EXPECT_TRUE(mat.matches(FLAG_BRDF | FLAG_SPEC));
    EXPECT_TRUE(mat.matches(FLAG_BRDF | FLAG_BTDF | FLAG_SPEC));
    EXPECT_FALSE(mat.matches(FLAG_BTDF | FLAG_SPEC));
}

SPECTRE_TEST(Material, MicrofacetR_value)
{
    Fresnel* fresnel = new Dielectric(cauchy(1.f, 0.f, 0.f),
                                      cauchy(1.33f, 0.f, 0.f));
    MicrofacetDist* blinn = new Blinn(100.f);
    MicrofacetR mat(blinn, fresnel);
    Spectrum res;

    //different hemispheres
    Vec3 wo(-1.f, 0.f, -1.f);
    Vec3 wi(-1.f, 0.f, 1.f);
    res = mat.value(&wo, &wi);
    EXPECT_TRUE(res.is_black());

    //wo parallel to plane
    wo.z = 0.f;
    res = mat.value(&wo, &wi);
    EXPECT_TRUE(res.is_black());

    //wi parallel to plane
    wo.z = 1.f;
    wi.z = 0.f;
    res = mat.value(&wo, &wi);
    EXPECT_TRUE(res.is_black());

    //half vector zero length
    wo = Vec3(-1.f, 0.f, -1.f);
    wi = Vec3(1.f, 0.f, 1.f);
    res = mat.value(&wo, &wi);
    EXPECT_TRUE(res.is_black());

    wo.z = 1.f;
    res = mat.value(&wo, &wi);
    EXPECT_FALSE(res.is_black());
    EXPECT_NEAR(res.w[0], 0.0814097523f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.0814097523f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.0814097523f, 1e-5f);
}

SPECTRE_TEST(Material, MicrofacetR_sample_value)
{
    Fresnel* fresnel = new Dielectric(cauchy(1.f, 0.f, 0.f),
                                      cauchy(1.33f, 0.f, 0.f));
    MicrofacetDist* blinn = new Blinn(100.f);
    MicrofacetR mat(blinn, fresnel);
    Spectrum res;
    float pdf;

    //wi in the other hemisphere
    Vec3 wo(0.6f, 0.4f, 0.06f);
    Vec3 wi;
    res = mat.sample_value(&wo, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_TRUE(res.is_black());

    //correct value
    wo = Vec3(0.f, 0.f, 1.f);
    res = mat.sample_value(&wo, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_FALSE(res.is_black());
    EXPECT_NEAR(wi.x, -0.231914222f, 1e-5f);
    EXPECT_NEAR(wi.y, -0.0000000202745856f, 1e-5f);
    EXPECT_NEAR(wi.z, 0.972736239f, 1e-5f);
    EXPECT_NEAR(res.w[0], 0.0421385542f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.0421385542f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.0421385542f, 1e-5f);
    EXPECT_NEAR(pdf, 2.05727267f, 1e-5f);
}

SPECTRE_TEST(Material, MicrofacetR_pdf)
{
    Fresnel* fresnel = new Dielectric(cauchy(1.f, 0.f, 0.f),
                                      cauchy(1.33f, 0.f, 0.f));
    MicrofacetDist* blinn = new Blinn(100.f);
    MicrofacetR mat(blinn, fresnel);
    float pdf;

    //different hemispheres
    Vec3 wo(0.6f, 0.4f, 0.06f);
    Vec3 wi(-0.825808227f, -0.552789986f, -0.111641996f);
    pdf = mat.pdf(&wo, &wi);
    EXPECT_EQ(pdf, 0.f);

    //wh zero length
//    wi = Vec3(-0.6f,-0.4f,0.06f);
//    pdf = mat.pdf(&wo, &wi);
//    EXPECT_EQ(pdf,0.f);

    //previous test pdf
    wo = Vec3(0.f, 0.f, 1.f);
    wi = Vec3(-0.231914222f, -0.0000000202745856f, 0.972736239f);
    pdf = mat.pdf(&wo, &wi);
    EXPECT_NEAR(pdf, 2.057285f, 1e-5f);
}

SPECTRE_TEST(Material, MicrofacetT_flags)
{
    MicrofacetDist* beckmann = new Beckmann(0.3f);
    MicrofacetT mat(beckmann, cauchy(1.f, 0.f, 0.f), cauchy(1.33f, 0.f, 0.f));
    EXPECT_EQ(mat.get_flags(), (char)FLAG_BTDF);
    EXPECT_FALSE(mat.matches(FLAG_BRDF));
    EXPECT_TRUE(mat.matches(FLAG_BTDF));
    EXPECT_FALSE(mat.matches(FLAG_SPEC));
    EXPECT_TRUE(mat.matches(FLAG_BRDF | FLAG_BTDF));
    EXPECT_FALSE(mat.matches(FLAG_BRDF | FLAG_SPEC));
    EXPECT_TRUE(mat.matches(FLAG_BRDF | FLAG_BTDF | FLAG_SPEC));
    EXPECT_TRUE(mat.matches(FLAG_BTDF | FLAG_SPEC));
}

SPECTRE_TEST(Material, MicrofacetT_value)
{
    MicrofacetDist* beckmann = new Beckmann(0.3f);
    MicrofacetT mat(beckmann, cauchy(1.f, 0.f, 0.f), cauchy(1.33f, 0.f, 0.f));
    Spectrum res;

    //same hemispheres
    Vec3 wo(-1.f, 0.f, 1.f);
    Vec3 wi(-1.f, 0.f, 1.f);
    res = mat.value(&wo, &wi);
    EXPECT_TRUE(res.is_black());

    //wo parallel to plane
    wo.z = 0.f;
    res = mat.value(&wo, &wi);
    EXPECT_TRUE(res.is_black());

    //wi parallel to plane
    wo.z = 1.f;
    wi.z = 0.f;
    res = mat.value(&wo, &wi);
    EXPECT_TRUE(res.is_black());

    //half vector zero length
    wo = Vec3(-1.f, 0.f, -1.f);
    wi = Vec3(1.33f, 0.f, 1.33f);
    res = mat.value(&wo, &wi);
    EXPECT_TRUE(res.is_black());

    //D*G = 0
    wo = Vec3(0.6f, 0.4f, 0.06f);
    wi.z = -1.f;
    res = mat.value(&wo, &wi);
    EXPECT_TRUE(res.is_black());

    //wh negative
    wo = Vec3(-1.f, 0.f, 1.f);
    wi = Vec3(1.f, 0.f, -0.06f);
    res = mat.value(&wo, &wi);
    EXPECT_FALSE(res.is_black());

    //execution from outside
    wo = Vec3(0.6f, 0.4f, 0.06f);
    wi = Vec3(-0.321353912f, -0.351839572f, -0.879170417f);
    res = mat.value(&wo, &wi);
    EXPECT_NEAR(res.w[0], 0.109290525f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.109290525f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.109290525f, 1e-5f);

    //execution from inside
    wo.z = -0.9f;
    wi = Vec3(-0.659094095f, -0.404581398f, 0.63396281f);
    res = mat.value(&wo, &wi);
    EXPECT_NEAR(res.w[0], 2.83526969f, 1e-5f);
    EXPECT_NEAR(res.w[1], 2.83526969f, 1e-5f);
    EXPECT_NEAR(res.w[2], 2.83526969f, 1e-5f);
}

SPECTRE_TEST(Material, MicrofacetT_sample_value)
{
    MicrofacetDist* beckmann = new Beckmann(0.3f);
    MicrofacetT mat(beckmann, cauchy(1.f, 0.f, 0.f), cauchy(1.33f, 0.f, 0.f));
    Spectrum res;
    float pdf;
    Vec3 wi;

    //parallel to plane
    pdf = 1.f;
    Vec3 wo(-1.f, 0.f, 0.f);
    res = mat.sample_value(&wo, &wi, 0.5f, 0.5f, &pdf);
    EXPECT_EQ(pdf, 0.f);
    EXPECT_TRUE(res.is_black());

    //tir
    pdf = 1.f;
    wo = Vec3(0.6f, 0.4f, -0.5f);
    res = mat.sample_value(&wo, &wi, 0.6f, 0.4f, &pdf);
    EXPECT_EQ(pdf, 0.f);
    EXPECT_TRUE(res.is_black());

    //correct from outside
    pdf = 1.f;
    wo = Vec3(0.6f, 0.4f, 0.06f);
    wo.normalize();
    res = mat.sample_value(&wo, &wi, 0.6f, 0.4f, &pdf);
    EXPECT_NEAR(wi.x, -0.473553061f, 1e-5f);
    EXPECT_NEAR(wi.y, -0.524536133f, 1e-5f);
    EXPECT_NEAR(wi.z, -0.707537591f, 1e-5f);
    EXPECT_NEAR(res.w[0], 0.0087483963f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.0087483963f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.0087483963f, 1e-5f);
    EXPECT_NEAR(pdf, 0.0126143619f, 1e-5f);

    //correct from inside
    pdf = 1.f;
    wo = Vec3(0.6f, 0.4f, -0.9f);
    wo.normalize();
    res = mat.sample_value(&wo, &wi, 0.6f, 0.4f, &pdf);
    EXPECT_NEAR(wi.x, -0.59114027f, 1e-5f);
    EXPECT_NEAR(wi.y, -0.534547925f, 1e-5f);
    EXPECT_NEAR(wi.z, 0.603996515f, 1e-5f);
    EXPECT_NEAR(res.w[0], 5.97027016f, 1e-5f);
    EXPECT_NEAR(res.w[1], 5.97027016f, 1e-5f);
    EXPECT_NEAR(res.w[2], 5.97027016f, 1e-5f);
    EXPECT_NEAR(pdf, 4.47990608f, 1e-5f);
}

SPECTRE_TEST(Material, MicrofacetT_pdf)
{
    MicrofacetDist* beckmann = new Beckmann(0.3f);
    MicrofacetT mat(beckmann, cauchy(1.f, 0.f, 0.f), cauchy(1.33f, 0.f, 0.f));
    float pdf;

    //same hemispheres
    Vec3 wo(-1.f, 0.f, 1.f);
    Vec3 wi(-1.f, 0.f, 1.f);
    pdf = mat.pdf(&wo, &wi);
    EXPECT_EQ(pdf, 0.f);

    //wo parallel to plane
    wo.z = 0.f;
    pdf = mat.pdf(&wo, &wi);
    EXPECT_EQ(pdf, 0.f);

    //wi parallel to plane
    wo.z = 1.f;
    wi.z = 0.f;
    pdf = mat.pdf(&wo, &wi);
    EXPECT_EQ(pdf, 0.f);

    //half vector zero length
    wo = Vec3(-1.f, 0.f, -1.f);
    wi = Vec3(1.33f, 0.f, 1.33f);
    pdf = mat.pdf(&wo, &wi);
    EXPECT_EQ(pdf, 0.f);

    pdf = 1.f;
    wo = Vec3(0.6f, 0.4f, 0.06f);
    wo.normalize();
    wi = Vec3(-0.473553061f, -0.524536133f, -0.707537591f);
    pdf = mat.pdf(&wo, &wi);
    EXPECT_NEAR(pdf, 0.012614323f, 1e-5f);

    //correct from inside
    pdf = 1.f;
    wo = Vec3(0.6f, 0.4f, -0.9f);
    wo.normalize();
    wi = Vec3(-0.59114027f, -0.534547925f, 0.603996515f);
    pdf = mat.pdf(&wo, &wi);
    EXPECT_NEAR(pdf, 4.47991657f, 1e-5f);
}

SPECTRE_TEST(Material, FresnelConditions_Conductor_eval)
{
    Conductor cond(METALS[METAL_COPPER].n, METALS[METAL_COPPER].k);
    Spectrum res = cond.eval(0.5f);
    EXPECT_NEAR(res.w[0], 0.548298597f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.688397527f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.937712908f, 1e-5f);
}

SPECTRE_TEST(Material, FresnelConditions_Dielectric_eval)
{
    Dielectric diel(cauchy(1.f, 0.f, 0.f), cauchy(1.33f, 0.f, 0.f));
    Spectrum res;

    //get_eta
    EXPECT_NEAR(diel.get_eta_incident(), 1.f, 1e-5f);
    EXPECT_NEAR(diel.get_eta_transmitted(), 1.33f, 1e-5f);

    //outside
    res = diel.eval(0.5f);
    EXPECT_NEAR(res.w[0], 0.0591256134f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.0591256134f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.0591256134f, 1e-5f);

    //inside no tir
    res = diel.eval(-0.9f);
    EXPECT_NEAR(res.w[0], 0.0223328397f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.0223328397f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.0223328397f, 1e-5f);

    //inside tir
    res = diel.eval(-0.4f);
    EXPECT_EQ(res.w[0], 1.f);
    EXPECT_EQ(res.w[1], 1.f);
    EXPECT_EQ(res.w[2], 1.f);
}

SPECTRE_TEST(Material, MultiBSDF_inherit_bdf)
{
    MultiBSDF material;
    material.inherit_bdf(new Lambertian());
    for(int i = 1; i<_MAX_BDF_; i++)
        material.inherit_bdf(new Lambertian());
    Bdf* lambertian = new Lambertian();
    errors_count[ERROR_INDEX] = 0;
    material.inherit_bdf(lambertian);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
    errors_count[ERROR_INDEX] = 0;
    material.inherit_bdf(lambertian);
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
    delete lambertian;
}

SPECTRE_TEST(Material, MultiBSDF_value)
{
    const Bsdf* materials[1];
    MultiBSDF material_r;
    MultiBSDF metal;
    MultiBSDF materialWtexture;
    unsigned char associations = 0;
    ColorRGB red((unsigned char)255, 0, 0);
    Texture* ut = new TextureUniform(Spectrum(red, false));
    Sphere s;
    Matrix4 m;
    Vec3 wi;
    Normal shading_normal;
    Spectrum res;
    m.set_translation(Vec3(-2, 0, 0));
    Asset a(&s, m, 1);
    Ray r(Point3(-2, -10, 0), Vec3(0, 1, 0));
    HitPoint hit;
    ShadingSpace matrix;
    float distance = FLT_MAX;
    material_r.inherit_bdf(new Lambertian());
    materialWtexture.inherit_bdf(new Lambertian(), ut);
    metal.inherit_bdf(new ConductorReflection(METALS[METAL_GOLD].n,
                                              METALS[METAL_GOLD].k));
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));

    //reflected ray spec ok
    wi = Vec3(0.f, 1.f, 0.f);
    wi.normalize();
    materials[0] = &material_r;
    a.set_materials(materials, 1, &associations);
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));
    a.get_material(0)->gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix, true);
    EXPECT_FALSE(res.is_black());
    EXPECT_NEAR(res.w[0], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.318309873f, 1e-5f);

    //reflected ray spec not ok
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix, false);
    EXPECT_FALSE(res.is_black());
    EXPECT_NEAR(res.w[0], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.318309873f, 1e-5f);

    //specular ray not allowed
    materials[0] = &metal;
    a.set_materials(materials, 1, &associations);
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix, false);
    EXPECT_TRUE(res.is_black());

    //specular ray allowed (however value will always return false for specular)
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix, true);
    EXPECT_TRUE(res.is_black());

    //multi material, pick only non specular
    metal.inherit_bdf(new Lambertian());
    materials[0] = &metal;
    a.set_materials(materials, 1, &associations);
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix, true);
    EXPECT_NEAR(res.w[0], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.318309873f, 1e-5f);

    //textured material
    wi = Vec3(0.f, 1.f, 0.f);
    materials[0] = &materialWtexture;
    a.set_materials(materials, 1, &associations);
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix, true);
    EXPECT_NEAR(res.w[0], 0.131288946f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.0676958858f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.00615417166f, 1e-5f);
    delete ut;

    //multi material - transmitted part
    MultiBSDF mat_glass;
    Spectrum ior_i = cauchy(1.f, 0.f);
    Spectrum ior_t = cauchy(1.33f, 0.f);
    mat_glass.inherit_bdf(new DielectricReflection(ior_i, ior_t));
    mat_glass.inherit_bdf(new Refraction(ior_i, ior_t));
    //multi material specular, choose first (reflection)
    mat_glass.gen_shading_matrix(&hit, &matrix, &shading_normal);
    wi = Vec3(0.f, -1.f, 0.f);
    materials[0] = &mat_glass;
    a.set_materials(materials, 1, &associations);
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix, true);
    EXPECT_TRUE(res.is_black());//-> no value for specular transmitted :'(
}

SPECTRE_TEST(Material, MultiBSDF_sample_value)
{
    bool matched_spec;
    Sphere s;
    Matrix4 m;
    Vec3 wi;
    Normal shading_normal;
    float pdf;
    Spectrum res;
    m.set_translation(Vec3(-2, 0, 0));
    Asset a(&s, m, 1);
    Ray r(Point3(-2, -10, 0), Vec3(0, 1, 0));
    HitPoint hit;
    ShadingSpace matrix;
    float distance = FLT_MAX;
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));

    MultiBSDF material_r;
    material_r.inherit_bdf(new Lambertian());

    //brdf diffuse match spec ok
    material_r.gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = material_r.sample_value(0.5f, 0.5f, 0.5f, &(r.direction), &hit,
                                  &matrix, &wi,
                                  &pdf, true, &matched_spec);
    EXPECT_NEAR(res.w[0], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.318309873f, 1e-5f);
    EXPECT_NEAR(pdf, 0.22507906f, 1e-5f);
    EXPECT_NEAR(wi.x, -0.707106769f, 1e-5f);
    EXPECT_NEAR(wi.y, 0.707106769f, 1e-5f);
    EXPECT_TRUE(flt_equal(wi.z, 0.f));
    EXPECT_EQ(matched_spec, false);
    EXPECT_TRUE(wi.is_normalized());

    //brdf diffuse match spec not ok
    res = material_r.sample_value(0.5f, 0.5f, 0.5f, &(r.direction), &hit,
                                  &matrix, &wi,
                                  &pdf, false, &matched_spec);
    EXPECT_NEAR(res.w[0], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.318309873f, 1e-5f);
    EXPECT_NEAR(pdf, 0.22507906f, 1e-5f);
    EXPECT_NEAR(wi.x, -0.707106769f, 1e-5f);
    EXPECT_NEAR(wi.y, 0.707106769f, 1e-5f);
    EXPECT_TRUE(flt_equal(wi.z, 0.f));
    EXPECT_EQ(matched_spec, false);
    EXPECT_TRUE(wi.is_normalized());

    //textured material
    MultiBSDF materialWtexture;
    ColorRGB red((unsigned char)255, 0, 0);
    Texture* ut = new TextureUniform(Spectrum(red, false));
    materialWtexture.inherit_bdf(new Lambertian(), ut);
    materialWtexture.gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = materialWtexture.sample_value(0.5f, 0.5f, 0.5f, &(r.direction), &hit,
                                        &matrix,
                                        &wi, &pdf, false, &matched_spec);
    EXPECT_NEAR(res.w[0], 0.131288946f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.0676958858f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.00615417166f, 1e-5f);
    EXPECT_NEAR(pdf, 0.22507906f, 1e-5f);
    EXPECT_NEAR(wi.x, -0.707106769f, 1e-5f);
    EXPECT_NEAR(wi.y, 0.707106769f, 1e-5f);
    EXPECT_NEAR(wi.z, 0.f, 1e-5f);
    EXPECT_EQ(matched_spec, false);
    EXPECT_TRUE(wi.is_normalized());

    MultiBSDF mat_glass;
    Spectrum ior_i = cauchy(1.f, 0.f);
    Spectrum ior_t = cauchy(1.33f, 0.f);
    mat_glass.inherit_bdf(new DielectricReflection(ior_i, ior_t));
    mat_glass.inherit_bdf(new Refraction(ior_i, ior_t));
    //multi material specular, choose first (reflection)
    mat_glass.gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = mat_glass.sample_value(0.1f, 0.5f, 0.5f, &(r.direction), &hit,
                                 &matrix, &wi,
                                 &pdf, true, &matched_spec);
    EXPECT_NEAR(res.w[0], 0.0200593174f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.0200593174f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.0200593174f, 1e-5f);
    EXPECT_NEAR(pdf, 0.5f, 1e-5f);
    EXPECT_NEAR(wi.x, 0.f, 1e-5f);
    EXPECT_NEAR(wi.y, 1.f, 1e-5f);
    EXPECT_NEAR(wi.z, 0.f, 1e-5f);
    EXPECT_EQ(matched_spec, true);
    EXPECT_TRUE(wi.is_normalized());
    //multi material specular, choose second (refraction)
    res = mat_glass.sample_value(1.f, 0.5f, 0.5f, &(r.direction), &hit, &matrix,
                                 &wi,
                                 &pdf, true, &matched_spec);
    EXPECT_NEAR(res.w[0], 1.73341715f, 1e-5f);
    EXPECT_NEAR(res.w[1], 1.73341715f, 1e-5f);
    EXPECT_NEAR(res.w[2], 1.73341715f, 1e-5f);
    EXPECT_NEAR(pdf, 0.5f, 1e-5f);
    EXPECT_NEAR(wi.x, 0.f, 1e-5f);
    EXPECT_NEAR(wi.y, -1.f, 1e-5f);
    EXPECT_NEAR(wi.z, 0.f, 1e-5f);
    EXPECT_EQ(matched_spec, true);
    EXPECT_TRUE(wi.is_normalized());

    //multi material specular, Total Internal Reflection
    Vec3 woW(0.5, 0.2, -0.4);
    res = mat_glass.sample_value(1.f, 0.7, 0.3, &woW, &hit, &matrix, &wi, &pdf,
                                 true, &matched_spec);
    EXPECT_TRUE(res.is_black());

    //brdf no match
    res = mat_glass.sample_value(0.5f, 0.5f, 0.5f, &(r.direction), &hit,
                                 &matrix, &wi,
                                 &pdf, false, &matched_spec);
    EXPECT_TRUE(res.is_black());
    EXPECT_EQ(pdf, 0.f);

    //multi material specular + non specular, non specular not allowed
    mat_glass.inherit_bdf((Bdf*)new Lambertian());
    res = material_r.sample_value(0.5f, 0.5f, 0.5f, &(r.direction), &hit,
                                  &matrix, &wi,
                                  &pdf, false, &matched_spec);
    EXPECT_NEAR(res.w[0], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.318309873f, 1e-5f);
    EXPECT_NEAR(pdf, 0.22507906f, 1e-5f);
    EXPECT_NEAR(wi.x, -0.707106769f, 1e-5f);
    EXPECT_NEAR(wi.y, 0.707106769f, 1e-5f);
    EXPECT_NEAR(wi.z, 0.f, 1e-5f);
    EXPECT_EQ(matched_spec, false);
    EXPECT_TRUE(wi.is_normalized());

    MultiBSDF mat_glossy;
    MicrofacetDist* ggx1 = new GGXiso(0.2f);
    MicrofacetDist* ggx2 = new GGXiso(0.2f);
    mat_glossy.inherit_bdf(new MicrofacetR(ggx1,
                                           new Dielectric(ior_i, ior_t)));
    mat_glossy.inherit_bdf(new MicrofacetT(ggx2, ior_i, ior_t));
    mat_glossy.gen_shading_matrix(&hit, &matrix, &shading_normal);
    //multi material glossy, choose first (reflection)
    res = mat_glossy.sample_value(0.1f, 0.5f, 0.5f, &(r.direction), &hit,
                                  &matrix, &wi,
                                  &pdf, false, &matched_spec);
    EXPECT_NEAR(res.w[0], 0.0116626127f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.0116626127f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.0116626127f, 1e-5f);
    EXPECT_NEAR(pdf, 0.268972039f, 1e-5f);
    EXPECT_NEAR(wi.x, 0.384615242f, 1e-5f);
    EXPECT_NEAR(wi.y, 0.923076987f, 1e-5f);
    EXPECT_NEAR(wi.z, 0.f, 1e-5f);
    EXPECT_EQ(matched_spec, false);
    EXPECT_TRUE(wi.is_normalized());
    //multi material glossy, choose second (refraction)
    res = mat_glossy.sample_value(1.f, 0.5f, 0.5f, &(r.direction), &hit,
                                  &matrix, &wi,
                                  &pdf, false, &matched_spec);
    EXPECT_NEAR(res.w[0], 30.8263855f, 1e-5f);
    EXPECT_NEAR(res.w[1], 30.8263855f, 1e-5f);
    EXPECT_NEAR(res.w[2], 30.8263855f, 1e-5f);
    EXPECT_NEAR(pdf, 15.9433241f, 1e-5f);
    EXPECT_NEAR(wi.x, 0.0664419233f, 1e-5f);
    EXPECT_NEAR(wi.y, -0.997790277f, 1e-5f);
    EXPECT_NEAR(wi.z, 0.f, 1e-5f);
    EXPECT_EQ(matched_spec, false);
    EXPECT_TRUE(wi.is_normalized());
    delete ut;
}

SPECTRE_TEST(Material, MultiBSDF_pdf)
{
    Sphere s;
    Matrix4 m;
    Normal shading_normal;
    Vec3 wi;
    float pdf;
    m.set_translation(Vec3(-2, 0, 0));
    Asset a(&s, m, 1);
    Ray r(Point3(-2, -10, 0), Vec3(0, 1, 0));
    HitPoint hit;
    ShadingSpace matrix;
    float distance = FLT_MAX;
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));

    MultiBSDF material;
    MultiBSDF metal;
    //empty
    wi = Vec3(0.f, 1.f, 0.f);
    material.gen_shading_matrix(&hit, &matrix, &shading_normal);
    pdf = material.pdf(&(r.direction), &hit, &matrix, &wi, false);
    EXPECT_EQ(pdf, 0.f);

    //non matching
    metal.inherit_bdf(new ConductorReflection(METALS[METAL_GOLD].n,
                                              METALS[METAL_GOLD].k));
    pdf = metal.pdf(&(r.direction), &hit, &matrix, &wi, false);
    EXPECT_EQ(pdf, 0.f);

    //matching
    material.inherit_bdf(new Lambertian());
    pdf = material.pdf(&(r.direction), &hit, &matrix, &wi, false);
    EXPECT_NEAR(pdf, 0.318309873f, 1e-5f);

    //multiple values
    MultiBSDF material2;
    float pdf2;
    material2.inherit_bdf(new Lambertian());
    material2.inherit_bdf(new Lambertian());
    material2.gen_shading_matrix(&hit, &matrix, &shading_normal);
    pdf2 = material2.pdf(&(r.direction), &hit, &matrix, &wi, false);
    EXPECT_NEAR(pdf2, pdf, 1e-5f);
}

SPECTRE_TEST(Material, SingleBRDF_inherit_bdf)
{
    //add single reflective material
    errors_count[WARNING_INDEX] = 0;
    SingleBRDF material(new Lambertian());
    EXPECT_EQ(errors_count[WARNING_INDEX], 0);
    //fail to add transmittive material
    errors_count[WARNING_INDEX] = 0;
    SingleBRDF material2(new Refraction(cauchy(1.0, 0.f), cauchy(1.33f, 0.f)));
    EXPECT_EQ(errors_count[WARNING_INDEX], 1);
    errors_count[WARNING_INDEX] = 0;
}

SPECTRE_TEST(Material, SingleBRDF_value)
{
    const Bsdf* materials[1];
    unsigned char associations = 0;
    ColorRGB red((unsigned char)255, 0, 0);
    Texture* ut = new TextureUniform(Spectrum(red, false));
    Sphere s;
    Matrix4 m;
    Vec3 wi;
    Normal shading_normal;
    Spectrum res;
    m.set_translation(Vec3(-2, 0, 0));
    Asset a(&s, m, 1);
    Ray r(Point3(-2, -10, 0), Vec3(0, 1, 0));
    HitPoint hit;
    ShadingSpace matrix;
    float distance = FLT_MAX;
    SingleBRDF material_r(new Lambertian());
    SingleBRDF materialWtexture(new Lambertian(), ut);
    SingleBRDF metal(new ConductorReflection(METALS[METAL_GOLD].n,
                                             METALS[METAL_GOLD].k));
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));

    //matching brdf no-spec
    wi = Vec3(0.f, 1.f, 0.f);
    wi.normalize();
    materials[0] = &material_r;
    a.set_materials(materials, 1, &associations);
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));
    a.get_material(0)->gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix, false);
    EXPECT_FALSE(res.is_black());
    EXPECT_NEAR(res.w[0], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.318309873f, 1e-5f);

    //transmitted ray, so the singlebrdf returns black
    wi = Vec3(0.f, -1.f, 0.f);
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix, false);
    EXPECT_TRUE(res.is_black());

    //matching brdf yes-spec
    wi = Vec3(0.f, 1.f, 0.f);
    wi.normalize();
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix, true);
    EXPECT_FALSE(res.is_black());
    EXPECT_NEAR(res.w[0], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.318309873f, 1e-5f);

    //non matching spec
    materials[0] = &metal;
    a.set_materials(materials, 1, &associations);
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));
    a.get_material(0)->gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix, false);
    EXPECT_TRUE(res.is_black());

    //matching spec.. but value does not allow me to return > 0 for specular
    wi = Vec3(0.f, 1.f, 0.f);
    wi.normalize();
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix, true);
    EXPECT_TRUE(res.is_black());

    //textured material
    materials[0] = &materialWtexture;
    a.set_materials(materials, 1, &associations);
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));
    a.get_material(0)->gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix, true);
    EXPECT_NEAR(res.w[0], 0.131288946f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.0676958858f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.00615417166f, 1e-5f);
    delete ut;
}

SPECTRE_TEST(Material, SingleBRDF_sample_value)
{
    const Bsdf* materials[1];
    unsigned char association = 0;
    bool matched_spec;
    Sphere s;
    Matrix4 m;
    Vec3 wi;
    Normal shading_normal;
    float pdf;
    Spectrum res;
    m.set_translation(Vec3(-2, 0, 0));
    Asset a(&s, m, 1);
    Ray r(Point3(-2, -10, 0), Vec3(0, 1, 0));
    HitPoint hit;
    ShadingSpace matrix;
    float distance = FLT_MAX;
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));

    SingleBRDF material_r(new Lambertian());
    SingleBRDF metal(new ConductorReflection(METALS[METAL_GOLD].n,
                                             METALS[METAL_GOLD].k));
    materials[0] = &metal;
    a.set_materials(materials, 1, &association);
    //brdf specular no match
    metal.gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = metal.sample_value(0.5f, 0.5f, 0.5f, &(r.direction), &hit, &matrix,
                             &wi,
                             &pdf, false, &matched_spec);
    EXPECT_TRUE(res.is_black());

    //brdf specular match
    res = metal.sample_value(0.5f, 0.5f, 0.5f, &(r.direction), &hit, &matrix,
                             &wi,
                             &pdf, true, &matched_spec);
    EXPECT_NEAR(res.w[0], 2.43357158f, 1e-5f);
    EXPECT_NEAR(res.w[1], 1.25041258f, 1e-5f);
    EXPECT_NEAR(res.w[2], 1.03968287f, 1e-5f);
    EXPECT_NEAR(pdf, 1.f, 1e-5f);
    EXPECT_NEAR(wi.x, 0.f, 1e-5f);
    EXPECT_NEAR(wi.y, 1.f, 1e-5f);
    EXPECT_NEAR(wi.z, 0.f, 1e-5f);
    EXPECT_EQ(matched_spec, true);
    EXPECT_TRUE(wi.is_normalized());

    //brdf diffuse match with no-spec requested
    material_r.gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = material_r.sample_value(0.5f, 0.5f, 0.5f, &(r.direction), &hit,
                                  &matrix, &wi,
                                  &pdf, false, &matched_spec);
    EXPECT_NEAR(res.w[0], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.318309873f, 1e-5f);
    EXPECT_NEAR(pdf, 0.22507906f, 1e-5f);
    EXPECT_NEAR(wi.x, -0.707106769f, 1e-5f);
    EXPECT_NEAR(wi.y, 0.707106769f, 1e-5f);
    EXPECT_NEAR(wi.z, 0.f, 1e-5f);
    EXPECT_EQ(matched_spec, false);
    EXPECT_TRUE(wi.is_normalized());

    //brdf diffuse match with spec requested
    res = material_r.sample_value(0.5f, 0.5f, 0.5f, &(r.direction), &hit,
                                  &matrix, &wi,
                                  &pdf, true, &matched_spec);
    EXPECT_NEAR(res.w[0], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.318309873f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.318309873f, 1e-5f);
    EXPECT_NEAR(pdf, 0.22507906f, 1e-5f);
    EXPECT_NEAR(wi.x, -0.707106769f, 1e-5f);
    EXPECT_NEAR(wi.y, 0.707106769f, 1e-5f);
    EXPECT_NEAR(wi.z, 0.f, 1e-5f);
    EXPECT_EQ(matched_spec, false);
    EXPECT_TRUE(wi.is_normalized());

    //textured material
    ColorRGB red((unsigned char)255, 0, 0);
    Texture* ut = new TextureUniform(Spectrum(red, false));
    SingleBRDF materialWtexture(new Lambertian(), ut);
    materialWtexture.gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = materialWtexture.sample_value(0.5f, 0.5f, 0.5f, &(r.direction), &hit,
                                        &matrix,
                                        &wi, &pdf, false, &matched_spec);
    EXPECT_NEAR(res.w[0], 0.131288946f, 1e-5f);
    EXPECT_NEAR(res.w[1], 0.0676958858f, 1e-5f);
    EXPECT_NEAR(res.w[2], 0.00615417166f, 1e-5f);
    EXPECT_NEAR(pdf, 0.22507906f, 1e-5f);
    EXPECT_NEAR(wi.x, -0.707106769f, 1e-5f);
    EXPECT_NEAR(wi.y, 0.707106769f, 1e-5f);
    EXPECT_TRUE(flt_equal(wi.z, 0.f));
    EXPECT_EQ(matched_spec, false);
    EXPECT_TRUE(wi.is_normalized());
    delete ut;
}

SPECTRE_TEST(Material, SingleBRDF_pdf)
{
    Sphere s;
    Matrix4 m;
    Vec3 wi;
    Normal shading_normal;
    float pdf;
    m.set_translation(Vec3(-2, 0, 0));
    Asset a(&s, m, 1);
    Ray r(Point3(-2, -10, 0), Vec3(0, 1, 0));
    HitPoint hit;
    ShadingSpace matrix;
    float distance = FLT_MAX;
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));

    SingleBRDF material_r(new Lambertian());
    SingleBRDF metal(new ConductorReflection(METALS[METAL_GOLD].n,
                                             METALS[METAL_GOLD].k));
    wi = Vec3(0.f, 1.f, 0.f);

    //non matching spec
    metal.gen_shading_matrix(&hit, &matrix, &shading_normal);
    pdf = metal.pdf(&(r.direction), &hit, &matrix, &wi, false);
    EXPECT_EQ(pdf, 0.f);

    //matching spec
    pdf = metal.pdf(&(r.direction), &hit, &matrix, &wi, true);
    EXPECT_EQ(pdf, 0.f);

    //matching brdf no-spec
    material_r.gen_shading_matrix(&hit, &matrix, &shading_normal);
    pdf = material_r.pdf(&(r.direction), &hit, &matrix, &wi, false);
    EXPECT_NEAR(pdf, 0.318309873f, 1e-5f);

    //matching brdf yes-spec
    pdf = material_r.pdf(&(r.direction), &hit, &matrix, &wi, true);
    EXPECT_NEAR(pdf, 0.318309873f, 1e-5f);
}

SPECTRE_TEST(Material, DualBsdf_value)
{
    const Bsdf* materials[1];
    unsigned char associations = 0;
    Sphere s;
    Matrix4 m;
    Vec3 wi = Vec3(0, 1, 0);
    Normal shading_normal;
    m.set_translation(Vec3(-2, 0, 0));
    Asset a(&s, m, 1);
    Ray r(Point3(-2, -10, 0), Vec3(0, 1, 0));
    HitPoint hit;
    ShadingSpace matrix;
    ColorXYZ res;
    ColorXYZ alt;
    float distance = FLT_MAX;
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));

    TextureUniform red(Spectrum(ColorRGB(1.f, 0.f, 0.f), false));
    TextureUniform blue(Spectrum(ColorRGB(0.f, 0.f, 1.f), false));
    Vec2 scale(1.f);
    Vec2 shift(0.f);
    TextureImage tx(TEST_ASSETS "images/correct.bmp", shift, scale, UNFILTERED);
    SingleBRDF mat0(new Lambertian(), &red);
    SingleBRDF mat1(new Lambertian(), &blue);
    MaskBoolean mask(&tx, RED, false);

    //masked
    DualBsdf dual(&mat0, &mat1, mask);
    materials[0] = &dual;
    a.set_materials(materials, 1, &associations);
    ASSERT_TRUE(a.intersect(&r, &distance, &hit));
    a.get_material(0)->gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix,
                                   false).to_xyz();
    alt = mat1.value(&r.direction, &hit, &wi, &matrix, false).to_xyz();
    EXPECT_EQ(res.r, alt.r);
    EXPECT_EQ(res.g, alt.g);
    EXPECT_EQ(res.b, alt.b);

    //not masked
    r = Ray(Point3(-2.9, 0.4, 0), Vec3(0, 1, 0));
    ASSERT_TRUE(a.intersect(&r, &distance, &hit));
    a.get_material(0)->gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = a.get_material(0)->value(&r.direction, &hit, &wi, &matrix,
                                   false).to_xyz();
    alt = mat0.value(&r.direction, &hit, &wi, &matrix, false).to_xyz();
    EXPECT_EQ(res.r, alt.r);
    EXPECT_EQ(res.g, alt.g);
    EXPECT_EQ(res.b, alt.b);
}

SPECTRE_TEST(Material, DualBsdf_sample_value)
{
    const Bsdf* materials[1];
    unsigned char associations = 0;
    Sphere s;
    Matrix4 m;
    bool matched_spec;
    Vec3 wi;
    Vec3 wi_alt;
    float pdf;
    float pdf_alt;
    Normal shading_normal;
    m.set_translation(Vec3(-2, 0, 0));
    Asset a(&s, m, 1);
    Ray r(Point3(-2, -10, 0), Vec3(0, 1, 0));
    HitPoint hit;
    ShadingSpace matrix;
    ColorXYZ res;
    ColorXYZ alt;
    float distance = FLT_MAX;
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));

    TextureUniform red(Spectrum(ColorRGB(1.f, 0.f, 0.f), false));
    TextureUniform blue(Spectrum(ColorRGB(0.f, 0.f, 1.f), false));
    Vec2 scale(1.f);
    Vec2 shift(0.f);
    TextureImage tx(TEST_ASSETS "images/correct.bmp", shift, scale, UNFILTERED);
    SingleBRDF mat0(new Lambertian(), &red);
    SingleBRDF mat1(new Lambertian(), &blue);
    MaskBoolean mask(&tx, RED, false);

    //masked
    DualBsdf dual(&mat0, &mat1, mask);
    materials[0] = &dual;
    a.set_materials(materials, 1, &associations);
    ASSERT_TRUE(a.intersect(&r, &distance, &hit));
    a.get_material(0)->gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = a.get_material(0)->sample_value(0.5f, 0.5f, 0.5f, &(r.direction),
                                          &hit, &matrix, &wi, &pdf, false,
                                          &matched_spec).to_xyz();
    alt = mat1.sample_value(0.5f, 0.5f, 0.5f, &(r.direction), &hit, &matrix,
                            &wi_alt, &pdf_alt, false, &matched_spec).to_xyz();
    EXPECT_EQ(res.r, alt.r);
    EXPECT_EQ(res.g, alt.g);
    EXPECT_EQ(res.b, alt.b);
    EXPECT_EQ(wi.x, wi_alt.x);
    EXPECT_EQ(wi.y, wi_alt.y);
    EXPECT_EQ(wi.z, wi_alt.z);
    EXPECT_EQ(pdf, pdf_alt);

    //not masked
    r = Ray(Point3(-2.9, 0.4, 0), Vec3(0, 1, 0));
    ASSERT_TRUE(a.intersect(&r, &distance, &hit));
    a.get_material(0)->gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = a.get_material(0)->sample_value(0.5f, 0.5f, 0.5f, &(r.direction),
                                          &hit, &matrix, &wi, &pdf, false,
                                          &matched_spec).to_xyz();
    alt = mat0.sample_value(0.5f, 0.5f, 0.5f, &(r.direction), &hit, &matrix,
                            &wi_alt, &pdf_alt, false, &matched_spec).to_xyz();
    EXPECT_EQ(res.r, alt.r);
    EXPECT_EQ(res.g, alt.g);
    EXPECT_EQ(res.b, alt.b);
    EXPECT_EQ(wi.x, wi_alt.x);
    EXPECT_EQ(wi.y, wi_alt.y);
    EXPECT_EQ(wi.z, wi_alt.z);
    EXPECT_EQ(pdf, pdf_alt);
}

SPECTRE_TEST(Material, DualBsdf_pdf)
{
    const Bsdf* materials[1];
    unsigned char associations = 0;
    Sphere s;
    Matrix4 m;
    Vec3 wi;
    Normal shading_normal;
    m.set_translation(Vec3(-2, 0, 0));
    Asset a(&s, m, 1);
    Ray r(Point3(-2, -10, 0), Vec3(0, 1, 0));
    HitPoint hit;
    ShadingSpace matrix;
    float res;
    float alt;
    float distance = FLT_MAX;
    EXPECT_TRUE(a.intersect(&r, &distance, &hit));

    TextureUniform red(Spectrum(ColorRGB(1.f, 0.f, 0.f), false));
    TextureUniform blue(Spectrum(ColorRGB(0.f, 0.f, 1.f), false));
    Vec2 scale(1.f);
    Vec2 shift(0.f);
    TextureImage tx(TEST_ASSETS "images/correct.bmp", shift, scale, UNFILTERED);
    SingleBRDF mat0(new Lambertian(), &red);
    SingleBRDF mat1(new Lambertian(), &blue);
    MaskBoolean mask(&tx, RED, false);

    //masked
    DualBsdf dual(&mat0, &mat1, mask);
    materials[0] = &dual;
    a.set_materials(materials, 1, &associations);
    ASSERT_TRUE(a.intersect(&r, &distance, &hit));
    a.get_material(0)->gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = a.get_material(0)->pdf(&(r.direction), &hit, &matrix, &wi, false);
    alt = mat1.pdf(&(r.direction), &hit, &matrix, &wi, false);
    EXPECT_EQ(res, alt);

    //not masked
    r = Ray(Point3(-2.9, 0.4, 0), Vec3(0, 1, 0));
    ASSERT_TRUE(a.intersect(&r, &distance, &hit));
    a.get_material(0)->gen_shading_matrix(&hit, &matrix, &shading_normal);
    res = a.get_material(0)->pdf(&(r.direction), &hit, &matrix, &wi, false);
    alt = mat0.pdf(&(r.direction), &hit, &matrix, &wi, false);
    EXPECT_EQ(res, alt);
}

SPECTRE_TEST_END(Material)
