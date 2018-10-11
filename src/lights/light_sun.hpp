//Created,   4 Oct 2018
//Last Edit 11 Oct 2018

/**
 *  \file light_sun.hpp
 *  \brief     Directional light at infinity, like the sun
 *  \author    Davide Pizzolotto
 *  \version   0.2
 *  \date      11 Oct 2018
 *  \copyright GNU GPLv3
 */


#ifndef __LIGHT_SUN_HPP__
#define __LIGHT_SUN_HPP__

#include "lights/light.hpp"
#include "utility/date.hpp"
#include "utility/spectrum.hpp"
#include "geometry/point3.hpp"
#include "geometry/ray.hpp"


/**
 *  \brief Class implementing the sun lighting
 *
 *  This class implements a light useful to simulate the sun. This kind of light
 *  can be seen as a spotlight infinitely far away, that does not lose power
 *  based on its distance (otherwise the power would be zero). Thus the light
 *  beams arrive with the same direction to every point of the scene.
 */
class LightSun : public Light
{
public:

    /**
     *  \brief Constructor for the sun, given time and position
     *
     *  Given a Date between 2000BC and 6000AD, an hour and the position of the
     *  camera, this function calculates the correct position of the sun in the
     *  sky
     *
     *  \param[in] intensity Spectrum of the emitted light
     *  \param[in] position The viewer (camera) position
     *  \param[in] world_rad The radius of the scene
     *  \param[in] time A Date object representing the desired instant of time
     *  \param[in] latitude The latitude (+ North, - South) of the camera
     *  position
     *  \param[in] longitude The longitude (+ East, - West) of the camera
     *  position
     *  \param[in] elevation The altitude at which the user is
     *  \sa Scene::radius()
     */
    LightSun(const Spectrum& intensity, const Point3* position,
             float world_rad, Date time, float latitude,
             float longitude, float elevation = 0.01);

    /**
     *  \brief Constructor for the sun, given ray direction
     *
     *  Construct a sunlight given the position of the sun (source) and the
     *  direction of the sunrays. This is done to simplify sunlight construction
     *  in case of non-physical system, if possible use the other constructor.
     *
     *  \param[in] intensity Spectrum of the emitted light
     *  \param[in] sunray Origin and direction of the emitted sunrays
     *  \param[in] world_rad The radius of the scene
     *  \sa Scene::radius()
     */
    LightSun(const Spectrum& intensity, const Ray& sunray, float world_rad);

    /** \brief Return the radiance for a random ray
     *
     *  Sample a random ray from the sun and return the emitted radiance
     *  originated from that point. Since the sun is represented by a single ray
     *  leaving it, always the same ray is returned.
     *
     *  \param[in] r0 UNUSED
     *  \param[in] r1 UNUSED
     *  \param[in] r2 UNUSED
     *  \param[in] r3 UNUSED
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

    /** \brief Generates the incident vector and return the radiance
     *
     *  Given a point in the scene, compute the incident direction pointing
     *  towards the light and compute the radiance arriving at that point.
     *  This method assume that there are no occluding objects between the light
     *  and the point
     *
     *  \param[in] r0 UNUSED
     *  \param[in] r1 UNUSED
     *  \param[in] positionW The current position of the viewer, in other words
     *  the point receiving radiance from this light. In world space
     *  \param[out] wiW The computed incident direction in world space
     *  \param[out] pdf 1.f, since this method generates the only vector
     *  pointing to the light
     *  \param[out] distance The distance of the light from the position. Since
     *  the
     *  sun is placed outside the scene, this will be always greater than two
     *  times the
     *  scene size
     *  \return The emitted radiance from the computed ray
     *  \sa sample_surface
     *  \sa pdf(const Point3* p, const Vec3* wi)const
     */
    Spectrum
    sample_visible_surface(float r0, float r1, const Point3* positionW,
                           Vec3* wiW,
                           float* pdf, float* distance) const override;

    /** \brief Return the probability density function for this light
     *
     *  This pdf is correct for rays generated by the sample_surface method.
     *
     *  \param[in] ray A ray generated with the sample_surface method,
     *  representing a random ray originating from the light
     *  \return 1.f since only one ray is generated from the sun, always with
     *  the same
     *  pdf
     *  \sa sample_surface()
     */
    float pdf(const Ray* ray) const override;

    /** \brief Return the probability density function for this light
     *
     *  This pdf is correct for rays generated by the sample_visible_surface
     *  method.
     *  Since there is no chance that a random vector will hit the light which
     *  is infinitely small and infinitely far away, the pdf of
     *  sample_visible_surface is 0.
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

    /**
     * A ray which origin is the sun position outside the scene
     *  and direction the sunray dir towards the hit point
     */
    Ray lray;

    /** The radius of the scene */
    float radius_w;
};

#endif
