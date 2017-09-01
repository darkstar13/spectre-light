//Created,  10 Jun 2017
//Last Edit  8 Aug 2017

/**
 *  \file refraction.hpp
 *  \brief Implementation of a BTDF
 *  \author    Davide Pizzolotto
 *  \version   0.1
 *  \date      8 Aug 2017
 *  \copyright GNU GPLv3
 */


#ifndef __REFRACTION_HPP__
#define __REFRACTION_HPP__

#include "materials/bdf.hpp"
#include "utility/color.hpp"
#include "utility/spectrum.hpp"
#include "geometry/vec3.hpp"
#include "materials/fresnel_conditions.hpp"

/**
 *  \class Refraction refraction.hpp "materials/refraction.hpp"
 *  \brief BTDF for a smooth surface
 *
 *  The Refraction class is a BTDF that models a smooth surface. This class is
 *  used to calculate the transmission of light inside an object
 */
class Refraction : public Bdf
{
public:

    /** \brief Default constructor
     *
     *  \param[in] specular Scale factor for transmitted light
     *  \param[in] eta_incident Refraction index for the incident material
     *  \param[in] eta_transmitted Refraction index for the transmitted material
     */
    Refraction(const Spectrum& specular, float eta_incident,
               float eta_transmitted);

    /** \brief Copy the BTDF
     *
     *  Method used to copy this class
     *
     *  \warning The returned Bdf is heap allocated, and must be deallocated.
     *  Although this is really bad practice, it is the only possible
     *  implementation without using reference counting.
     *
     *  \return an heap allocated base pointer of the cloned class
     */
    Bdf* clone()const;

    /** \brief NOOP
     *
     *  Refraction follows a delta distribution, so  this function returns
     *  always 0
     *
     *  \param[in] wo outgoing ray
     *  \param[in] wi incident ray
     *  \return 0
     */
    Spectrum df(const Vec3* wo, const Vec3* wi)const;

    /** \brief Returns the value of the BTDF
     *
     *  Computes the transmitted vector, and the value of the BTDF for the pair
     *  of rays
     *
     *  \param[in] wo The outgoing direction
     *  \param[out] wi The incident direction
     *  \param[in] r0 A random float in the interval (0.0,1.0) UNUSED
     *  \param[in] r1 A random float in the interval (0.0,1.0) UNUSED
     *  \param[out] pdf The probability density function of the chosen point
     *  over the bdf hemisphere. Since the only valid pair of direction is
     *  generated, this method returns 1.0 as pdf
     *  \return The value of the BTDF
     */
    Spectrum df_s(const Vec3 *wo, Vec3 *wi, float r0, float r1,
                  float* pdf)const;

    /** \brief Return the probability density function for this bdf
     *
     *  Given a pair of vectors, return the pdf value for these directions. In
     *  other words the probability that another random sample will be equal to
     *  this one. Since this is a delta distribution, the probability that an
     *  arbitrary pair of direction is exactly equal to the specular one is 0.0f
     *
     *  \param[in] wo The outgoing direction
     *  \param[in] wi The incident direction
     *  \return 0
     */
    float pdf(const Vec3* wo, const Vec3* wi)const;

private:

    //scale factor
    Spectrum specular;

    //incident ior
    float eta_i;

    //transmitted ior
    float eta_t;

    //fresnel term
    Dielectric d;
};

#endif
