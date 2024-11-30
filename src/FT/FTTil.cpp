#include "FTTil.h"

FTTil_t::FTTil_t(MemoryReader* reader)
{
    std::string head = reader->string(7);
    if (head[6] || head != "<tile>")
        return;
    while (reader->u8())
        ;
    reader->Pos(reader->CurrPos() + 7 + 4); // Unknown
    Width = reader->u32();
    Height = reader->u32();

    if (!reader->FindFragment((uint8_t*)"<tiledata>", 10, reader->CurrPos()))
        return;
    reader->Pos(reader->CurrPos() + 10 + 3); // Signature
    FrameCount = reader->u32();

    for (uint32_t frm = 0; frm < FrameCount; frm++)
    {
        FTZar_t* frame = new FTZar_t(reader);
        reader->string(8);
        ZarFrames.push_back(frame);
    }
}