#include "bdf.hpp"
Bdf::Bdf(BdfFlags flags)
{
    Bdf::type = flags;
}

Bdf::~Bdf()
{

}

BdfFlags Bdf::getFlags() const
{
    return Bdf::type;
}

Color Bdf::df_s(const Vec3 *wo, Vec3 *wi, float r0, float r1, float* pdf)const
{
    //sample x,y points on the hemisphere, shirley's method maybe's better
    float t = TWO_PI * r0;
    float r = sqrtf(r1);
    wi->x = r * cosf(t);
    wi->y = r * cosf(t);
    wi->z = sqrtf(max(0.f,1.f-wi->x*wi->x-wi->y*wi->y));
    //if the wo was flipped, flip also wi
    if(wo->z < 0) wi->z *= -1.f;
    *pdf = fabsf(wi->z)*INV_PI;
    return df(wo,wi);
}
float Bdf::pdf(const Vec3* wo, const Vec3* wi)const
{
    return fabsf(wi->z)*INV_PI;
}

Bsdf::Bsdf()
{
    Bsdf::count = 0;
}

Bsdf::~Bsdf()
{
    for(int i=0;i<count;i++)
        delete Bsdf::bdfs[i];
}

void Bsdf::addBdf(const Bdf* addme)
{
#ifdef _LOW_LEVEL_CHECKS_
    if(count==_MAX_BDF_)
    {
        Console.severe("Cannot add more Bdfs");
        return;
    }
#endif
    Bsdf::bdfs[count++] = addme->clone();
}

Color Bsdf::df(const Vec3 *wo, const HitPoint* h, const Vec3 *wi)const
{
    Vec3 wo_shading_space(wo->dot(h->right),wo->dot(h->cross),wo->dot(h->n));
    Vec3 wi_shading_space(wi->dot(h->right),wi->dot(h->cross),wi->dot(h->n));
    BdfFlags val;
    if(wi->dot(h->n)*wo->dot(h->n) > 0) //reflected ray
        val = BRDF;
    else                                //transmitted ray
        val = BTDF;
    Color retval;
    for(int i=0;i<count;i++)
    {
        if(bdfs[i]->isType(val)) //add contribution only if matches refl/trans
            retval += bdfs[i]->df(&wo_shading_space,&wi_shading_space);
    }
    return retval;
}

Color Bsdf::df_s(float r0, float r1, float r2, const Vec3* wo,
                 const HitPoint* h, Vec3* wi, float* pdf)const
{
    if(Bsdf::count == 0)
    {
        *pdf = 0.f;
        return Color(); //otherwise it will access invalid array positions
    }
    int chosen = (int)(r0 * count);
    if(chosen==count) //out of array bounds
        chosen--;

    //transform to shading space
    Vec3 wo_shading_space(wo->dot(h->right),wo->dot(h->cross),wo->dot(h->n));
    Vec3 tmpwi;
    //compute sampled bdf value
    Color retval = bdfs[chosen]->df_s(&wo_shading_space,&tmpwi,r1,r2,pdf);

    //transform incident ray to world space
    wi->x = h->right.x*tmpwi.x + h->cross.x * tmpwi.y + h->n.x * tmpwi.z;
    wi->x = h->right.y*tmpwi.x + h->cross.y * tmpwi.y + h->n.y * tmpwi.z;
    wi->x = h->right.z*tmpwi.x + h->cross.z * tmpwi.y + h->n.z * tmpwi.z;

    //if specular return the computed value
    if(bdfs[count]->getFlags() & (BRDF|SPECULAR))
        return retval;

    //else compute the value for the given pair of ray
    BdfFlags val = bdfs[count]->getFlags();
    char matching = 1;
    if(wi->dot(h->n)*wo->dot(h->n) > 0)
        val = (BdfFlags)(val & ~BTDF);
    else
        val = (BdfFlags)(val & ~BRDF);
    for(int i=0;i<count;i++)
    {
        if(bdfs[i]->isType(val)) //add contribution only if matches refl/trans
        {
            retval += bdfs[i]->df(&wo_shading_space,&tmpwi);
            *pdf+=bdfs[i]->pdf(&wo_shading_space,&tmpwi);
            matching++;
        }
    }
    if(matching>1) //most of times this will be 1. And the division is expensive
        *pdf/=matching;
    return retval;
}

float Bsdf::pdf(const Vec3* wo,  const HitPoint* h, const Vec3* wi)const
{
    if(Bsdf::count == 0)
        return 0.f;
    Vec3 wo_shading_space(wo->dot(h->right),wo->dot(h->cross),wo->dot(h->n));
    float pdf = 0.f;
    int matching = 0;
    for (int i = 0; i < count; ++i)
    {
        matching++;
        pdf += bdfs[i]->pdf(wo, wi);
    }
    if(matching>0)
        return pdf/(float)matching;
    else
        return 0.f;
}
