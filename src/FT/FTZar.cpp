#include "FTZar.h"
#include <stdio.h>

FTZar_t::FTZar_t(MemoryReader* reader)
{
    // Read header
    std::string head = reader->string(6);
    if (head[5] || head != "<zar>")
        return;
    Type = reader->u8();
    reader->Pos(reader->CurrPos() + 1);   // \0
    Width = reader->u32();
    Height = reader->u32();
    PalettePresent = reader->u8();
    printf("Header: %s\n", head.c_str());
    printf("Type: %i\n", Type);
    printf("Width: %i\n", Width);
    printf("Height: %i\n", Height);
    printf("PalettePresent: %i\n", PalettePresent);

    // Read palette
    if (PalettePresent)
    {
        PaletteCount = reader->u32();
        if (PaletteCount > 256)
            return;
        Palette.resize(PaletteCount);
        for (size_t i = 0; i < PaletteCount; i++)
        {
            uint8_t b = reader->u8();
            uint8_t g = reader->u8();
            uint8_t r = reader->u8();
            uint8_t a = reader->u8();
            Palette[i] = ColorRGBA(r, g, b, a);
        }
        if (Type == 0x34)
            DefColor = reader->u8();
    }

    // Read image
    uint32_t rleSize = reader->u32();
    printf("rleSize: %i\n", rleSize);
    std::vector<uint8_t> rleBuffer;
    rleBuffer.resize(reader->Bytes() - reader->CurrPos());
    uint8_t* rleBuf = &rleBuffer[0];
    reader->Read(&rleBuffer[0], reader->Bytes() - reader->CurrPos());
    reader->Pos(reader->CurrPos() + rleSize);

    // Decode
    while (rleSize)
    {
        int control = *rleBuf;
        rleBuf++;
        rleSize--;

        int controlMode = (control & 3);
        int controlCount = (control >> 2);

        for (int i = 0; i < controlCount; i++)
        {
            ColorRGBA col(0, 0, 0, 0);
            ColorRGBA col2Push(col.r, col.g, col.b, rleBuf[2 * i + 1]);
            switch (controlMode)
            {
            case 1:
                col = Palette[rleBuf[i]];
                col2Push = ColorRGBA(col.r, col.g, col.b, 0xFF);
                Pixels.push_back(col2Push);
                break;
            case 2:
                col = Palette[rleBuf[2 * i]];
                col2Push = ColorRGBA(col.r, col.g, col.b, rleBuf[2 * i + 1]);
                Pixels.push_back(col2Push);
                break;
            case 3:
                col = Palette[DefColor];
                col2Push = ColorRGBA(col.r, col.g, col.b, rleBuf[i]);
                Pixels.push_back(col2Push);
                break;
            default:
                break;
            }
        }

        if (controlMode)
        {
            rleSize -= controlCount;
            rleBuf += controlCount;
            if (controlMode == 2)
            {
                rleSize -= controlCount;
                rleBuf += controlCount;
            }
        }
    }
}