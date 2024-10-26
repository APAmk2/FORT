#include "FallFRM.h"

FrmFrame_t::FrmFrame_t(ByteReader* reader)
{
	Width = reader->u16();
	Height = reader->u16();
	FrameSize = reader->u32();
	XShift = reader->i16();
	YShift = reader->i16();
	for (size_t i = 0, len = Width * Height; i < len; i++)
	{
		FrameData.push_back(reader->u8());
	}
}

FallFrm_t::FallFrm_t(ByteReader* reader)
{
	Version = reader->u32();
	Fps = reader->u16();
	ActionFrame = reader->u16();
	FramesPerDir = reader->u16();
	for (uint8_t i = 0; i < 6; i++)
	{
		XShift.push_back(reader->i16());
	}
	for (uint8_t i = 0; i < 6; i++)
	{
		YShift.push_back(reader->i16());
	}
	for (uint8_t i = 0; i < 6; i++)
	{
		DataShift.push_back(reader->u32());
	}
	DataSize = reader->u32();
	for (size_t i = 0, len = FramesPerDir * 6; i < len; i++)
	{
		if ((DataSize > 0 && reader->CurrPos() > DataSize) || reader->CurrPos() >= reader->Bytes())
		{
			DirCount = i / FramesPerDir;
			break;
		}
		FrmFrame_t* currFrame = new FrmFrame_t(reader);
		Frames.push_back(currFrame);
	}
}