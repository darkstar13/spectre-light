/*  Created,  18 Jul 2018   */
/*  Last Edit  4 Aug 2018   */

/**
 *  \file png.h
 *  \brief Wrapper for libpng
 *  \details Several functions used to read and write PNG images, with the
 *  aid of libpng
 *  \author Davide Pizzolotto
 *  \version 0.2
 *  \date  4 Aug 2018
 *  \copyright MIT
 */


#ifndef __PNG_H__
#define __PNG_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <png.h>

/**
 *  \brief Writes a png image
 *
 *   Writes a png image with 24 bits per pixel, default compression and no
 *   interlacing or alpha channels
 *
 *  \param[in] name The path on disk where the image will be written.
 *  Existing files will be overwritten
 *  \param[in] width Width of the image
 *  \param[in] height Height of the image
 *  \param[in] data array of 32-bit values containing the image data in
 *  top-down left to right order. The channels for each pixel are in 0xBBGGRRAA
 *  order, with the alpha value always 0xFF.
 *  \return 1 if the image was successfully written, 0 otherwise
 */
char png_write(const char* name, int width, int height, const uint32_t* data);

/**
 *  \brief Checks if a png image is valid
 *
 *  Checks if the header of the file correspond to an actual png image
 *  readable by the functions defined in this file.
 *
 *  \note Only the header is checked, so if the file is truncated in the
 *  image data, this function could return 1 and the read could fail
 *
 *  \param[in] name The path on disk where the image can be found
 *  \return 1 if the image header is a valid png one, 0 otherwise
 */
char png_valid(const char* name);

/**
 *  \brief Returns the width and height of a png image
 *
 *  Checks the header of an image and saves the image width and height in
 *  the pointer passed as input. If the image is not readable or the header
 *  not compatible 0 is returned.
 *
 *  \param[in] name The path on disk where the image can be found
 *  \param[out] width The width of the image
 *  \param[out] height The height of the image
 *  \return 1 if the width and height was successfully read, 0 otherwise
 */
char png_dimensions(const char* name, int* width, int* height);

/**
 *  \brief Reads a png image
 *
 *  Reads a png image and writes the image data on the values array in
 *  top-down left-right order. This array is expected to be already allocated
 *  with a size of width*height. Every channel is written in a single uint32_t
 *  value 0xBBGGRRAA with values in the range [0-255] and alpha value always
 *  0xFF.
 *
 *  \param[in] name The path on disk where the image can be found
 *  \param[out] values The array of values that will be written. This array
 *  must be already allocated, use png_dimensions to know the actual size of
 *  the image and to preallocate it
 *  \return 0 if the read was not successful, 1 otherwise
 */
char png_read(const char* name, uint32_t* values);

#endif
