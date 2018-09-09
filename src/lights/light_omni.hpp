//Created,   9 Sep 2018
//Last Edit  9 Sep 2018

/**
 *  \file light.hpp
 *  \brief     Point emitting light in every direction
 *  \author    Davide Pizzolotto
 *  \version   0.1
 *  \date      9 Sep 2018
 *  \copyright GNU GPLv3
 */


#ifndef __LIGHT_OMNI_HPP__
#define __LIGHT_OMNI_HPP__


#include "light.hpp"
#include "geometry/matrix4.hpp"

/**
 *  \brief Point emitting light in every direction
 *
 *  The LightOmni class provides an implementation of a point emitting light
 *  in every direction. This type of light is an infinitely small point and
 *  thus it won't be rendered.
 *  This light is always positioned at (0,0,0) in world, the Matrix4 passed
 *  to its constructor is used to translate it. Note that being the light
 *  infinitely small, both rotation and scaling are useless.
 */
class LightOmni : public Light
{
public:

    /**
     *  \brief Default constructor
     *  \param[in] intensity The spectrum representing the intensity of the
     *  light
     *  \param[in] transform Transform Matrix used to place the light, which
     *  origin is (0,0,0) into the scene in world space.
     */
    LightOmni(Spectrum intensity, const Matrix4& transform);

    /** \brief Return the radiance for a random ray
     *
     *  Sample a random ray from the light and return the emitted radiance
     *  originated from that point
     *
     *  \param[in] r0 A random number in the interval (0.0,1.0)
     *  \param[in] r1 A random number in the interval (0.0,1.0)
     *  \param[in] r2 A random number in the interval (0.0,1.0)
     *  \param[in] r3 A random number in the interval (0.0,1.0)
     *  \param[out] out The computed ray originating the radiance, in world
     *  space units
     *  \param[out] pdf The probability density function of the chosen point
     *  over the light
     *  \return The emitted radiance from the computed ray
     *  \sa sample_visible_surface
     *  \sa pdf(const Ray* ray)const
     */
    Spectrum sample_surface(float r0, float r1, float r2, float r3, Ray* out,
                            float* pdf) const override;

    /** \brief Generate the incident vector and return the radiance
     *
     *  Given a point in the scene, compute the incident direction pointing
     *  towards the light and compute the radiance arriving at that point.
     *  The point sampled on the light is randomly chosen on the visible
     *  surface.
     *  This method assume that there are no occluding objects between the light
     *  and the point
     *
     *  \param[in] r0 UNUSED
     *  \param[in] r1 UNUSED
     *  \param[in] position The current position of the viewer, in other words
     *  the point receiving radiance from this light
     *  \param[out] wi The computed incident direction
     *  \param[out] pdf 1.f, since this method generates the only vector
     *  pointing to the light
     *  \param[out] distance The distance of the light from the position
     *  \return The emitted radiance from the computed ray
     *  \sa sample_surface
     *  \sa pdf(const Point3* p, const Vec3* wi)const
     */
    Spectrum
    sample_visible_surface(float r0, float r1, const Point3* position, Vec3* wi,
                           float* pdf, float* distance) const override;

    /** \brief Return the probability density function for this light
     *
     *  This pdf is correct for rays generated by the sample_surface method.
     *
     *  \param[in] ray A ray generated with the sample_surface method,
     *  representing a random ray originating from the light
     *  \return The pdf for this light
     *  \sa sample_surface()
     */
    float pdf(const Ray* ray) const override;

    /** \brief Return the probability density function for this light
     *
     *  This pdf is correct for rays generated by the sample_visible_surface
     *  method.
     *  Since there is no chance that a random vector will hit the light which
     *  is infinitely small, the pdf of sample_visible_surface is 0.
     *
     *  \param[in] p UNUSED
     *  \param[in] wi UNUSED
     */
    float pdf(const Point3* p, const Vec3* wi) const override;

    /**
     *  \brief Returns true if the light can be rendered
     *
     *  This method returns true if the light can be seen while rendering
     *  (an example of this are LightArea, while LightOmni are an example of
     *  not renderable lights, since they are infinitely small)
     *
     *  \return true if the light can be rendered. false otherwise
     */
    bool renderable() const override;

private:

    ///Position of the omni light
    Point3 light_position;
};

#endif
