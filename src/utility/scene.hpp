//Created,  29 Jun 2017
//Last Edit 21 Apr 2018

/**
 *  \file scene.hpp
 *  \brief     Class enclosing a scene
 *  \details   Agglomerate of Assets composing a scene, and the kdtree to
 *             intersect them
 *  \author    Davide Pizzolotto
 *  \version   0.2
 *  \date      21 Apr 2018
 *  \copyright GNU GPLv3
 */


#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "accelerators/kdtree.hpp"
#include "primitives/asset.hpp"
#include "geometry/matrix4.hpp"
#include "utility/utility.hpp"
#include "utility/console.hpp"
#include "lights/area_light.hpp"
#include <unordered_map>

/**
 *  \class Scene scene.hpp "utility/scene.hpp"
 *  \brief Implementation of a collection of shapes
 *
 *  The scene class is a collection of shapes representing the scene that will
 *  be rendered. This is composed by Asset classes and a KdTree class used for
 *  fast intersection.
 *
 *
 *  \warning The scene class is responsible for the lifecycle of the shapes and
 *  assets inside it, so the shapes added with the Scene::inherit_shape() are
 *  INHERITED and therefore MUST NOT be deallocated manually
 *
 *  \warning This class is not thread-safe
 *
 *  \note Before using the scene one MUST finalize the tree by calling
 *  KdTree::buildTree()
 */
class Scene
{
public:

    ///Default constructor
    Scene();

    ///Default destructor
    ~Scene();

    ///Preserve rule of three
    Scene& operator=(Scene const&) = delete;

    ///Preserve rule of three
    Scene(Scene const&) = delete;

    ///The KdTree used to perform fast intersections on the scene
    KdTree k;

    /** \brief Add a Shape to the scene
     *
     *  Given a pointer to a shape, the scene inherits its ownership. This
     *  method is useless because the inherited shapes are not used directly
     *  by the Scene, however, ensures that they are deleted by this class
     *  along with the assets when rendering is over
     *
     *  \param[in] addme The shape that will be added to the Scene
     *  \return The added shape id
     */
    unsigned int inherit_shape(Shape* addme);

    /** \brief Return the number of shapes in the scene
     * \return The number of shapes in the scene
     */
    unsigned int shapes_size()const;

    /** \brief Add an Asset to the scene
     *
     *  Given a pointer to an asset, the scene inherits its ownership.
     *  If possible, use shapes already inside this class by calling the
     *  Scene::add_asset function. Use this function only if the Shape wrapped
     *  by the Asset was already added to the scene
     *
     *  \param[in] addme The asset that will be added
     */
    void inherit_asset(Asset* addme);

    /** \brief Return the number of assets in the scene
     * \return The number of assets in the scene
     */
    unsigned int assets_size()const;

    /** \brief Add a Light to the scene
     *
     *  Given a pointer to a Light, the scene inherits its ownership.
     *  If possible, use shapes already inside this class by calling the
     *  Scene::add_light function. Use this function only if the Shape wrapped
     *  by the Light was already added to the scene
     *
     *  \param[in] addme The light that will be added
     */
    void inherit_light(AreaLight* addme);

    /** \brief Return the number of lights in the scene
     * \return The number of lights in the scene
     */
    unsigned int lights_size()const;

    /** \brief  Return the array of lights in the scene
     * \return The array of lights in the scene
     */
    const AreaLight* const* get_lights()const;

private:

    //map of shapes
    std::unordered_map<unsigned int,const Shape*> shapes;

    //Array of asset pointers
    Asset** assets;

    //size of asset pointers array
    unsigned int assets_allocated;

    //next insertion index in the asset pointers array
    unsigned int asset_index;

    //Array of light pointers
    AreaLight** lights;

    //size of light pointers array
    unsigned int lights_allocated;

    //next insertion index in the asset pointers array
    unsigned int light_index;

};

#endif
