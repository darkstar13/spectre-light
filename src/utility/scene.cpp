//author: Davide Pizzolotto
//license: GNU GPLv3

#include "scene.hpp"

Scene::~Scene()
{
    std::unordered_map<unsigned int, const Shape*>::const_iterator shape_it;
    for(shape_it = shapes.begin(); shape_it != shapes.end(); shape_it++)
        delete shape_it->second;
    shapes.clear();

    std::unordered_map<unsigned int, const Asset*>::const_iterator asset_it;
    for(asset_it = assets.begin(); asset_it != assets.end(); asset_it++)
        delete asset_it->second;
    assets.clear();

    //DO NOT DELETE individual lights. lights are just pointers owned in, the
    //assets array.
    //The light array is there just for faster lookup
}

void Scene::inherit_shape(const Shape* addme)
{
    //unordered map to check in O(1) if the shape is duplicate upon addition
    shapes.insert(std::make_pair(addme->get_id(), addme));
}

unsigned int Scene::size_shapes() const
{
    return (unsigned int)shapes.size();
}

void Scene::inherit_asset(const Asset* addme)
{
    //unordered map to check in O(1) if the asset is duplicate upon addition
    assets.insert(std::make_pair(addme->get_id(), addme));
    k.addAsset(addme);
}

unsigned int Scene::size_assets() const
{
    return (unsigned int)assets.size();
}

void Scene::inherit_light(const AreaLight* addme)
{
    if(lights.size()<_MAX_LIGHTS_)
    {
        //usually I avoid auto keyword, but this iterator would be > 80 col
        auto insert_res = assets.insert(std::make_pair(addme->get_id(), addme));
        if(insert_res.second) //true if it was added
        {
            lights.push_back(addme);
            k.addAsset(addme);
        }
    }
    else
        Console.warning(MESSAGE_MAXLIGHTSNUMBER, _MAX_LIGHTS_);
}

unsigned int Scene::size_lights() const
{
    return (unsigned int)lights.size();
}

const AreaLight* Scene::get_light(int index) const
{
    return lights[index];
}
