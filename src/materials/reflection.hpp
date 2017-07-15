//Created,  10 Jun 2017
//Last Edit 14 Jul 2017

/**
 *  \file reflection.hpp
 *  \brief Specular reflective BRDF
 *  \author    Davide Pizzolotto
 *  \version   0.1
 *  \date      14 Jul 2017
 *  \copyright GNU GPLv3
 */


#ifndef __REFLECTION_HPP__
#define __REFLECTION_HPP__

#include "materials/bdf.hpp"
#include "materials/fresnel_conditions.hpp"
#include "utility/color.hpp"
#include "geometry/vec3.hpp"

/**
 *  \class Reflection reflection.hpp "materials/reflection.hpp"
 *  \brief BRDF for a smooth mirror-like reflection
 *
 *  The Reflection class is a BRDF that models a smooth surface, exhibiting
 *  specular, mirror-like reflection.
 *  To exhibit such an effect, every light beam is reflected with the same
 *  angle
 */
class Reflection : public Bdf
{
public:

    /** \brief Default constructor
     *
     *  \param specular The spectrum of light reflected back
     *  \param etai Index of refraction from the incoming material
     *  \param etat Index of refraction of the transmitted material
     */
    Reflection(Color specular, float etai, float etat);

    /** \brief Default Constructor for metallic material
     *
     *  \param[in] specular The spectrum of light reflected back
     *  \param[in] refraction The ior for the material. In a metallic material,
     *  index of refraction is different based on the wavelenght of light
     *  \param[in] absorption The amount of light absorbed by the material
     */
    Reflection(Color specular, Color refraction, Color absorption);

    ///Default destructor
    ~Reflection();

    /** \brief Copy the BRDF
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
     *  Since specular reflection exhist only for a single pair of ray, the
     *  BRDF follows a delta distribution, and this function returns always 0
     *
     *  \param[in] wo outgoing ray
     *  \param[in] wiincident ray
     *  \return 0
     */
    Color df(const Vec3* wo, const Vec3* wi)const;

    /** \brief Returns the value of the BRDF
     *
     *  Computes the incident vector, and the value of the BRDF for the pair
     *  of rays
     *
     *  \param[in] wo The outgoing direction
     *  \param[out] wi The incident direction
     *  \param[in] r0 A random float in the interval (0.0,1.0) UNUSED
     *  \param[in] r1 A random float in the interval (0.0,1.0) UNUSED
     *  \param[out] pdf The probability density function of the chosen point
     *  over the bdf hemisphere. In this case 0.0 since this is a delta
     *  distribution
     *  \return The value of the BRDF for the pair of directions
     */
    Color df_s(const Vec3 *wo, Vec3 *wi, float, float, float* pdf)const;

    /** \brief Return the probability density function for this bdf
     *
     *  Given a pair of vectors, return the pdf value for these directions. In
     *  other words the probability that another random sample will be equal to
     *  this one
     *
     *  \param[in] wo The outgoing direction
     *  \param[in] wi The incident direction
     *  \return The pdf for this set of values
     */
    float pdf(const Vec3* wo, const Vec3* wi)const;

private:

    //scattered light
    Color specular;

    //fresnel term
    FresnelConditions* fc;
};


#endif