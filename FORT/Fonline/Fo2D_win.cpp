#include "Fo2D_win.h"
#include "imgui.h"
#include "lodepng.h"
#include "imgui_stdlib.h"
#include <stdio.h>
#include <filesystem>

using namespace std;

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