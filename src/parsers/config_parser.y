/* Created,  24 Mar 2018 */
/* Last Edit  3 Apr 2018 */

%define api.value.type union-directive
%define lr.type lalr
%code requires
%define parse.trace
%define parse.error verbose
%{
    #include "parsers/parsed_structs.h"
    int yylex(void);
    void yyerror(char const*);
    ParsedScene parsed;
    //add a string to the Parsed data and avoid a leak if overwriting
    #define ADD_STRING(DST,SRC) if(DST==NULL)free(DST);DST=SRC;
%}

%token END 0 "end of file"
%token INVALID "invalid character"
%token COLON ":"
%token OPEN_SQ "["
%token CLOSE_SQ "]"
%token OPEN_CU "{"
%token CLOSE_CU "}"
%token COMMA ","
%token OUTPUT "`out` keywork"
%token INTEGRATOR "`integrator` keyword"
%token RESOLUTION "`resolution` keyword"
%token WIDTH "`width` keyword"
%token HEIGHT "`height` keyword"
%token SAMPLER "``sampler` keyword"
%token SPP "`spp` keyword"
%token FILTER "`filter` keyword"
%token TYPE "`type` keyword"
%token CAMERA "`camera` keyword"
%token ORTHOGRAPHIC "`orthographic` keyword"
%token PERSPECTIVE "`perspective` keyword"
%token PANORAMA "`panorama` keyword"
%token POSITION "`position` keyword"
%token TARGET "`target` keyword"
%token UP "`up` keyword"
%token FOV "`fov` keyword"
%token RANDOM "`random` keyword"
%token STRATIFIED "`stratified` keyword"
%token BOX "`box` keyword"
%token TENT "`tent` keyword"
%token GAUSS "`gauss` keyword"
%token MITCHELL "`mitchell` keyword"
%token LANCZOS "`lanczos` keyword"
%token UNFILTERED "`unfiltered` keyword"
%token TRILINEAR "`trilinear` keyword"
%token EWA "`ewa` keyword"
%token VAL_0 "`value0` keyword"
%token VAL_1 "`value1` keyword"
%token SHAPE "`shape` keyword"
%token WORLD "`world` keyword"
%token LIGHT "`light` keyword"
%token TEMPERATURE "`temperature` keyword"
%token NAME "`name` keyword"
%token ROTATION "`rotation` keyword"
%token SCALE "`scale` keyword"
%token SHIFT "`shift` keyword"
%token COLOR "`color` keyword"
%token MATERIAL "`material` keyword"
%token DUALMATERIAL "`dualmaterial` keyword"
%token TEXTURE "`texture` keyword"
%token ANISOTROPY "`anisotropy` keyword"
%token BECKMANN "`beckmann` keyword"
%token BLINN "`blinn` keyword"
%token DIFFUSE "`diffuse` keyword"
%token DISTRIBUTION "`distribution` keyword"
%token ELEM "`element` keyword"
%token GGX "`ggx` keyword"
%token GLASS "`glass` keyword"
%token GLOSSY "`glossy` keyword"
%token IOR "`ior` keyword"
%token MATTE "`matte` keyword"
%token METAL "`metal keyword"
%token ROUGHNESS "`roughness` keyword"
%token SPECULAR "`specular` keyword"
/* %token BUMP "`bump` keyword" */
%token NORMAL "`normal` keyword"
%token SRC "`src` keyword"
%token PATH_TRACE "`path` keyword"
%token CHNR "Red channel attribute"
%token CHNG "Green channel attribute"
%token CHNB "Blue channel attribute"
%token CHNA "Alpha channel attribute"
%token INV "inverted attribute"
%token MASK "`mask` keyword"
%token FIRST "`first` keyword"
%token SECOND "`second` keyword"
%token RADIUS "`radius` keyword"
%token FALLOFF "`falloff` keyword"
%token OMNI "`omni` keyword"
%token SPOT "`spot` keyword"
%token AREA "`area` keyword"
%token SILVER "`Ag`"
%token ALUMINIUM "`Al`"
%token GOLD "`Au`"
%token COPPER "`Cu`"
%token IRON "`Fe`"
%token MERCURY "`Hg`"
%token LEAD "`Pb`"
%token PLATINUM "`Pt`"
%token TUNGSTEN "`W`"
%token BERYLLIUM "`Be`"
%token BISMUTH "`Bi`"
%token COBALT "`Co`"
%token CHROMIUM "`Cr`"
%token GERMANIUM "`Ge`"
%token POTASSIUM "`K`"
%token LITHIUM "`Li`"
%token MAGNESIUM "`Mg`"
%token MANGANESE "`Mn`"
%token MOLYBDENUM "`Mo`"
%token SODIUM "`Na`"
%token NIOBIUM "`Nb`"
%token NICKEL "`Ni`"
%token PALLADIUM "`Pd`"
%token RHODIUM "`Rh`"
%token TANTALUM "`Ta`"
%token TITANIUM "`Ti`"
%token VANADIUM "`V`"
%token ZINC "`Zn`"
%token ZIRCONIUM "`Zr`"

%union{
    struct { float x, float y, float z } vec;
    float fval;
    int ival;
    uint32_t uval;
    char* sval;
}

%token <uval> UINT "positive integer value"
%token <ival> INT "integer value"
%token <fval> FLOAT "floating point value"
%token <sval> STRING "quoted string"

%%

%start file;

file
: file stmt
| stmt
;

stmt
: OUTPUT COLON STRING {ADD_STRING(parsed.output,$3.sval);}
| RESOLUTION COLON OPEN_CU resolution_obj CLOSE_CU
| FILTER COLON OPEN_CU filter_obj CLOSE_CU
| SAMPLER  COLON RANDOM {parsed.sampler_type = RANDOM;}
| SAMPLER COLON STRATIFIED {parsed.sampler_type = STRATIFIED;}
| SPP COLON UINT        {parsed.spp = $3.fval;}
| INTEGRATOR COLON PATH_TRACE {/* path_trace is the only available and dflt */}
| CAMERA COLON OPEN_CU camera_obj CLOSE_CU {/* camera depends on resolution */}
| SHAPE COLON STRING {push_ResizableStack(&(parsed.parsed_mesh_object),$3.sval);}
| WORLD COLON OPEN_CU world_obj CLOSE_CU {push_ResizableParsed(&(parsed.parsed_mesh_world),&(parsed.cur_mesh));init_ParsedMeshWorld(&(parsed->cur_mesh));}
| LIGHT COLON OPEN_CU light_obj CLOSE_CU {push_ResizableParsed(&(parsed.parsed_lights),&(parsed.cur_light));init_ParsedLight(&(parsed->cur_light));}
| TEXTURE COLON OPEN_CU texture_obj CLOSE_CU {push_ResizableParsed(&(parsed.parsed_textures),&(parsed.cur_tex));init_ParsedTexture(&(parsed->cur_tex));}
| MATERIAL COLON STRING {push_ResizableStack(&(parsed.children),$3.sval);}
| MATERIAL COLON OPEN_CU material_obj CLOSE_CU {push_ResizableParsed(&(parsed.parsed_materials),&(parsed.cur_mat));init_ParsedMaterial(&(parsed->cur_mat));}
| DUALMATERIAL COLON OPEN_CU dualmaterial_obj CLOSE_CU {push_ResizableParsed(&(parsed.parsed_dualmaterials),&(parsed.cur_dualmat));init_ParsedDualMaterial(&(parsed.cur_dualmat));}
| COMMA
;

resolution_obj: resolution_obj resolution_stmt | resolution_stmt;
resolution_stmt
: WIDTH COLON UINT {parsed.width = $3.uval;}
| HEIGHT COLON UINT {parsed.height = $3.uval;}
| COMMA
;

filter_obj: filter_obj filter_stmt | filter_stmt;
filter_stmt
: TYPE COLON BOX {parsed.filter_type = BOX;}
| TYPE COLON TENT {parsed.filter_type = TENT;}
| TYPE COLON GAUSS {parsed.filter_type = GAUSS;}
| TYPE COLON MITCHELL {parsed.filter_type = MITCHELL;}
| TYPE COLON LANCZOS {parsed.filter_type = LANCZOS;}
| VAL_0 COLON number {parsed.value0 = $3.fval;}
| VAL_1 COLON number {parsed.value1 = $3.fval;}
| TEXTURE COLON UNFILTERED {driver.tex_filter = UNFILTERED;}
| TEXTURE COLON TRILINEAR {driver.tex_filter = TRILINEAR;}
| TEXTURE COLON EWA {driver.tex_filter = EWA;}
| COMMA
;

camera_obj: camera_obj camera_stmt | camera_stmt;
camera_stmt
: TYPE COLON ORTHOGRAPHIC {parsed.camera_type = ORTHOGRAPHIC;}
| TYPE COLON PERSPECTIVE {parsed.camera_type = PERSPECTIVE;}
| TYPE COLON PANORAMA {parsed.camera_type = PANORAMA;}
| POSITION COLON vector {parsed.camera_pos[0] = $3.vec.x;parsed.camera_pos[1] = $3.vec.y;parsed.camera_pos[2] = $3.vec.z;}
| TARGET COLON vector {parsed.camera_tar[0] = $3.vec.x;parsed.camera_tar[1] = $3.vec.y;parsed.camera_tar[2] = $3.vec.z;}
| UP COLON vector {parsed.camera_up[0] = $3.vec.x;parsed.camera_up[1] = $3.vec.y;parsed.camera_up[2] = $3.vec.z;}
| FOV COLON number {parsed.fov = $3;}
| COMMA
;

world_obj: world_obj world_stmt | world_stmt
world_stmt
: NAME COLON STRING {ADD_STRING(&(parsed.cur_mesh.name),$3.sval);}
| POSITION COLON vector {parsed.cur_mesh.position[0] = $3.vec.x;parsed.cur_mesh.position[1] = $3.vec.y;parsed.cur_mesh.position[2] = $3.vec.z;}
| ROTATION COLON vector {parsed.cur_mesh.rotation[0] = $3.vec.x;parsed.cur_mesh.rotation[1] = $3.vec.y;parsed.cur_mesh.rotation[2] = $3.vec.z;}
| SCALE COLON vector {parsed.cur_mesh.scale[0] = $3.vec.x;parsed.cur_mesh.scale[1] = $3.vec.y;parsed.cur_mesh.scale[2] = $3.vec.z;}
| SCALE COLON number {parsed.cur_mesh.scale[0] = $3.fval;parsed.cur_mesh.scale[1] = $3.fval;parsed.cur_mesh.scale[2] = $3.fval;}
| MATERIAL COLON STRING {ADD_STRING(&(parsed.cur_mesh.material_name),$3.sval);}
| MASK COLON STRING {ADD_STRING(&(parsed.cur_mask.mask_tex),$3.sval); parsed.cur_mesh.mask = parsed.cur_mask;init_ParsedMask(&(parsed.cur_mask));}
| MASK COLON STRING attributes {ADD_STRING(&(parsed.cur_mask.mask_tex),$3.sval); parsed.cur_mesh.mask = parsed.cur_mask;init_ParsedMask(&(parsed.cur_mask));}
| COMMA
;

light_obj: light_obj light_stmt | light_stmt;
light_stmt
: NAME COLON STRING {ADD_STRING(&(parsed.cur_light.name),$3.sval);}
| TEMPERATURE COLON UINT {parsed.cur_light.temperature = $3.uval;}
| COLOR COLON vector {parsed.cur_light.color[0] = $3.vec.x;parsed.cur_light.color[1] = $3.vec.y;parsed.cur_light.color[2] = $3.vec.z;}
| TYPE COLON AREA {parsed.cur_light.type = AREA;}
| TYPE COLON OMNI {parsed.cur_light.type = OMNI;}
| TYPE COLON SPOT {parsed.cur_light.type = SPOT;}
| POSITION COLON vector {parsed.cur_light.position[0] = $3.vec.x;parsed.cur_light.position[1] = $3.vec.y;parsed.cur_light.position[2] = $3.vec.z;}
| ROTATION COLON vector {parsed.cur_mesh.rotation[0] = $3.vec.x;parsed.cur_mesh.rotation[1] = $3.vec.y;parsed.cur_mesh.rotation[2] = $3.vec.z;}
| SCALE COLON vector {parsed.cur_light.scale[0] = $3.vec.x;parsed.cur_light.scale[1] = $3.vec.y;parsed.cur_light.scale[2] = $3.vec.z;}
| SCALE COLON number {parsed.cur_light.scale[0] = $3.fval;parsed.cur_light.scale[1] = $3.fval;parsed.cur_light.scale[2] = $3.fval;}
| RADIUS COLON number {parsed.cur_light.radius = $3.fval;}
| FALLOFF COLON number {driver.cur_light.falloff = $3.fval;}
| COMMA
;

texture_obj: texture_obj texture_stmt|texture_stmt;
texture_stmt
: SRC COLON STRING {ADD_STRING(&(parsed.cur_tex.tex_name),$3.sval);}
| NAME COLON STRING {ADD_STRING(&(parsed.cur_tex.tex_src),$3.sval);}
| SCALE COLON vector2 {parsed.cur_tex.tex_scale[0] = $3.vec.x; parsed.cur_tex.tex_scale[1]=$3.vec.y;}
| SCALE COLON number {parsed.cur_text.tex_scale[0] = $3.fval; parsed.cur_tex.tex_scale[1]=$3.fval;}
| SHIFT COLON vector2 {parsed.cur_tex.tex_shift[0] = $3.vec.x; parsed.cur_tex.tex_shift[1]=$3.vec.y;}
| SHIFT COLON number {parsed.cur_tex.tex_shift[0] = $3.fval; parsed.cur_tex.tex_shift[1]=$3.fval;}
| COMMA
;


material_obj
: NAME COLON STRING material_rec {driver.cur_mat.name = $3.substr(1,$3.size()-2);}
| material_rec NAME COLON STRING {driver.cur_mat.name = $4.substr(1,$4.size()-2);}
| material_rec NAME COLON STRING material_rec {driver.cur_mat.name = $4.substr(1,$4.size()-2);}
| NAME COLON STRING {driver.cur_mat.name = $3.substr(1,$3.size()-2);}
;

material_rec: material_rec material_stmt | material_stmt;
material_stmt
: TYPE COLON MATTE {driver.cur_mat.type = MATTE;}
| TYPE COLON GLOSSY {driver.cur_mat.type = GLOSSY;}
| TYPE COLON METAL {driver.cur_mat.type = METAL;}
| TYPE COLON GLASS {driver.cur_mat.type = GLASS;}
| IOR COLON number {driver.cur_mat.ior = cauchy($3,0);}
| IOR COLON vector2 {driver.cur_mat.ior = cauchy($3.x,$3.y);}
| IOR COLON vector {driver.cur_mat.ior = cauchy($3.x,$3.y,$3.z);}
| IOR COLON vector vector {driver.cur_mat.ior = sellmeier($3.x,$3.y,$3.z,$4.x,$4.y,$4.z);}
| ROUGHNESS COLON number {driver.cur_mat.rough_x = $3;}
| ANISOTROPY COLON number {driver.cur_mat.rough_y = $3;}
| DISTRIBUTION COLON BLINN {driver.cur_mat.dist = SPECTRE_DIST_BLINN;}
| DISTRIBUTION COLON BECKMANN {driver.cur_mat.dist = SPECTRE_DIST_BECKMANN;}
| DISTRIBUTION COLON GGX {driver.cur_mat.dist = SPECTRE_DIST_GGX;}
| DIFFUSE COLON STRING {driver.cur_mat.diffuse = $3.substr(1,$3.size()-2);}
| DIFFUSE COLON vector {driver.tex_color = $3; driver.cur_mat.diffuse_uniform = driver.load_texture_uniform();}
| SPECULAR COLON STRING {driver.cur_mat.specular = $3.substr(1,$3.size()-2);}
| SPECULAR COLON vector {driver.tex_color = $3; driver.cur_mat.specular_uniform = driver.load_texture_uniform();}
/* | BUMP COLON STRING {driver.cur_mat.bump = $3.substr(1,$3.size()-2); driver.cur_mat.bump_is_normal = false;} */
| NORMAL COLON STRING {driver.cur_mat.bump = $3.substr(1,$3.size()-2); /* driver.cur_mat.bump_is_normal = true; */}
| ELEM COLON element {driver.cur_mat.elem = $3;}
| COMMA
;

dualmaterial_obj
: NAME COLON STRING dualmaterial_rec {driver.cur_dualmat.name = $3.substr(1,$3.size()-2);}
| dualmaterial_rec NAME COLON STRING {driver.cur_dualmat.name = $4.substr(1,$4.size()-2);}
| dualmaterial_rec NAME COLON STRING dualmaterial_rec {driver.cur_dualmat.name = $4.substr(1,$4.size()-2);}
| NAME COLON STRING {driver.cur_dualmat.name = $3.substr(1,$3.size()-2);}
;

dualmaterial_rec: dualmaterial_rec dualmaterial_stmt | dualmaterial_stmt;
dualmaterial_stmt
: FIRST COLON STRING {driver.cur_dualmat.first = $3.substr(1,$3.size()-2);}
| SECOND COLON STRING {driver.cur_dualmat.second = $3.substr(1,$3.size()-2);}
| MASK COLON STRING {driver.cur_mask.mask_tex = $3.substr(1,$3.size()-2);driver.cur_dualmat.mask = driver.cur_mask; driver.cur_mask = ParsedMask();}
| MASK COLON STRING attributes {driver.cur_mask.mask_tex = $3.substr(1,$3.size()-2);driver.cur_dualmat.mask = driver.cur_mask; driver.cur_mask = ParsedMask();}
| COMMA
;

/* add also elements to materials/metals.hpp file, while respecting indexes.
   Non-existent materials are thrown away here, so no check is performed outside
   the parser */
element
: SILVER {$$ = METAL_SILVER; }
| ALUMINIUM {$$ = METAL_ALUMINIUM; }
| GOLD {$$ = METAL_GOLD; }
| COPPER {$$ = METAL_COPPER; }
| IRON {$$ = METAL_IRON; }
| MERCURY {$$ = METAL_MERCURY; }
| LEAD {$$ = METAL_LEAD; }
| PLATINUM {$$ = METAL_PLATINUM; }
| TUNGSTEN {$$ = METAL_TUNGSTEN; }
| BERYLLIUM {$$ = METAL_BERYLLIUM; }
| BISMUTH {$$ = METAL_BISMUTH; }
| COBALT {$$ = METAL_COBALT; }
| CHROMIUM {$$ = METAL_CHROMIUM; }
| GERMANIUM {$$ = METAL_GERMANIUM; }
| POTASSIUM {$$ = METAL_POTASSIUM; }
| LITHIUM {$$ = METAL_LITHIUM; }
| MAGNESIUM {$$ = METAL_MAGNESIUM; }
| MANGANESE {$$ = METAL_MANGANESE; }
| MOLYBDENUM {$$ = METAL_MOLYBDENUM; }
| SODIUM {$$ = METAL_SODIUM; }
| NIOBIUM {$$ = METAL_NIOBIUM; }
| NICKEL {$$ = METAL_NICKEL; }
| PALLADIUM {$$ = METAL_PALLADIUM; }
| RHODIUM {$$ = METAL_RHODIUM; }
| TANTALUM {$$ = METAL_TANTALUM; }
| TITANIUM {$$ = METAL_TITANIUM; }
| VANADIUM {$$ = METAL_VANADIUM; }
| ZINC {$$ = METAL_ZINC; }
| ZIRCONIUM {$$ = METAL_ZIRCONIUM; }
;

attributes: attributes attribute | attribute;

attribute
: channel {driver.cur_mask.mask_chn = $1;}
| INV {driver.cur_mask.mask_inv = true;}
;

channel
: CHNR  {$$ = RED;}
| CHNG  {$$ = GREEN;}
| CHNB  {$$ = BLUE;}
| CHNA  {$$ = ALPHA;}
;

vector:
OPEN_SQ number COMMA number COMMA number CLOSE_SQ
{ $$ = Vec3($2,$4,$6);}
;

vector2
: OPEN_SQ number COMMA number CLOSE_SQ { $$ = Vec2($2,$4);}
;

number
: FLOAT {$$ = $1;}
| integer {$$ = (float)$1;}
;

integer
: UINT { $$ = $1;}
| INT  { $$ = $1;}
;

%%

void yyerror (const char* msg)
{
    printf("error");
    //driver.error(l,m);
}
