
#include <gtest/gtest.h>


#include "parsers/parser_obj.hpp"
#include "primitives/mesh.hpp"

TEST(ParserObj,input_error)
{
    ParserObj parser;
    errors_count[ERROR_INDEX] = 0;
    parser.start_parsing(TEST_ASSETS "parser_obj/nonexistent.obj");
    EXPECT_EQ(errors_count[ERROR_INDEX], 1);
    errors_count[ERROR_INDEX] = 0;
}

TEST(ParserObj,close_unopened_input)
{
    ParserObj parser;
    parser.end_parsing();
    //test passes if everything does not crash
    EXPECT_TRUE(true);
}

TEST(ParserObj,get_next_mesh_unopened)
{
    //unopened
    ParserObj parser;
    Mesh m(1);
    EXPECT_FALSE(parser.get_next_mesh(&m));
    parser.start_parsing(TEST_ASSETS "parser_obj/nonexistent.obj");
    EXPECT_FALSE(parser.get_next_mesh(&m));
}

TEST(ParserObj,get_next_mesh_retval)
{
    bool res;
    ParserObj parser;
    parser.start_parsing(TEST_ASSETS "parser_obj/pyramid.obj");
    Mesh m(6);
    res = parser.get_next_mesh(&m);
    EXPECT_TRUE(res);
    res = parser.get_next_mesh(&m);
    EXPECT_FALSE(res);
    parser.end_parsing();

    parser.start_parsing(TEST_ASSETS "parser_obj/multi.obj");
    Mesh m1(6);
    Mesh m2(2);
    res = parser.get_next_mesh(&m1);
    EXPECT_TRUE(res);
    res = parser.get_next_mesh(&m2);
    EXPECT_TRUE(res);
    res = parser.get_next_mesh(&m1);
    EXPECT_FALSE(res);
    parser.end_parsing();
}

TEST(ParserObj,get_next_obj_tris)
{
    bool res;
    ParserObj parser;
    parser.start_parsing(TEST_ASSETS "parser_obj/pyramid.obj");
    Mesh m(6);
    res = parser.get_next_mesh(&m);
    EXPECT_NEAR(m.surface(),3.2360679775,1e-5);
    EXPECT_TRUE(res);
    parser.end_parsing();
}

TEST(ParseObj,triangulate)
{
    bool res;
    ParserObj parser;
    parser.start_parsing(TEST_ASSETS "parser_obj/ngon.obj");
    Mesh m(6);
    res = parser.get_next_mesh(&m);
    EXPECT_EQ(parser.get_face_no(), 14);
    EXPECT_NEAR(m.surface(),8.221780,1e-5);
    EXPECT_TRUE(res);
    parser.end_parsing();
}

TEST(ParserObj,negative_tris_index)
{
    bool res;
    ParserObj parser;
    parser.start_parsing(TEST_ASSETS "parser_obj/neg_vertices.obj");
    Mesh m(6);
    res = parser.get_next_mesh(&m);
    EXPECT_NEAR(m.surface(),3.2360679775,1e-5);
    EXPECT_TRUE(res);
    parser.end_parsing();
}

TEST(ParserObj,multiple_meshes)
{
    bool res;
    ParserObj parser;
    parser.start_parsing(TEST_ASSETS "parser_obj/multi.obj");
    Mesh m0(1);
    Mesh m1(1);
    res = parser.get_next_mesh(&m0);
    ASSERT_TRUE(res);
    res = parser.get_next_mesh(&m1);
    ASSERT_TRUE(res);
    EXPECT_NEAR(m0.surface(),6,1e-5);
    EXPECT_NEAR(m1.surface(),6,1e-5);
    parser.end_parsing();
}

TEST(ParserObj,textures)
{
    //can't test correctness for textures... so I'll put a multiobject in the
    //same file. If textures are parsed wrongly the second object will have
    //wrong vertices
    bool res;
    ParserObj parser;
    parser.start_parsing(TEST_ASSETS "parser_obj/textures.obj");
    Mesh m0(1);
    Mesh m1(1);
    res = parser.get_next_mesh(&m0);
    EXPECT_TRUE(res);
    res = parser.get_next_mesh(&m1);
    EXPECT_TRUE(res);
    EXPECT_NEAR(m1.surface(),3.2360679775,1e-5);
    parser.end_parsing();
}

TEST(ParserObj,normal_reconstruction)
{
    bool res;
    ParserObj parser;
    parser.start_parsing(TEST_ASSETS "parser_obj/nonormal.obj");
    Mesh m(6);
    res = parser.get_next_mesh(&m);
    ASSERT_TRUE(res);
    float densities[6];
    Matrix4 mat;
    mat.set_identity();
    m.get_densities_array(&mat, densities);
    Point3 p;
    Normal n;
    m.sample_point(0.f, 0.f, densities, &p, &n);
    EXPECT_NEAR(n.x,1.6,1e-5);
    EXPECT_NEAR(n.y,0,1e-5);
    EXPECT_NEAR(n.z,0.5,1e-5);
    parser.end_parsing();
}

TEST(ParserObj,get_object_name)
{
    //object name with g
    bool res;
    ParserObj parser;
    parser.start_parsing(TEST_ASSETS "parser_obj/pyramid.obj");
    Mesh m(6);
    res = parser.get_next_mesh(&m);
    EXPECT_EQ(parser.get_mesh_name(), "SquarePyr");
    EXPECT_TRUE(res);
    parser.end_parsing();

    parser.start_parsing(TEST_ASSETS "parser_obj/pyramid_o.obj");
    Mesh m2(6);
    res = parser.get_next_mesh(&m2);
    EXPECT_EQ(parser.get_mesh_name(), "SquarePyr");
    EXPECT_TRUE(res);
    parser.end_parsing();
}

TEST(ParserObj,get_face_number)
{
    //object name with g
    bool res;
    ParserObj parser;
    parser.start_parsing(TEST_ASSETS "parser_obj/pyramid.obj");
    Mesh m(6);
    res = parser.get_next_mesh(&m);
    EXPECT_EQ(parser.get_face_no(), 6);
    EXPECT_TRUE(res);
    parser.end_parsing();
}

TEST(ParserObj,get_material_no)
{
    //push some materials used in the file into the library
    Bsdf* mat0 = new SingleBRDF();
    Bsdf* mat1 = new SingleBRDF();
    mat0->inherit_bdf(new Lambertian());
    mat1->inherit_bdf(new Lambertian());
    MtlLib.add_inherit("Red", mat0);
    MtlLib.add_inherit("Green", mat1);
    ParserObj parser;
    bool res;
    parser.start_parsing(TEST_ASSETS "parser_obj/multimat.obj");
    Mesh m(6);
    res = parser.get_next_mesh(&m);
    EXPECT_EQ(parser.get_material_no(), 4);
    EXPECT_TRUE(res);
    parser.end_parsing();
    MtlLib.clear();
}

TEST(ParserObj,get_materials)
{
    //push some materials used in the file into the library
    Bsdf* mat0 = new SingleBRDF();
    Bsdf* mat1 = new SingleBRDF();
    mat0->inherit_bdf(new Lambertian());
    mat1->inherit_bdf(new Lambertian());
    MtlLib.add_inherit("Red", mat0);
    MtlLib.add_inherit("Green", mat1);
    ParserObj parser;
    bool res;
    parser.start_parsing(TEST_ASSETS "parser_obj/multimat.obj");
    Mesh m(6);
    res = parser.get_next_mesh(&m);
    const Bsdf** mats = new const Bsdf*[parser.get_material_no()];
    parser.get_materials(mats);
    for(int i=0;i<parser.get_material_no();i++)
        EXPECT_EQ(mats[i], MtlLib.get_default());
    EXPECT_TRUE(res);
    delete[] mats;
    parser.end_parsing();
    MtlLib.clear();
}

TEST(ParserObj,get_material_association)
{
    //push some materials used in the file into the library
    Bsdf* mat0 = new SingleBRDF();
    Bsdf* mat1 = new SingleBRDF();
    mat0->inherit_bdf(new Lambertian());
    mat1->inherit_bdf(new Lambertian());
    MtlLib.add_inherit("Red", mat0);
    MtlLib.add_inherit("Green", mat1);
    ParserObj parser;
    bool res;
    parser.start_parsing(TEST_ASSETS "parser_obj/multimat.obj");
    Mesh m(6);
    res = parser.get_next_mesh(&m);
    unsigned char* assoc = new unsigned char[parser.get_face_no()];
    parser.get_material_association(assoc);
    parser.end_parsing();
    EXPECT_EQ(assoc[0],0);
    EXPECT_EQ(assoc[1],0);
    EXPECT_EQ(assoc[2],1);
    EXPECT_EQ(assoc[3],2);
    EXPECT_EQ(assoc[4],0);
    EXPECT_EQ(assoc[5],3);
    EXPECT_TRUE(res);
    MtlLib.clear();
}


