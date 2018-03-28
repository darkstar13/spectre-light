# Input File format

The file format expected by *spectre-light* is similar in fashion to a JSON file,
with the following differences:

- The keys are reserved keywords and thus cannot be neither user-defined nor quoted
- A comma is not required between each `key:value` pair
- The toplevel object cannot be enclosed in curly braces

Here is an example of the file structure
```
out: "output.tga"
resolution: {
    width: 1280
    height: 1024
}
filter: {
    type: lanczos
    value0: 3.5
}
spp: 36
integrator: pt
sampler: stratified
camera: {
    type: perspective
    position: [0,0,0]
    target: [-1.5,0.0,.9]
    up: [0,1,0]
    fov: 0.55
}
shape: "lamp.obj"
shape: "scene.obj"

light: {
    name: "normal lamp"
    temperature: 5500
}

world: {
    name: "box001"
    position: [-1,0,0]
    scale: 1.5
}
```
## Key(word)s
### Toplevel keys
The possible keys are the following:

Key | Type | Usage
---|---|---
out| quoted string | The file that will be written when rendering the current scene
resolution | [object](#resolution) | The resolution of the final rendered image
filter | [object](#filter) | The filter used to process the final image after gathering each sample value
spp | int | The number of samples per pixel. The default value is 121
integrator | enum | The type of integrator used to solve the rendering equation. The only possible value is `pt` for path trace
sampler | enum | The type of sampler used to gather samples. Possible values are `random` for a pure random sampler or `stratified` for a sampler that subdivides a region in strata and picks a random sample for each strata. The latter is the default and suggested value
camera | [object](#camera) | A description of the camera used to render the current scene
shape | quoted string | A path to an obj file containing mesh descriptions. The actual mesh/es are not used in the scene until positioned with the `world` keyword
light | [object](#light) | Description of a light positioned into the scene
world | [object](#world) | Description of a shape positioned into the scene

### Objects
These are the various keys that can be used for objects described in the previous section. These objects must be enclosed in curly braces

#### Resolution
Keys for Resolution objects

Key | Type | Usage
---|---|---
width | int | The width of the rendered image
height | int | The height of the rendered image

#### Camera
Keys for Camera objects

Key | Type | Usage
---|---|---
type | enum | Defines the type of camera. The values can be `perspective`, `orthographic` or `panorama`
position | 3 values float array | The position of the camera. Default value is `[0,0,0]`
target | 3 values float array | The target of the camera. Default value is `[0,0,1]`
up | 3 values float array | A vector representing the upward direction seen from the camera. The default value is `[0,1,0]`
fov | float | Used only by perspective cameras, the field of view in degrees

#### Filter
Keys for Filter objects

Key | Type | Usage
---|---|---
type | enum | Defines the type of filter. The values can be `box`, `tent`, `gaussian`, `mitchell` and `lanczos`. The default value is `mitchell`
value0 | float | The parameter for the filters. Unused in box and tent filter, this value is the σ (falloff) value of the gaussian filter defaulted to 2, the B parameter of the Mitchell-Netravali defaulted to 0.33 and the 𝜏 parameter of the Lanczos-Sinc filter defaulted to 3
value1 | float | Used only in the Mitchell-Netravali filter as the C parameter. The default value is 0.33

#### Light
Keys for Light objects

Key | Type | Usage
---|---|---
name | quoted string | The name of the light model. This is the name of a mesh previously parsed with the `shape` keyword. This value is mandatory.
temperature | int | The temperature in kelvin of the light. This value will set the light color based on its temperature and thus should not be used in conjunction with the `color` keyword
color | 3 values float array | The spectrum emitted by this light. The spectrum is represented as a 3 values RGB number where `[0,0,0]` is full black and `[1,1,1]` is full white. Default is `[1,1,1]`
position | 3 values float array | The position of the light in the scene. Default is `[0,0,0]`
rotation | 3 values float array | The rotation of the light in the scene, in degrees. Default is `[0,0,0]`
scale | 3 values float array or single float value | The scaling applied to the light in the scene. The single float value applies uniform scaling to every dimension. Default is `[1,1,1]`

#### World
Keys for World objects

Key | Type | Usage
---|---|---
name | quoted string | The name of the model previously parsed with the `shape` keyword. This value is mandatory.
material | quoted string | The name of the material that will be applied to the model
position | 3 values float array | The position of the model in the scene. Default is `[0,0,0]`
rotation | 3 values float array | The rotation of the model in the scene, in degrees. Default is `[0,0,0]`
scale | 3 values float array or single float value | The scaling applied to the model in the scene. The single float value applies uniform scaling to every dimension. Default is `[1,1,1]`
