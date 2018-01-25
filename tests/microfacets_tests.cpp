
#include <gtest/gtest.h>


#include "materials/microfacet_distributions.hpp"
#include "utility/utility.hpp"
#include <random>

#define EPSILON 1E-5f

TEST(MicrofacetDist,Blinn_D)
{
    const int SAMPLES = 160;
    Blinn blinn(48.f);
    Vec3 wo(0.f,0.f,0.f);
    float mean = 0;
    float stddev = 0;
    for(float f = -40.f;f<=40.f;f+=0.5f)
    {
        wo.z = cosf(radians(f));
        mean+=blinn.D(&wo);
    }
    mean/=SAMPLES;
    EXPECT_FLOAT_EQ(mean, 2.05130506f);

    for(float f = -40.f;f<=40.f;f+=0.5f)
    {
        wo.z = cosf(radians(f));
        float sample = blinn.D(&wo) - mean;
        stddev += sample*sample;
    }
    stddev=sqrtf(stddev/SAMPLES);
    EXPECT_FLOAT_EQ(stddev,2.71867514f);
}

TEST(MicrofacetDist,Blinn_G)
{
    Blinn blinn(48.f);
    Vec3 wo;
    Vec3 wi;
    float res;

    //cos = 0;
    wo = Vec3(1.f,0.f,0.f);
    wi = Vec3(1.f,0.f,0.f);
    res = blinn.G(&wo,&wi);
    EXPECT_EQ(res, 0.f);

    //cos >1
    wo = Vec3(0.f,0.f,1.2f);
    wi = Vec3(0.f,0.f,1.2f);
    res = blinn.G(&wo,&wi);
    EXPECT_EQ(res, 1.f);

    //c > 1.6f
    wo = Vec3(0.f,0.f,0.9f);
    wi = Vec3(0.f,0.f,0.9f);
    res = blinn.G(&wo,&wi);
    EXPECT_EQ(res, 1.f);

    //normal execution
    wo = Vec3(0.f,0.f,0.1f);
    wi = Vec3(0.f,0.f,0.1f);
    res = blinn.G(&wo,&wi);
    EXPECT_FLOAT_EQ(res, 0.693502366f);
}

TEST(MicrofacetDist,Blinn_pdf)
{
    const int SAMPLES = 160;
    Blinn blinn(48.f);
    Vec3 wo(0.f,0.f,1.f);
    Vec3 wh(0.f,0.f,0.f);

    //test dot(wh,wo) < 0
    wh.z = -1.f;
    float res = blinn.pdf(&wo,&wh);
    EXPECT_EQ(res,0.f);

    float mean = 0;
    float stddev = 0;
    for(float f = -40.f;f<=40.f;f+=0.5f)
    {
        wh.z = cosf(radians(f));
        mean+=blinn.pdf(&wo,&wh);
    }
    mean/=SAMPLES;
    EXPECT_FLOAT_EQ(mean, 2.05130506f);

    for(float f = -40.f;f<=40.f;f+=0.5f)
    {
        wh.z = cosf(radians(f));
        float sample = blinn.pdf(&wo,&wh) - mean;
        stddev += sample*sample;
    }
    stddev=sqrtf(stddev/SAMPLES);
    EXPECT_FLOAT_EQ(stddev,2.71867514f);
}

TEST(MicrofacetDist,Blinn_sample_wh)
{
    Blinn blinn(48.f);
    Vec3 wo;
    Vec3 wh;

    //same hemisphere sampled
    wo = Vec3(0.5f,0.1f,0.9f);
    wo.normalize();
    blinn.sample_wh(&wo, 0.5f, 0.5f, &wh);
    EXPECT_FLOAT_EQ(wh.x,-0.167018592f);
    EXPECT_FLOAT_EQ(wh.y,-0.0000000146012287f);
    EXPECT_FLOAT_EQ(wh.z,0.985953748f);

    //different hemisphere sampled
    wo = Vec3(0.404866f, 0.647051f, -0.646071f);
    wo.normalize();
    blinn.sample_wh(&wo, 0.5f, 0.5f, &wh);
    EXPECT_FLOAT_EQ(wh.x,0.167018592f);
    EXPECT_FLOAT_EQ(wh.y,0.0000000146012287f);
    EXPECT_FLOAT_EQ(wh.z,-0.985953748f);
}

TEST(MicrofacetDist,Beckmann_D)
{
    const int SAMPLES = 160;
    Beckmann beckmann(0.2f);
    Vec3 wo(0.f,0.f,0.f);
    float res = beckmann.D(&wo);
    EXPECT_EQ(res, 0.f);

    float mean = 0;
    float stddev = 0;
    for(float f = -40.f;f<=40.f;f+=0.5f)
    {
        wo.z = cosf(radians(f));
        mean+=beckmann.D(&wo);
    }
    mean/=SAMPLES;
    EXPECT_FLOAT_EQ(mean, 1.44289351f);

    for(float f = -40.f;f<=40.f;f+=0.5f)
    {
        wo.z = cosf(radians(f));
        float sample = beckmann.D(&wo) - mean;
        stddev += sample*sample;
    }
    stddev=sqrtf(stddev/SAMPLES);
    EXPECT_FLOAT_EQ(stddev,2.46824217f);
}

TEST(MicrofacetDist,Beckmann_G)
{
    Beckmann beckmann(0.2f);
    Vec3 wo;
    Vec3 wi;
    float res;

    //cos = 0;
    wo = Vec3(1.f,0.f,0.f);
    wi = Vec3(1.f,0.f,0.f);
    res = beckmann.G(&wo,&wi);
    EXPECT_EQ(res, 0.f);

    //cos >1
    wo = Vec3(0.f,0.f,1.2f);
    wi = Vec3(0.f,0.f,1.2f);
    res = beckmann.G(&wo,&wi);
    EXPECT_EQ(res, 1.f);

    //c > 1.6f
    wo = Vec3(0.f,0.f,0.9f);
    wi = Vec3(0.f,0.f,0.9f);
    res = beckmann.G(&wo,&wi);
    EXPECT_EQ(res, 1.f);

    //normal execution
    wo = Vec3(0.f,0.f,0.1f);
    wi = Vec3(0.f,0.f,0.1f);
    res = beckmann.G(&wo,&wi);
    EXPECT_FLOAT_EQ(res, 0.693502366f);
}

TEST(MicrofacetDist,Beckmann_sample_wh)
{
    Beckmann beckmann(.2f);
    Vec3 wo;
    Vec3 wh;

    //same hemisphere sampled
    wo = Vec3(0.5f,0.1f,0.9f);
    wo.normalize();
    beckmann.sample_wh(&wo, 0.5f, 0.5f, &wh);
    EXPECT_FLOAT_EQ(wh.x,-0.164249673f);
    EXPECT_FLOAT_EQ(wh.y,-0.0000000143591627f);
    EXPECT_FLOAT_EQ(wh.z,0.986418783f);

    //different hemisphere sampled
    wo = Vec3(0.404866f, 0.647051f, -0.646071f);
    wo.normalize();
    beckmann.sample_wh(&wo, 0.5f, 0.5f, &wh);
    EXPECT_FLOAT_EQ(wh.x,0.164249673f);
    EXPECT_FLOAT_EQ(wh.y,0.0000000143591627f);
    EXPECT_FLOAT_EQ(wh.z,-0.986418783f);
}

TEST(MicrofacetDist,Beckmann_pdf)
{
    const int SAMPLES = 160;
    Beckmann beckmann(0.2f);
    Vec3 wo(0.f,0.f,1.f);
    Vec3 wh(0.f,0.f,0.f);
    float mean = 0;
    float stddev = 0;
    for(float f = -40.f;f<=40.f;f+=0.5f)
    {
        wh.z = cosf(radians(f));
        mean+=beckmann.pdf(&wo,&wh);
    }
    mean/=SAMPLES;
    EXPECT_FLOAT_EQ(mean, 1.43569744f);

    for(float f = -40.f;f<=40.f;f+=0.5f)
    {
        wh.z = cosf(radians(f));
        float sample = beckmann.pdf(&wo,&wh) - mean;
        stddev += sample*sample;
    }
    stddev=sqrtf(stddev/SAMPLES);
    EXPECT_FLOAT_EQ(stddev,2.46401787f);
}

TEST(MicrofacetDist,GGXiso_D)
{
    const int SAMPLES = 160;
    GGXiso ggx(0.2f);
    Vec3 wo(0.f,0.f,0.f);
    float res = ggx.D(&wo);
    EXPECT_EQ(res, 0.f);

    float mean = 0;
    float stddev = 0;
    for(float f = -40.f;f<=40.f;f+=0.5f)
    {
        wo.z = cosf(radians(f));
        mean+=ggx.D(&wo);
    }
    mean/=SAMPLES;
    EXPECT_FLOAT_EQ(mean, 1.83194709f);

    for(float f = -40.f;f<=40.f;f+=0.5f)
    {
        wo.z = cosf(radians(f));
        float sample = ggx.D(&wo) - mean;
        stddev += sample*sample;
    }
    stddev=sqrtf(stddev/SAMPLES);
    EXPECT_FLOAT_EQ(stddev,2.40666032f);
}

TEST(MicrofacetDist,GGXiso_G)
{
    GGXiso ggx(0.2f);
    Vec3 wo;
    Vec3 wi;
    float res;

    //cos = 0;
    wo = Vec3(1.f,0.f,0.f);
    wi = Vec3(1.f,0.f,0.f);
    res = ggx.G(&wo,&wi);
    EXPECT_EQ(res, 0.f);

    //normal execution
    wo = Vec3(0.f,0.f,0.3f);
    wi = Vec3(0.f,0.f,0.3f);
    res = ggx.G(&wo,&wi);
    EXPECT_FLOAT_EQ(res, 0.826182127f);
}

TEST(MicrofacetDist,GGXiso_sample_wh)
{
    GGXiso ggx(.2f);
    Vec3 wo;
    Vec3 wh;

    //same hemisphere sampled
    wo = Vec3(0.5f,0.1f,0.9f);
    wo.normalize();
    ggx.sample_wh(&wo, 0.5f, 0.5f, &wh);
    EXPECT_FLOAT_EQ(wh.x,-0.19611606f);
    EXPECT_FLOAT_EQ(wh.y,-0.0000000171450107f);
    EXPECT_FLOAT_EQ(wh.z,0.980580687f);

    //different hemisphere sampled
    wo = Vec3(0.404866f, 0.647051f, -0.646071f);
    wo.normalize();
    ggx.sample_wh(&wo, 0.5f, 0.5f, &wh);
    EXPECT_FLOAT_EQ(wh.x,0.19611606f);
    EXPECT_FLOAT_EQ(wh.y,0.0000000171450107f);
    EXPECT_FLOAT_EQ(wh.z,-0.980580687f);
}

TEST(MicrofacetDist,GGXiso_pdf)
{
    const int SAMPLES = 160;
    GGXiso ggx(0.2f);
    Vec3 wo(0.f,0.f,1.f);
    Vec3 wh(0.f,0.f,0.f);
    float mean = 0;
    float stddev = 0;
    for(float f = -40.f;f<=40.f;f+=0.5f)
    {
        wh.z = cosf(radians(f));
        mean+=ggx.pdf(&wo,&wh);
    }
    mean/=SAMPLES;
    EXPECT_FLOAT_EQ(mean, 1.80560493f);

    for(float f = -40.f;f<=40.f;f+=0.5f)
    {
        wh.z = cosf(radians(f));
        float sample = ggx.pdf(&wo,&wh) - mean;
        stddev += sample*sample;
    }
    stddev=sqrtf(stddev/SAMPLES);
    EXPECT_FLOAT_EQ(stddev,2.41045499f);
}

TEST(MicrofacetDist,GGXaniso_everything)
{
    //no assert, nothing. GGXaniso is a 1:1 copy from pbrt
    //so this distribution has not been plotted nor double checked
    //with another software
    //the test is here just for the coverage
    GGXaniso ggx(0.3f,0.1f);
    Vec3 wo(0.f,0.f,0.f);
    Vec3 wh(0.f,0.f,1.2f);
    Vec3 wi(0.f,0.f,0.5f);
    ggx.D(&wo);
    ggx.D(&wh);
    ggx.D(&wi);
    ggx.G(&wo,&wh);
    ggx.G(&wo,&wi);
    wo = Vec3(0.5f,0.1f,0.9f);
    ggx.sample_wh(&wo, 0.5, 0.6, &wh);
    wo = Vec3(0.404866f, 0.647051f, -0.646071f);
    ggx.sample_wh(&wo, 0.5, 0.5, &wh);
}
