//created 15-10
#ifndef __LIGHT_SKY_HPP__
#define __LIGHT_SKY_HPP__

#include "samplers/distributions.hpp"
#include "textures/texture_image.hpp"
#include "lights/light.hpp"
#include "textures/texture.hpp"
#include "geometry/matrix4.hpp"
#include "geometry/point3.hpp"
#include "geometry/ray.hpp"
#include "geometry/vec3.hpp"
#include "utility/spectrum.hpp"

class LightSky : public Light
{
public:

    /**
     *  \brief Creates the skydome
     *
     *  \param[in] tex The texture that will be used by the dome
     *  \param[in] scene_centre The central point of the scene (height from
     *  horizon will be ignored)
     *  \param[in] world_rad The radius of the scene
     */
    LightSky(const TextureImage* tex, const Point3& scene_centre,
             float world_rad);

    ~LightSky();

    LightSky(LightSky&) = delete;

    Spectrum sample_surface(float r0, float r1, float r2, float r3, Ray* out,
                            float* pdf) const override;

    Spectrum
    sample_visible_surface(float r0, float r1, const Point3* position, Vec3* wi,
                           float* pdf, float* distance) const override;

    float pdf(const Ray* ray) const override;

    float pdf(const Point3* p, const Vec3* wiW) const override;

    bool renderable() const override;

    /**
     *  \brief Returns the radiance of a ray that didn't hit any asset
     *
     *  A sky needs to collect every ray that escapes the scene and return the
     *  correct value for the hit on the skydome. This function, given a ray
     *  that escaped the scene, returns the Spectrum obtained from the skydome
     *  texture.
     *  Note that it is not checked if the ray really escaped the scene.
     *
     *  \param[in] ray The ray that will be used to deterimine the radiance
     *  \return The Spectrum of the radiance for the escaped ray
     */
    Spectrum radiance_escaped(const Ray* ray) const;

private:
    const TextureImage* skytexture;
    Distribution2D* distribution;
    Matrix4 world2light;
    Matrix4 light2world;
};

#endif
