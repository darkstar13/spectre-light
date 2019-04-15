//author: Davide Pizzolotto
//license: GNU GPLv3

#include "chunk_wrapper.hpp"
#include "utility/console.hpp"
#include <cstdio>

#define MAGIC_NUMBER 0xAB7E0CBB

void ChunkWrapper::push_chunk(ChunkNamed& chunk)
{
    //the order is: id(2 bytes), size(8 bytes), data
    ChunkWrapper::chunks.push_back(std::move(chunk));
}

void ChunkWrapper::pop_chunk(ChunkNamed* chunk)
{
    *chunk = ChunkWrapper::chunks.front();
    ChunkWrapper::chunks.pop_front();
}

bool ChunkWrapper::empty()
{
    return chunks.empty();
}

void ChunkWrapper::write(const char* path) const
{
    FILE* fout = fopen(path, "wb+");
    if(fout != NULL)
    {
        uint32_t magic_number = MAGIC_NUMBER;
        size_t allocated_len = 4096;
        auto size = (uint16_t)chunks.size();
        auto data = (uint8_t*)malloc(sizeof(uint8_t)*allocated_len);
        fwrite(&magic_number, sizeof(uint32_t), 1, fout);
        fwrite(&size, sizeof(uint16_t), 1, fout);
        for(auto copy : ChunkWrapper::chunks)
        {
            fwrite(&(copy.id), sizeof(copy.id), 1, fout);
            auto len = (uint64_t)copy.data.size();
            fwrite(&len, sizeof(uint64_t), 1, fout);
            if(len>allocated_len)
            {
                while(len<allocated_len)
                    allocated_len <<= 1U;
                free(data);
                data = (uint8_t*)malloc(sizeof(uint8_t)*allocated_len);
            }
            uint64_t data_index = 0;
            for(uint8_t value : copy.data)
            {
                data[data_index++] = value;
            }
            fwrite(data, sizeof(uint8_t), len, fout);
        }
        free(data);
        fclose(fout);
    }
    else
        Console.severe(MESSAGE_W_DENIED, path);

}

void ChunkWrapper::read(const char* path)
{
    FILE* fin = fopen(path, "rb");
    if(fin != NULL)
    {
        size_t allocated_len = 4096;
        uint32_t magic_number;
        uint16_t size;
        fread(&magic_number, sizeof(uint32_t), 1, fin);
        fread(&size, sizeof(uint16_t), 1, fin);
        auto data = (uint8_t*)malloc(sizeof(uint8_t)*allocated_len);
        for(uint16_t i = 0; i<size; i++)
        {
            uint16_t chunk_id;
            uint64_t len;
            fread(&chunk_id, sizeof(uint16_t), 1, fin);
            fread(&len, sizeof(uint64_t), 1, fin);
            if(len>allocated_len)
            {
                while(len<allocated_len)
                    allocated_len <<= 1U;
                free(data);
                data = (uint8_t*)malloc(sizeof(data)*allocated_len);
            }
            ChunkNamed tmp;
            tmp.set_id(chunk_id);
            fread(data, sizeof(uint8_t), len, fin);
            for(uint32_t j = 0; j<len; j++)
                tmp.push_int8(data[j]);
            ChunkWrapper::push_chunk(tmp);
        }
        free(data);
        fclose(fin);
    }
    else
        Console.severe(MESSAGE_INPUT_ERROR, path);
}