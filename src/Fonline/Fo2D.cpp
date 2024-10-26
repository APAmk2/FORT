#include "Fo2D.h"

#define DIRS_COUNT 6

Fo2DFrame_t::Fo2DFrame_t(ByteReader* reader)
{
	Shared = reader->u8();
	if(!Shared)
	{
		Width = reader->u16();
		Height = reader->u16();
		NextX = reader->i16();
		NextY = reader->i16();
		for (size_t i = 0, len = Width * Height; i < len; i++)
		{
			ColorRGBA currPixel{ reader->u8(), reader->u8(), reader->u8(), reader->u8() };
			Pixels.push_back(currPixel);
		}
	}
	else
	{
		SharedIndex = reader->u16();
	}
}

Fo2DData_t::Fo2DData_t(ByteReader* reader, uint16_t frameCount)
{
	XOffs = reader->i16();
	YOffs = reader->i16();
	for (size_t i = 0; i < frameCount; i++)
	{
		Fo2DFrame_t currFrame(reader);
		Frames.push_back(currFrame);
	}
}

Fo2D_t::Fo2D_t(ByteReader* reader)
{
	uint8_t checkNum = reader->u8();
	reader->Pos(0);
	uint32_t animSign = reader->u32();
	reader->Pos(1);
	if(checkNum == 42 && animSign != 0xDEADBEEF) // Latest Fo2D format
	{
		FrameCount = reader->u16();
		AnimTicks = reader->u16();
		DirCount = reader->u8();

		for (size_t i = 0; i < DirCount; i++)
		{
			Fo2DData_t currData(reader, FrameCount);
			Data.push_back(currData);
		}
	}
	else if(checkNum != 137 && animSign != 0xDEADBEEF) // FOnline 2 (SDK Rev. 840) format
	{
		reader->Pos(0);
		FrameCount = 1;
		AnimTicks = 1;
		DirCount = 1;

		Fo2DData_t newData;
		Fo2DFrame_t newFrame;
		newFrame.Width = reader->u32();
		newFrame.Height = reader->u32();
		//if (newFrame.width == 1196314761) return;
		for (size_t i = 0, len = newFrame.Width * newFrame.Height; i < len; i++)
		{
			ColorRGBA currPixel{ reader->u8(), reader->u8(), reader->u8(), reader->u8() };
			newFrame.Pixels.push_back(currPixel);
		}
		newData.Frames.push_back(newFrame);
		Data.push_back(newData);
	}
	else if (animSign == 0xDEADBEEF) // FOnline 2 (SDK Rev. 840) "Fast" format
	{
		reader->Pos(4);
		FrameCount = reader->u16();
		AnimTicks = reader->u32();
		DirCount = reader->u8();

		for (uint8_t dir = 0; dir < DirCount; dir++)
		{
			Fo2DData_t newData;
			for (uint16_t i = 0; i < FrameCount; i++)
			{
				Fo2DFrame_t newFrame;
				newFrame.Width = reader->u16();
				newFrame.Height = reader->u16();
				newData.XOffs = reader->u16();
				newData.YOffs = reader->u16();
				newFrame.NextX = reader->u16();
				newFrame.NextY = reader->u16();

				for (size_t i = 0, len = newFrame.Width * newFrame.Height; i < len; i++)
				{
					ColorRGBA currPixel{ reader->u8(), reader->u8(), reader->u8(), reader->u8() };
					newFrame.Pixels.push_back(currPixel);
				}
			}
		}
	}
}