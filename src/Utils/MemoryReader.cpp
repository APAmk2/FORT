//All copyrights goes to JordanCpp, thnx for allowing me to use it - APAMK2

#include "MemoryReader.hpp"
#include <string>

MemoryReader::MemoryReader(ByteReader* byteReader) :
	_Offset(0),
	_ByteReader(byteReader)
{
}

void MemoryReader::Close()
{
	_ByteReader->Close();
}

bool MemoryReader::Reset(const std::string& path, size_t type)
{
	_Offset = 0;
	bool result = _ByteReader->Reset(path, type);

	_Content.resize(_ByteReader->Bytes());

	_ByteReader->Read(&_Content.front(), _ByteReader->Bytes());

	return result;
}

bool MemoryReader::Reset(const std::vector<uint8_t>& data)
{
	_Offset  = 0;
	_Content = data;

	return true;
}

std::vector<uint8_t>& MemoryReader::Content()
{
	return _Content;
}

void MemoryReader::Read(void* buffer, size_t bytes)
{
	memcpy(buffer, &_Content[_Offset], bytes);
	_Offset += bytes;
}

uint8_t MemoryReader::u8()
{
    uint8_t val;
    memcpy((char*)&val, &_Content[_Offset], sizeof(uint8_t));
    _Offset += sizeof(uint8_t);
    return val;
}

uint16_t MemoryReader::u16()
{
    uint16_t val;
    memcpy((char*)&val, &_Content[_Offset], sizeof(uint16_t));
    _Offset += sizeof(uint16_t);
    return val;
}

int16_t MemoryReader::i16()
{
    int16_t val;
    memcpy((char*)&val, &_Content[_Offset], sizeof(int16_t));
    _Offset += sizeof(int16_t);
    return val;
}

uint32_t MemoryReader::u32()
{
    uint32_t val;
    memcpy((char*)&val, &_Content[_Offset], sizeof(uint32_t));
    _Offset += sizeof(uint32_t);
    return val;
}

int32_t MemoryReader::i32()
{
    int32_t val;
    memcpy((char*)&val, &_Content[_Offset], sizeof(int32_t));
    _Offset += sizeof(int32_t);
    return val;
}

std::string MemoryReader::string(int len)
{
    char* buffer = new char[len + 1];
    memcpy(buffer, &_Content[_Offset], len);
    buffer[len] = '\0';
    std::string val(buffer);
    delete[] buffer;
    _Offset += len;

    return val;
}

void MemoryReader::Pos(size_t value)
{
    _Offset = value;
}

size_t MemoryReader::CurrPos()
{
    return _Offset;
}

size_t MemoryReader::Bytes()
{
    return _Content.size();
}

bool MemoryReader::FindFragment(const uint8_t* fragment, uint32_t fragmentLen, uint32_t beginOffs)
{
    if (fragmentLen > Bytes())
        return false;

    for (uint32_t i = beginOffs; i < Bytes() - fragmentLen; i++)
    {
        if (_Content[i] == fragment[0])
        {
            bool notMatch = false;
            for (uint32_t j = 1; j < fragmentLen; j++)
            {
                if (_Content[i + j] != fragment[j])
                {
                    notMatch = true;
                    break;
                }
            }

            if (!notMatch)
            {
                _Offset = i;
                return true;
            }
        }
    }
    return false;
}

bool MemoryReader::Eof()
{
    return _Offset < Bytes();
}