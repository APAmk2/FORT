#include "FallRIX.h"

FallRIX_t::FallRIX_t(MemoryReader* reader)
{
    // Read header
    std::string head = reader->string(4);
    if (head != "RIX3")
        return;
    Width = reader->u16();
    Height = reader->u16();
    if (Width != 640 || Height != 480)
        return;
    PalType = reader->u8();
    StoreType = reader->u8();
    if (PalType != 0xAF || StoreType != 0x00)
        return;
    for (uint16_t i = 0; i < 256; i++)
    {
        ColorRGB currColor;
        currColor.r = reader->u8();
        currColor.g = reader->u8();
        currColor.b = reader->u8();
        Palette.push_back(currColor);
    }

    for (size_t i = 0, len = 640 * 480; i < len; i++)
    {
        Pixels.push_back(reader->u8());
    }
}