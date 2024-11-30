//All copyrights goes to JordanCpp, thnx for allowing me to use it - APAMK2

#pragma once

#include "ByteReader.hpp"
#include <vector>

class MemoryReader
{
public:
    MemoryReader(ByteReader* byteReader);
    void Close();
    bool Reset(const std::string& path, size_t type);
    bool Reset(const std::vector<uint8_t>& data);
    std::vector<uint8_t>& Content();
    void Read(void* buffer, size_t bytes);
    uint8_t u8();
    uint16_t u16();
    uint32_t u32();
    int16_t i16();
    int32_t i32();
    std::string string(int len);
    void Pos(size_t value);
    size_t CurrPos();
    size_t Bytes();
    bool FindFragment(const uint8_t* fragment, uint32_t fragmentLen, uint32_t beginOffs);
    bool Eof();
private:
    size_t _Offset;
    ByteReader* _ByteReader;
    std::vector<uint8_t> _Content;
};