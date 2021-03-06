//author: Davide Pizzolotto
//license: GNU GPLv3

#include "sampler.hpp"

Sampler::Sampler(int startx, int endx, int starty, int endy, int spp,
                 const unsigned int* seed):rng(seed)
{
    Sampler::startx = startx;
    Sampler::endx = endx;
    Sampler::starty = starty;
    Sampler::endy = endy;
    Sampler::spp = spp;
}

void Sampler::get_random_numbers(float* container, int size)
{
    for(int i = 0; i<size; i++)
        container[i] = rng.getNumber();
}
