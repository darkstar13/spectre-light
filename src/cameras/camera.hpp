//Created,  24 May 2017
//Last Edit  7 Jun 2018

/**
 *  \file camera.hpp
 *  \brief     Interface for the camera
 *  \details   An abstract class representing a generic camera in the space
 *  \author    Davide Pizzolotto
 *  \version   0.2
 *  \date      7 Jun 2018
 *  \copyright GNU GPLv3
 */


#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "cameras/image_film.hpp"
#include "samplers/sampler.hpp"
#include "geometry/matrix4.hpp"
#include "geometry/ray.hpp"

/**
 * \class Camera camera.hpp "cameras/camera.hpp"
 * \brief An interface for a generic camera
 *
 *  This class represents the implementation of a generic camera positioned
 *  in space. The positioning, referred as "camera to world space" is done
 *  with the LookAt matrix.
 *
 *  This class exposes a method called create_ray, that must be overriden by
 *  the different types of camera that will be implemented. This method will
 *  generate the light ray that reached a specific point in the image plane
 */
class Camera
{
public:

    /** \brief Default constructor
     *
     *  \param[in] position The position of the camera in world space
     *  \param[in] target The target of the camera in world space
     *  \param[in] up A normalized vector that represents the up direction of
     *  the camera
     *  \param[in] width The width of the final image (resolution)
     *  \param[in] height The height of the final image (resolution)
     */
    Camera(const Point3* position, const Point3* target, const Vec3* up,
           int width, int height);

    ///Default destructor
    virtual ~Camera() = default;

    /** \brief Create a ray from a sample
     *
     *  Given a sample, generated by a sampler, in raster space, this methods
     *  creates a ray in world space
     *
     *  \param[in] sample The given sample
     *  \param[out] ray The output ray
     */
    void create_ray(const Sample* sample, Ray* ray) const;

    /** \brief Create a ray from a sample
     *
     *  Given a sample, generated by a sampler, in raster space, this methods
     *  creates a ray in world space. This method initializes also the
     *  rays offsetted by 1 pixel.
     *
     *  \note Every camera subclass has a create_ray method with improved
     *  efficiency and the same result. This is left only as fallback
     *  implementation
     *
     *  \param[in] sample The given sample
     *  \param[out] ray The output ray
     *  \param[out] rx The output ray offsetted by 1 pixel in the x direction
     *  \param[out] ry The output ray offsetted by 1 pixel in the y direction
     */
    virtual void create_ray(const Sample* sample, Ray* ray, Ray* rx, Ray* ry)
    const;

protected:

    /**  \brief Create a ray from a sample
     *
     *  Exactly like the create_ray method, but instead of initializing a
     *  ray and its offsetted values, this method generates the origin and
     *  direction for the ray in the single point. The create_ray() method will
     *  call this one to generate rays for the specific camera types
     */
    virtual void create_single_ray(const Sample* sample, Point3* orig,
                                   Vec3* dir) const = 0;

    ///Transformation matrix used to transform from camera space to world space
    Matrix4 camera2world;

    ///Offset used for the ray offsetted by 1 pixel in the x direction
    Vec3 dx;

    ///Offset used for the ray offsetted by 1 pixel in the y direction
    Vec3 dy;
};

#endif
