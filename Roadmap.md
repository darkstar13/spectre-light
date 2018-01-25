### V0.1 - FOUNDATIONS ###
- [x] basic geometric framework (vectors, matrices, rays)
- [x] spheres, boxes sdl
- [x] kd-tree accelerator
- [x] cameras (orthographic, perspective, 360 panorama)
- [x] samplers (stratified and pure random)
- [x] filters (box, tent, gaussian, mitchell, lanczos)
- [x] basic materials (lambert/oren-nayar, perfect reflection/refraction)
- [x] area lights
- [x] direct lighting tracer
- [x] path tracer
- [x] simple parser
- [x] triangled meshes
- [x] bvh aac accelerator
- [x] spectrum (rgb and spectral rendering, blackbody radiation)
- [ ] ~dispersion~ **POSTPONED**
- [ ] ~bidirectional path tracer~ **POSTPONED**
- [x] microfacet reflection and transmission (blinn, beckmann, ggx isotropic, ggx anisotropic)

### V0.2 ###
- [x] unit testing
- [ ] -> **extended image support (.jpg/.tiff support, metadata)** <-
- [ ] lex/yacc parser (.3ds parsing, .mtl parsing, quad/ngon parsing)
- [ ] diffuse texture mapping
- [ ] bump/normal textures
- [ ] environment (sky) system
- [ ] sun position and sunlight
- [ ] camera lenses
- [ ] depth of field
- [ ] bidirectional path tracer
- [ ] metropolis light transport
- [ ] irradiance caching

### V0.3 ###
- bvh exporter/importer/generator
- texture caching
- memory checks
- memory manager
- load and unload models from memory
- better occlusion testing
- ad hoc intersection methods for occlusion testing
- tessellation and displacement mapping
- more?

### V0.4 ###
- gpu rendering
- energy redistribution path tracing
- volume rendering