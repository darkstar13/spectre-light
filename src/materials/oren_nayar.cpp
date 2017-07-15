#include "oren_nayar.hpp"

OrenNayar::OrenNayar(Color diffuse, float sigma) : Bdf(BdfFlags(BRDF|DIFFUSE)),
                                                   diffuse(diffuse)
{
    float sigma2 = sigma*sigma;
    OrenNayar::A = 1.f - (sigma2/(2*(sigma2+0.33f)));
    OrenNayar::B = (0.45f*sigma2)/(sigma2+0.09f);
}

Bdf* OrenNayar::clone()const
{
    OrenNayar* res = new OrenNayar(*this);
    return res;
}

Color OrenNayar::df(const Vec3 *wout, const Vec3 *wincident) const
{
    float costhetain = wincident->z;
    float costhetaout = wout->z;
    float abscosthetain = fabsf(costhetain);
    float abscosthetaout = fabsf(costhetaout);
    float sinthetain = sqrtf(1.f-costhetain*costhetain);
    float sinthetaout = sqrtf(1.f-costhetaout*costhetaout);

    //cos(phiin - phiout) = cos(phiin)*cos(phiout)+sin(phiin)*sin(phiout)
    float maxcos = 0.0f;
    if(sinthetain>0 && sinthetaout>0) //cos(phiin - phiout) positive
    {
        float cosphiin, cosphiout, sinphiin, sinphiout;
        cosphiin = sinthetain==0.f?1.f:clamp(wincident->x/sinthetain,-1.f,1.f);
        cosphiout = sinthetaout==0.f?1.f:clamp(wout->x/sinthetaout,-1.f,1.f);
        sinphiin = sqrtf(1.f - cosphiin * cosphiin);
        sinphiout = sqrtf(1.f - cosphiout * cosphiout);
        maxcos = max(0.0f, cosphiin * cosphiout + sinphiin * sinphiout);
    }

    float sinalpha, tanbeta;
    if(abscosthetain>abscosthetaout)
    {
        sinalpha = sinthetaout;
        tanbeta = sinthetain/abscosthetain;
    }
    else
    {
        sinalpha = sinthetain;
        tanbeta = sinthetaout/abscosthetaout;
    }

    //diffuse/pi * (A+B*max[0,cos(phiin-phiout)]*sinalpha*tanbeta)
    return OrenNayar::diffuse * INV_PI * (A+B*maxcos*sinalpha*tanbeta);
}