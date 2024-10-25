#include "FallFRM.h"

FrmFrame_t::FrmFrame_t(ByteReader* reader)
{
	width = reader->u16();
	height = reader->u16();
	frameSize = reader->u32();
	xShift = reader->i16();
	yShift = reader->i16();
	for (size_t i = 0, len = width * height; i < len; i++)
	{
		frameData.push_back(reader->u8());
	}
}

FallFrm_t::FallFrm_t(ByteReader* reader)
{
	version = reader->u32();
	fps = reader->u16();
	actionFrame = reader->u16();
	framesPerDir = reader->u16();
	for (uint8_t i = 0; i < 6; i++)
	{
		xShift.push_back(reader->i16());
	}
	for (uint8_t i = 0; i < 6; i++)
	{
		yShift.push_back(reader->i16());
	}
	for (uint8_t i = 0; i < 6; i++)
	{
		dataShift.push_back(reader->u32());
	}
	dataSize = reader->u32();
	for (size_t i = 0, len = framesPerDir * 6; i < len; i++)
	{
		if ((dataSize > 0 && reader->CurrPos() > dataSize) || reader->CurrPos() >= reader->Bytes())
		{
			dirs = i / framesPerDir;
			break;
		}
		FrmFrame_t* currFrame = new FrmFrame_t(reader);
		frames.push_back(currFrame);
	}
}