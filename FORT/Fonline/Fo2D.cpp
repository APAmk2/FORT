#include "Fo2D.h"
#include "lodepng.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <stdio.h>
#include <filesystem>

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
		hdr->dirs = reader->u16();

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

int frameCounter = 0;

bool readFonline2D(std::filesystem::path& filename, Fo2D_t*& file)
{
	delete file;
	file = nullptr;
	frameCounter = 0;
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::BigEndian)) return false;
	file = new Fo2D_t(reader);
    file->filename = filename.stem().string();
	reader->Close();
	delete reader;

	return true;
}

void exportFonline2D(Fo2D_t*& file)
{
	for (size_t currData = 0; currData < file->hdr->dirs; currData++)
	{
		for (size_t currFrame = 0; currFrame < file->hdr->frames_count; currFrame++)
		{
			frame_t* currFrame_ptr = &file->data[currData].frames[currFrame];
			std::vector<unsigned char> image;
			
			int width = currFrame_ptr->width;
			int height = currFrame_ptr->height;
			image.resize(width * height * 4);
			for (size_t i = 0; i < width * height; i++)
			{
				image[i * 4] = currFrame_ptr->pixels[i].r;
				image[i * 4 + 1] = currFrame_ptr->pixels[i].g;
				image[i * 4 + 2] = currFrame_ptr->pixels[i].b;
				image[i * 4 + 3] = currFrame_ptr->pixels[i].a;
			}

			unsigned error = lodepng::encode((file->filename + "_" + to_string(currData) + "_" + to_string(currFrame)) + ".png", image, width, height);
			if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		}
	}
}

bool renderFonline2D(Fo2D_t* file, int& width, int& height, int& dir, SDL_Texture** Fo2DTex, SDL_Renderer* renderer)
{
	frame_t* currFrame_ptr = &file->data[dir].frames[frameCounter];
	
	SDL_DestroyTexture(*Fo2DTex);
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)&currFrame_ptr->pixels[0], currFrame_ptr->width, currFrame_ptr->height, 4 * 8, 4 * currFrame_ptr->width,
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	if (surface == nullptr) 
	{
		fprintf(stderr, "Failed to create SDL surface: %s\n", SDL_GetError());
		return false;
	}

	*Fo2DTex = SDL_CreateTextureFromSurface(renderer, surface);

	if (*Fo2DTex == nullptr) 
	{
		fprintf(stderr, "Failed to create SDL texture: %s\n", SDL_GetError());
	}

	SDL_FreeSurface(surface);
	
	width = currFrame_ptr->width;
	height = currFrame_ptr->height;
	
	frameCounter++;
	if (frameCounter >= file->data[dir].frames.size())
	{
		frameCounter = 0;
	}

	return true;
}

void Fo2DWindow::drawWindow()
{
	if (!getVisible()) return;
	ImGui::Begin("Fonline 2D Graphics Tool");

	ImGui::Text("Width:%i", Fo2Dwidth);
	ImGui::SameLine();
	ImGui::Text("Height:%i", Fo2Dheight);
	ImGui::SameLine();
	ImGui::Text("FPS:%i", (Fo2DFile != nullptr ? (Fo2DFile->hdr->anim_ticks / Fo2DFile->hdr->frames_count) / 10 : 0));
	ImGui::SameLine();
	ImGui::Text("Frames:%i/%i", frameCounter, (Fo2DFile != nullptr ? Fo2DFile->hdr->frames_count - 1: 0));
	if (ImGui::Button("<") && Fo2DFile != nullptr)
	{
		Fo2DDir--;
		if (Fo2DDir < 0) Fo2DDir = Fo2DFile->hdr->dirs - 1;
	}
	ImGui::SameLine();
	ImGui::Text("Dir:%i/%i", Fo2DDir, (Fo2DFile != nullptr ? Fo2DFile->hdr->dirs - 1 : 0));
	ImGui::SameLine();
	if (ImGui::Button(">") && Fo2DFile != nullptr)
	{
		Fo2DDir++;
		if (Fo2DDir >= Fo2DFile->hdr->dirs) Fo2DDir = 0;
	}

	ImGui::Text("Filename:%s", (Fo2DFile != nullptr ? Fo2DFile->filename.c_str() : ""));

	ImGui::InputText("FOnline 2D Graphics file path", &Fo2Dfilename);
	if (ImGui::Button("Load File"))
	{
		filesystem::path filepath = Fo2Dfilename;
		readFonline2D(filepath, Fo2DFile);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		exportFonline2D(Fo2DFile);
	}

	if (Fo2DFile != nullptr && Fo2DFPSTimer <= SDL_GetTicks())
	{
		renderFonline2D(Fo2DFile, Fo2Dwidth, Fo2Dheight, Fo2DDir, &Fo2DTex, renderer);
		Fo2DFPSTimer += Fo2DFile->hdr->anim_ticks / Fo2DFile->hdr->frames_count;
	}
	ImGui::Image((void*)Fo2DTex, ImVec2(Fo2Dwidth, Fo2Dheight));
	ImGui::End();
}

void Fo2DWindow::initWindow()
{}