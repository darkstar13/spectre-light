#include "reflection.hpp"

Reflection::Reflection(Color specular, Color refraction, Color absorption)
        : Bdf(BdfFlags(BRDF|SPECULAR)),specular(specular)
{
    Reflection::fc = new Conductor(refraction,absorption);
}

Reflection::Reflection(Color specular, float etai, float etat)
        : Bdf(BdfFlags(BRDF|SPECULAR)),specular(specular)
{
    Reflection::fc = new Dielectric(etai,etat);
}

Reflection::~Reflection()
{
    delete Reflection::fc;
}

Color Reflection::df(const Vec3*, const Vec3*) const
{
    return Color();
}

Color Reflection::df_s(const Vec3 *wo, Vec3 *wi, float, float, float* pdf) const
{
    //wi = wo * [-1 0 0 0]
    //          [0 -1 0 0]
    //          [0  0 1 0]
    //          [0  0 0 1]
    wi->x = -wo->x;
    wi->y = -wo->y;
    wi->z = wo->z;
    *pdf = 1.f;
    return fc->eval(wo->z)*specular/fabsf(wo->z);
}

float Reflection::pdf(const Vec3* wo, const Vec3* wi)const
{
    return 1.f;
}

Bdf* Reflection::clone()const
{
    Reflection* res = new Reflection(*this);
    return res;
}