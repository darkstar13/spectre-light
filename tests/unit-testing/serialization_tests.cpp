#include "spectre_tests.h"

#ifdef __XCODE__
#import <XCTest/XCTest.h>
#elif defined(__VS__)
#include "CppUnitTest.h"
#else

#include <gtest/gtest.h>

#endif

#include "geometry/vec3.hpp"
#include "geometry/vec2.hpp"
#include "utility/utility.hpp"
#include <climits>
#include "parsers/chunk.hpp"
#include "parsers/chunk_wrapper.hpp"
#include <cstdio> //mktemp
#include <cstdlib>
#include "utility/file.hpp"

SPECTRE_TEST_INIT(Serialization_tests)

SPECTRE_TEST(Serialization, chunk_int8)
{
    Chunk chunk;
    chunk.push_int8(10);
    chunk.push_int8(255);
    chunk.push_int8(-3);
    chunk.push_int8(32);
    EXPECT_EQ((uint8_t)chunk.pop_int8(), (uint8_t)10);
    EXPECT_EQ((uint8_t)chunk.pop_int8(), (uint8_t)255);
    EXPECT_EQ((int8_t)chunk.pop_int8(), (int8_t)-3);
    EXPECT_EQ((uint8_t)chunk.pop_int8(), (uint8_t)32);
}

SPECTRE_TEST(Serialization, chunk_int16)
{
    Chunk chunk;
    chunk.push_int16(10);
    chunk.push_int16(25500);
    chunk.push_int16(-3000);
    chunk.push_int16(32);
    //expect true because vs does not support uint16_t comparisons. W O W
    EXPECT_EQ((int16_t)chunk.pop_int16(), (int16_t)10);
    EXPECT_TRUE((uint16_t)chunk.pop_int16()==(uint16_t)25500);
    EXPECT_EQ((int16_t)chunk.pop_int16(), (int16_t)-3000);
    EXPECT_EQ((int16_t)chunk.pop_int16(), (int16_t)32);
}

SPECTRE_TEST(Serialization, chunk_int32)
{
    Chunk chunk;
    chunk.push_int32(10);
    chunk.push_int32(25678986);
    chunk.push_int32(-5358657);
    chunk.push_int32(32);
    EXPECT_EQ((uint32_t)chunk.pop_int32(), (uint32_t)10);
    EXPECT_EQ((uint32_t)chunk.pop_int32(), (uint32_t)25678986);
    EXPECT_EQ((int32_t)chunk.pop_int32(), (int32_t)-5358657);
    EXPECT_EQ((uint32_t)chunk.pop_int32(), (uint32_t)32);
}

SPECTRE_TEST(Serialization, chunk_int64)
{
    Chunk chunk;
    chunk.push_int64(10);
    chunk.push_int64((uint64_t)2567898765445656786);
    chunk.push_int64((int64_t)-53456758659876547);
    chunk.push_int64(32);
    //vs does not support also int64_t comparisons...
    EXPECT_EQ((uint64_t)chunk.pop_int64(), (uint64_t)10);
    EXPECT_EQ((uint64_t)chunk.pop_int64(), (uint64_t)2567898765445656786);
    EXPECT_TRUE((int64_t)chunk.pop_int64()==(int64_t)-53456758659876547);
    EXPECT_EQ((uint64_t)chunk.pop_int64(), (uint64_t)32);
}

SPECTRE_TEST(Serialization, chunk_float)
{
    Chunk chunk;
    chunk.push_float(3.14f);
    chunk.push_float(354.34f);
    EXPECT_EQ(chunk.pop_float(), 3.14f);
    EXPECT_EQ(chunk.pop_float(), 354.34f);
}

SPECTRE_TEST(Serialization, chunk_double)
{
    Chunk chunk;
    chunk.push_double(3.14);
    chunk.push_double(354.34);
    chunk.push_double(-6437.876469766);
    EXPECT_EQ(chunk.pop_double(), 3.14);
    EXPECT_EQ(chunk.pop_double(), 354.34);
    EXPECT_EQ(chunk.pop_double(), -6437.876469766);
}

SPECTRE_TEST(Serialization, chunk_string)
{
    Chunk chunk;
    chunk.push_string("/root/vmlinuz");
    chunk.push_string("output.ppm");
    EXPECT_STREQ(chunk.pop_string().c_str(), "/root/vmlinuz");
    EXPECT_STREQ(chunk.pop_string().c_str(), "output.ppm");
}

SPECTRE_TEST(Serialization, chunk_all_types)
{
    Chunk chunk;
    chunk.push_int8(250);
    chunk.push_float(3.14f);
    chunk.push_string("test string!!!");
    chunk.push_int32(21476543);
    chunk.push_string("another string");
    chunk.push_int16(-3);

    EXPECT_EQ(chunk.pop_int8(), (uint8_t)250);
    EXPECT_EQ(chunk.pop_float(), 3.14f);
    EXPECT_STREQ(chunk.pop_string().c_str(), "test string!!!");
    EXPECT_EQ(chunk.pop_int32(), (uint32_t)21476543);
    EXPECT_STREQ(chunk.pop_string().c_str(), "another string");
    EXPECT_EQ((int16_t)chunk.pop_int16(), (int16_t)-3);
}

SPECTRE_TEST(Serialization, chunknamed_id)
{
    ChunkNamed chunk;
    EXPECT_TRUE(chunk.get_id()==(uint16_t)0);
    chunk.set_id(35530);
    EXPECT_TRUE(chunk.get_id() == 35530);
}

SPECTRE_TEST(Serialization, chunknamed_chunk)
{
    Chunk chunk0;
    chunk0.push_string("I'm a chunk");
    uint64_t len0 = chunk0.size();
    Chunk chunk1;
    chunk1.push_string("I'm another chunk");
    uint64_t len1 = chunk1.size();
    ChunkNamed named;
    named.set_id(1);
    named.push_int32(4000000001);
    named.push_chunk(&chunk0);
    named.push_chunk(&chunk1);

    EXPECT_TRUE(named.get_id() == 1);
    EXPECT_EQ((uint32_t)named.pop_int32(), (uint32_t)4000000001);
    Chunk retrieved0;
    Chunk retrieved1;
    named.pop_chunk(&retrieved0, len0);
    named.pop_chunk(&retrieved1, len1);
    EXPECT_STREQ(retrieved0.pop_string().c_str(), "I'm a chunk");
    EXPECT_STREQ(retrieved1.pop_string().c_str(), "I'm another chunk");
}

SPECTRE_TEST(Serialization, wrapper_add_remove_chunks)
{
    ChunkNamed chunk1;
    chunk1.set_id(1);
    chunk1.push_string("test string for chunk 0");
    chunk1.push_string("another one");
    ChunkNamed chunk2;
    chunk2.set_id(2);
    chunk2.push_string("test string for chunk 1");
    chunk2.push_float(3.14f);
    chunk2.push_string("another one for chunk 1");
    ChunkWrapper wrapper;
    EXPECT_TRUE(wrapper.empty());
    wrapper.push_chunk(chunk1);
    EXPECT_FALSE(wrapper.empty());
    wrapper.push_chunk(chunk2);

    ChunkNamed retrieved1;
    ChunkNamed retrieved2;
    wrapper.pop_chunk(&retrieved1);
    wrapper.pop_chunk(&retrieved2);
    EXPECT_TRUE(wrapper.empty());
    EXPECT_STREQ(retrieved1.pop_string().c_str(), "test string for chunk 0");
    EXPECT_STREQ(retrieved1.pop_string().c_str(), "another one");
    EXPECT_STREQ(retrieved2.pop_string().c_str(), "test string for chunk 1");
    EXPECT_EQ(retrieved2.pop_float(), 3.14f);
    EXPECT_STREQ(retrieved2.pop_string().c_str(), "another one for chunk 1");
}

SPECTRE_TEST(Serialization, wrapper_to_file)
{
    ChunkNamed chunk1;
    chunk1.set_id(1);
    chunk1.push_string("test string for chunk 0");
    chunk1.push_string("another one");
    ChunkNamed chunk2;
    chunk2.set_id(2);
    chunk2.push_string("test string for chunk 1");
    chunk2.push_float(3.14f);
    chunk2.push_string("another one for chunk 1");
    ChunkWrapper wrapper;
    wrapper.push_chunk(chunk1);
    wrapper.push_chunk(chunk2);

    //there is a security risk in mktemp due to data race, but this is testing
    //so who cares
    File tmp = File::get_temp_file();
    const char* tmp_file = tmp.absolute_path();

    wrapper.write(tmp_file);
    ChunkWrapper retrieved_wrapper;
    retrieved_wrapper.read(tmp_file);

    ChunkNamed retrieved1;
    ChunkNamed retrieved2;
    retrieved_wrapper.pop_chunk(&retrieved1);
    retrieved_wrapper.pop_chunk(&retrieved2);
    EXPECT_STREQ(retrieved1.pop_string().c_str(), "test string for chunk 0");
    EXPECT_STREQ(retrieved1.pop_string().c_str(), "another one");
    EXPECT_STREQ(retrieved2.pop_string().c_str(), "test string for chunk 1");
    EXPECT_EQ(retrieved2.pop_float(), 3.14f);
    EXPECT_STREQ(retrieved2.pop_string().c_str(), "another one for chunk 1");

    unlink(tmp_file);
}

SPECTRE_TEST_END(Serialization)