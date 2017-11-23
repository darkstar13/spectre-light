# spectre-light
>Yet another photorealistic renderer with great ambitions
[![Build status](https://travis-ci.org/davidepi/spectre-light.svg?branch=master)](https://travis-ci.org/davidepi/spectre-light)
[![Build status](https://ci.appveyor.com/api/projects/status/d4bx9kjo42nnpfy5/branch/master?svg=true)](https://ci.appveyor.com/project/darkstar13/spectre-light/branch/master)

spectre-light is a photorealistic unbiased physically-based renderer. It inherits its base
structure from the great [pbrt](http://pbrt.org "pbrt homepage") by M.Pharr, W. Jakob and G.
Humphreys, but it is slowly diverging from it. The long term goal of this project is a
GPU-based physically based renderer, however, in order to reach that point, firstly a solid CPU
renderer will be created and then GPU support will be added.

The road ahead is long and for this reason the file [Roadmap.md](./Roadmap.md) contains the
list of features that will be implemented during the way)

## Getting started

These instruction will get a running copy of spectre-light in your machine in the fastest
possible way.

spectre-light uses [cmake](https://cmake.org "cmake homepage") as a build system, so this
is the only mandatory dependency. This tool is available in almost every package manager
if you are running a Linux distribution, as well as in `brew` if you are running macOS.
Furthermore, you can download the Windows or macOS installer of cmake from the official
site

### Building

Building spectre-light works exactly like any other cmake project, just be sure to pass the
`-DCMAKE_BUILD_TYPE="Release"` in order to enable every possible compiler optimization.
Here are the instruction step by step, assuming cmake 3.1+ is already installed

#### Linux and macOS
```
mkdir build/
cd build
cmake -DCMAKE_BUILD_TYPE="Release" ..
make
```

#### Windows
```
mkdir build/
cd build
cmake -DCMAKE_BUILD_TYPE="Release" -G "Visual Studio 14 2015 Win64" ..
```
At this point you need to open the generated solution and compile it with visual studio

### Running

After building spectre-light you can run it with the following command
```
spectre input.txt
```
where input is an ASCII file containing the description of the scene that will be rendered and
where the output file will be saved. More information about the input file can be found in
[the input_example.txt file](./input_example.txt)

The output file will be a `.ppm` image, that you can convert in macOSX and Linux using
```
convert out.ppm out.jpg
```
Unfortunately in Windows you have to use an online service that does this for you, however,
implementing a good image support is a key feature of the version 0.2.0

## How it works

You can find a detailed explanation of the whole rendering process inside the src/ folder,
 then for every subfolder there is another readme file explaining the purpose of the component
 you are viewing
 
 ## Documentation
 
 This project uses [Doxygen](http://doxygen.org) in order to generate the documentation.
 If you have doxygen installed, cmake will detect it and produce the target `doc`. After
 that the only thing left to do is to invoke the following command
 ```
 make doc
 ```
 in order to generate the html documentation of the project
 
 ## License
 
 spectre-light is released under the terms of the GNU GPLv3 License. You may find the
 content of the license in the [LICENSE.txt](./LICENSE.txt) file
