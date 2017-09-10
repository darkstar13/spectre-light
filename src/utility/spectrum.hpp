//Created,  29 Aug 2017
//Last Edit 10 Sep 2017

/**
 *  \file spectrum.hpp
 *  \brief Colours representations using wavelength spectrum
 *  \author Davide Pizzolotto
 *  \version 0.1
 *  \date 10 Sep 2017
 *  \copyright GNU GPLv3
 */


#ifndef __SPECTRUM_HPP__
#define __SPECTRUM_HPP__

#include "utility/color.hpp"
#include "settings.h"

///The wavelenght of the first sample in nanometers
#define SPECTRUM_START 400
///The number of samples
#define SPECTRUM_SAMPLES 16
///The inverse of the number of samples
#define INV_SPECTRUM_SAMPLES (1.f/SPECTRUM_SAMPLES)
///The interval in nanometers between each sample
#define SPECTRUM_INTERVAL 20

class StratifiedSpectrum;

/**
 *  \class Spectrum spectrum.hpp "utility/spectrum.hpp"
 *  \brief The representation of a visibile colour
 *
 *  Spectrum class contains an approximation of the visible spectrum. It 
 *  represents a colour by sampling its EM spectrum. The samples span the range 
 *  400nm to 700nm (included) with an interval of either 20nm or 10nm if the
 *  macro HQ_SPECTRUM is set to 1
 */
class Spectrum
{
public:
    
    ///Default constructor, does nothing
    Spectrum();
    
    /** \brief Initialize the spectrum given the temperature
     *
     *  Consider this spectrum as an emission from a blackbody and initialize it
     *  based on the temperature of the object.
     *  The value 0 is used as an efficient way to indicate a black spectrum.
     *
     *  \param[in] temperature The temperature of the body, in Kelvin degrees
     */
    Spectrum(int temperature);
    
    /** \brief Initialize the spectrum with a constant value
     *
     *  \param[in] val The value of the spectrum
     */
    Spectrum(float val);
    
    /** \brief Initialize the spectrum with the given values
     *
     *  Initialize this class by filling it with the given values
     *
     *  \param[in] vals The values, sorted by nm, used for this spectrum
     */
    Spectrum(const float* vals);
    
    /** \brief Attempt to convert an sRGB colour to a spectrum
     *
     *  Although an exact answer is impossible, this constructor tries to
     *  create a spectrum given an RGB colour. Note that multiple spectra can
     *  have the same RGB colour, so this method being a one-to-many can lead
     *  to unexpected results
     *
     *  \param[in] c The RGB colour that will be converted
     *  \param[in] light true if the colour is a light source
     */
    Spectrum(ColorRGB c, bool light);
    
    /** \brief Convert this spectrum to its XYZ representation
     *
     *  \return The XYZ representation of this spectrum
     */
    ColorXYZ toXYZ()const;
    
    /** \brief Calculate the luminous intensity of this spectrum
     *
     *  Calculate only the Y value of the XYZ color represented by this 
     *  spectrum. This value is used to represent the luminance
     *
     *  \return The luminous intensity of this spectrum
     */
    float luminance()const;
    
    /** \brief Retun true if the spectrum is composed only by 0 values
     * 
     *  \return true if only 0 values compose this spectrum
     */
    bool isBlack()const;
    
#ifdef SPECTRAL
    /** \brief Randomly choose only one component of the spectrum
     *
     *  \param[in] r0 A random number in the range [0.0,1.0]
     *  \param[in] pdf The pdf for the chosen value
     */
    virtual StratifiedSpectrum stratify(float r0, float* pdf)const;
    
    ///Return the contribution of this spectrum. This is useful for subclasses
    virtual Spectrum weight()const;
#endif
    
    ///The addition operation between two spectra
    Spectrum operator+(const Spectrum& s)const;
    ///The addition operation between two spectra
    void operator+=(const Spectrum& s);
    ///The subtraction operation between two spectra
    Spectrum operator-(const Spectrum& s)const;
    ///The subtraction operation between two spectra
    void operator-=(const Spectrum& s);
    ///The multiplication operation between two spectra
    Spectrum operator*(const Spectrum& s)const;
    ///The multiplication operation between two spectra
    void operator*=(const Spectrum& s);
    ///The division operation between two spectra
    Spectrum operator/(const Spectrum& s)const;
    ///The division operation between two spectra
    void operator/=(const Spectrum& s);
    ///The addition operation between a value and a spectrum
    Spectrum operator+(float v)const;
    ///The addition operation between a value and a spectrum
    void operator+=(float v);
    ///The subtraction operation between a value and a spectrum
    Spectrum operator-(float v)const;
    ///The subtraction operation between a value and a spectrum
    void operator-=(float v);
    ///The multiplication operation between a value and a spectrum
    Spectrum operator*(float v)const;
    ///The multiplication operation between a value and a spectrum
    void operator*=(float v);
    ///The division operation between a value and a spectrum
    Spectrum operator/(float v)const;
    ///The division operation between a value and a spectrum
    void operator/=(float v);
    
#ifdef SPECTRAL
    ///Wavelength samples
    float w[SPECTRUM_SAMPLES];
#else
    ///x,y,z components
    float w[3];
#endif
};

#ifdef SPECTRAL
class StratifiedSpectrum : public Spectrum
{
public:
    friend class Spectrum;
    
    /** \brief Randomly choose only one component of the spectrum
     *
     *  Since the StratifiedSpectrum is already stratified, this method
     *  returns *this
     *
     *  \param[in] r0 UNUSED
     *  \param[in] pdf UNUSED
     */
    StratifiedSpectrum stratify(float r0, float* pdf)const;
    
    /** \brief Weight the contribution of this class
     *
     *  In the StratifiedSpectrum class only one component has a meaningful
     *  value. In order to weight correctly this value, when added to other
     *  Spectrum classes, this method returns a Spectrum class where every
     *  value is zero except the stratified one which has a value of one
     */
    Spectrum weight()const;
    
    ///Get the chosen component of this spectrum
    unsigned char getComponent()const;
    
private:
    
    //constructor
    StratifiedSpectrum(float val, unsigned char index);
    
    //index of the only non-zero value
    unsigned char chosen;
};
#endif

///Spectrum of white surfaces
extern const Spectrum SPECTRUM_WHITE;
///Spectrum composed of 0 values
extern const Spectrum SPECTRUM_BLACK;
///Spectrum composed of 1 values
extern const Spectrum SPECTRUM_ONE;

#endif
