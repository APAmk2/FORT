#include "Fo2D.h"
#include <stdio.h>

using namespace std;

#define DIRS_COUNT 6

hdr_t::hdr_t(ByteReader* reader)
{
	frames_count = reader->u16();
	anim_ticks = reader->u16();
	dirs = reader->u8();
}

data_t::data_t(ByteReader* reader, hdr_t* hdr_ptr)
{
	offs_x = reader->i16();
	offs_y = reader->i16();
	for (size_t i = 0; i < hdr_ptr->frames_count; i++)
	{
		frame_t currFrame(reader);
		frames.push_back(currFrame);
	}
}

frame_t::frame_t(ByteReader* reader)
{
	is_shared = reader->u8() > 0;
	if(!is_shared)
	{
		width = reader->u16();
		height = reader->u16();
		next_x = reader->i16();
		next_y = reader->i16();
		for (size_t i = 0, len = width * height; i < len; i++)
		{
			ucolor currPixel{ reader->u8(), reader->u8(), reader->u8(), reader->u8() };
			pixels.push_back(currPixel);
		}
	}
	else
	{
		shared_indx = reader->u16();
	}
}

Fo2D_t::Fo2D_t(ByteReader* reader)
{
	uint8_t check_num = reader->u8();
	reader->Pos(0);
	uint32_t animSign = reader->u32();
	reader->Pos(1);
	if(check_num == 42 && animSign != 0xDEADBEEF)
	{
		hdr = new hdr_t(reader);

		for (size_t i = 0; i < hdr->dirs; i++)
		{
			data_t currData(reader, hdr);
			data.push_back(currData);
		}
	}
	else if(check_num != 137 && animSign != 0xDEADBEEF)
	{
		reader->Pos(0);

		hdr = new hdr_t;
		hdr->frames_count = 1;
		hdr->anim_ticks = 1;
		hdr->dirs = 1;

		data_t newData;
		newData.hdr_ptr = hdr;

		frame_t newFrame;
		newFrame.width = reader->u32();
		newFrame.height = reader->u32();
		//if (newFrame.width == 1196314761) return;
		for (size_t i = 0, len = newFrame.width * newFrame.height; i < len; i++)
		{
			ucolor currPixel{ reader->u8(), reader->u8(), reader->u8(), reader->u8() };
			newFrame.pixels.push_back(currPixel);
		}
		newData.frames.push_back(newFrame);
		data.push_back(newData);
	}
	else if (animSign == 0xDEADBEEF)
	{
		reader->Pos(4);
		hdr = new hdr_t;
		hdr->frames_count = reader->u16();
		hdr->anim_ticks = reader->u32();
		hdr->dirs = reader->u8();

		for (uint16_t dir = 0; dir < hdr->dirs; dir++)
		{
			data_t newData;
			newData.hdr_ptr = hdr;
			for (uint16_t i = 0; i < hdr->frames_count; i++)
			{
				frame_t newFrame;
				newFrame.width = reader->u16();
				newFrame.height = reader->u16();
				newData.offs_x = reader->u16();
				newData.offs_y = reader->u16();
				newFrame.next_x = reader->u16();
				newFrame.next_y = reader->u16();

				for (size_t i = 0, len = newFrame.width * newFrame.height; i < len; i++)
				{
					ucolor currPixel{ reader->u8(), reader->u8(), reader->u8(), reader->u8() };
					newFrame.pixels.push_back(currPixel);
				}
			}
		}
	}
}