//Created,   6 May 2017
//Last Edit 30 Mar 2018

/**
 *  \file asset.hpp
 *  \brief     Wrapper of shape, texture and material
 *  \details   Containst the class that merge together a shape with its
 *             materials and textures
 *  \author    Davide Pizzolotto
 *  \version   0.2
 *  \date      30 Mar 2018
 *  \copyright GNU GPLv3
 */


#ifndef __ASSET_HPP__
#define __ASSET_HPP__

#include "primitives/shape.hpp"
#include "materials/bdf.hpp"
#include "materials/material_library.hpp"

/**
 *  \class Asset asset.hpp "primitives/asset.hpp"
 *  \brief A container for shapes, materials and textures
 *
 *  The asset class is a wrapper that merges a Shape, used to define the
 *  structure of the object, a Material, used to define the physical properties
 *  of the object, and a Texture.
 */
class Asset
{
public:

    /** \brief Construct an asset, given the underlying shape
     *
     *  \param[in] sp A pointer to the Shape wrapped in this class
     *  \param[in] transform The transformation matrix that will be applied to
     *  the shape
     *  \param[in] material_no The number of materials used in the Asset
     */
    Asset(const Shape* sp, const Matrix4& transform, unsigned char material_no);

    ///Default destructor
    virtual ~Asset() = default;

    /** \brief Return the ID of this asset
     *
     *  \return A unsigned int representing the ID of this asset
     */
    unsigned int get_id()const;

    /** \brief Intersection of a Ray and this asset
     *
     *  Calls the intersection routine on the underlying shape.
     *  Check Shape::intersect
     *
     *  \param[in] r A pointer to the ray used to perform the intersection
     *  \param[out] distance The distance of the point of intersection
     *  \param[out] h An HitPoint class containing information about the
     *  intersection point
     */
    bool intersect(const Ray* r,float* distance, HitPoint* h)const;

    /** \brief Intersection of a Ray and the bounding box of the asset
     *
     *  Calls the intersection routine on the bounding box of this shape
     *
     *  \param[in] r A pointer to the ray used to perform the intersection
     *  \param[in] rp A pointer to the RayProperties struct, used for fast
     *                intersection
     *  \param[out] entry_point The entry point of the AABB
     *  \param[out] exit_point The exit point of the AABB
     */
    bool intersect_AABB(const Ray* r, const RayProperties* rp,
                       float* entry_point, float* exit_point)const;

    /** \brief Return a pointer to the AABB
     *
     *  Return a constant pointer to the axis aligned bounding box surrounding
     *  this asset
     *
     *  \return A pointer to the aabb surrounding this asset
     */
    const AABB* get_AABB() const;

    /** \brief Return true if this asset is a light
     *
     *  \return true if the asset is a light
     */
    virtual bool is_light()const;
    
    /** \brief Add a material to the set of materials used in this Asset
     *
     *  More than the usage of this function it is important to understand the
     *  reason of it. In particular when considering Meshes where every triangle
     *  has a different material. In this case:
     *  - Storing the material name for every triangle inside the Mesh and then
     *    perform a lookup in the hash map at each intersection would be too
     *    much computationally and memory expensive
     *  - Storing directly a pointer to the material inside each triangle would
     *    be too much memory expensive (8KB every 1K triangles in 64-bits
     *    systems)
     *  Moreover, both the previous solutions break instancing.
     *
     *  These problems are solved by keeping every material used in the Mesh
     *  into an array. Another array stores an unsigned char that acts as an
     *  offset of this array to find the corresponding material. With this
     *  method 1KB of data is used every 1K triangles + the actual size of the
     *  pointers (from 16 byte for a single material mesh up to 2K for mesh with
     *  255 materials)
     *
     *  This method is used to set the material at the ith index to be
     *  referenced by the triangles contained into the mesh. The 0th material
     *  should be the Default material since every Triangle without a material
     *  is assigned the index 0.
     *
     *  \warning This method does not allocate a bigger array if the index is
     *  outside the referenced area. The parser should take care of not writing
     *  outside the array bounds
     *
     *  \param[in] index The index at which the material is assigned. Index 0
     *  represents the default material
     *  \param[in] material A pointer to the material assigned to the given
     *  index
     */
    void set_material(const Bsdf* material, unsigned char index);
    
    /** \brief Return a pointer to the material
     *
     *  Return the material associated with the asset or the particular triangle
     *
     *  \param[in] index The ith face of the mesh for which the material should
     *  be looked up. 0 Returns the default material
     *  \return material A pointer to the material of the asset
     */
    const Bsdf* get_material(unsigned int index)const;

protected:

    ///Underlying model
    const Shape* model;

    ///ObjToWorld matrix
    Matrix4 objToWorld;

    ///WorldToObj matrix
    Matrix4 worldToObj;

private:

    //id of the asset
    const unsigned int id;

    //Bounding box
    AABB aabb;

    //Materials
    const Bsdf** materials;
    
    //index associating every face to every material
    unsigned char* material_index;

};

#endif
