#include "foart.h"
#include "SDL_opengl.h"

using namespace std;

void file_t::read(ByteReader* reader)
{
	check_num1 = reader->u8();
	if (check_num1 != 42) return;
	frames_count = reader->u16();
	anim_ticks = reader->u16();
	dirs = reader->u8();

	for (size_t i = 0; i < dirs; i++)
	{
		data_t currData;
		currData.hdr_ptr = this;
		currData.read(reader);
		data.push_back(currData);
	}
}

void data_t::read(ByteReader* reader)
{
	offs_x = reader->i16();
	offs_y = reader->i16();
	for (size_t i = 0; i < hdr_ptr->frames_count; i++)
	{
		frame_t currFrame;
		currFrame.read(reader);
		frames.push_back(currFrame);
	}
}

void frame_t::read(ByteReader* reader)
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

void oldfile_t::read(ByteReader* reader)
{
	width = reader->u32();
	height = reader->u32();
	if(width == 1196314761) return;
	for (size_t i = 0, len = width * height; i < len; i++)
	{
		ucolor currPixel{ reader->u8(), reader->u8(), reader->u8(), reader->u8() };
		pixels.push_back(currPixel);
	}
}

bool decodeFonline2D(filesystem::path& filename, int& width, int& height)
{
	ByteReader* reader = new ByteReader;
	reader->Reset(filename.string(), ByteReader::BigEndian);
	file_t file;
	file.read(reader);

	if (file.check_num1 != 42) return false;

	frame_t* currFrame_ptr = &file.data[0].frames[0];
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, currFrame_ptr->width, currFrame_ptr->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &currFrame_ptr->pixels[0]);
	width = currFrame_ptr->width;
	height = currFrame_ptr->height;

	return true;
}

void decodeOldFonline2D(filesystem::path& filename, int& width, int& height)
{
	ByteReader* reader = new ByteReader;
	reader->Reset(filename.string(), ByteReader::BigEndian);
	oldfile_t file;
	file.read(reader);
	if (file.width == 1196314761) return;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, file.width, file.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &file.pixels[0]);
	width = file.width;
	height = file.height;
}