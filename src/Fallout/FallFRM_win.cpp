#include "FallFRM_win.h"
#include "imgui.h"
#include "lodepng.h"
#include "imgui_stdlib.h"
#include <stdio.h>
#include <filesystem>

using namespace std;

static int FRMFrameCount = 0;
static std::vector<ucolor> FRMpalette;

bool SetupPalette(const std::filesystem::path& filename)
{
	FRMpalette.resize(0);
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::BigEndian)) return false;

	for (size_t i = 0; i < 256; i++)
	{
		ucolor currColor;
		currColor.r = reader->u8();
		currColor.g = reader->u8();
		currColor.b = reader->u8();
		FRMpalette.push_back(currColor);
	}

	reader->Close();
	delete reader;

	return true;
}

bool readFRM(std::filesystem::path& filename, FallFrm_t*& file)
{
	delete file;
	file = nullptr;
	FRMFrameCount = 0;
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::BigEndian)) return false;
	file = new FallFrm_t(reader);
	reader->Close();
	delete reader;

	return true;
}

void exportFRM(FallFrm_t*& file, const std::string& filename)
{
	for (uint16_t dir = 0; dir < 6; dir++)
	{
		for (uint16_t frame = 0, len = file->framesPerDir; frame < len; frame++)
		{
			uint16_t destInd = (dir * file->framesPerDir) + frame;
			if (destInd >= file->frames.size())
			{
				break;
			}
			FrmFrame_t* currFrame = file->frames[destInd];

			std::vector<uint8_t> image;
			size_t imglen = (size_t)currFrame->width * (size_t)currFrame->height;
			image.resize(imglen * 4);
			for (size_t i = 0; i < imglen; i++)
			{
				image[i * 4] = FRMpalette[currFrame->frameData[i]].r * 4; //Red
				image[i * 4 + 1] = FRMpalette[currFrame->frameData[i]].g * 4; //Green
				image[i * 4 + 2] = FRMpalette[currFrame->frameData[i]].b * 4; //Blue
				image[i * 4 + 3] = (!currFrame->frameData[i] ? 0 : 255);//Alpha
			}

			std::filesystem::path path = filename;
			path.replace_extension(std::to_string(dir) + "_" + std::to_string(frame) + ".png");
			unsigned error = lodepng::encode(path.string(), image, currFrame->width, currFrame->height, LCT_RGBA, 8);
			if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		}
	}
}

bool renderFRM(FallFrm_t* file, int& width, int& height, int& dir, SDL_Texture** FRMTex, SDL_Renderer* renderer)
{
	uint16_t destInd = (dir * file->framesPerDir) + FRMFrameCount;
	if (destInd >= file->frames.size())
	{
		return false;
	}

	FrmFrame_t* currFrame_ptr = file->frames[destInd];

	SDL_DestroyTexture(*FRMTex);

	std::vector<uint8_t> dataToWrite;
	for (uint32_t i = 0, len = currFrame_ptr->width * currFrame_ptr->height; i < len; i++)
	{
		ucolor *currColor = &FRMpalette[currFrame_ptr->frameData[i]];
		dataToWrite.push_back(currColor->r * 4);
		dataToWrite.push_back(currColor->g * 4);
		dataToWrite.push_back(currColor->b * 4);
		dataToWrite.push_back(!currFrame_ptr->frameData[i] ? 0 : 255);
	}

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)&dataToWrite[0], currFrame_ptr->width, currFrame_ptr->height, 4 * 8, 4 * currFrame_ptr->width,
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	if (surface == nullptr)
	{
		fprintf(stderr, "Failed to create SDL surface: %s\n", SDL_GetError());
		return false;
	}

	*FRMTex = SDL_CreateTextureFromSurface(renderer, surface);

	if (*FRMTex == nullptr)
	{
		fprintf(stderr, "Failed to create SDL texture: %s\n", SDL_GetError());
	}

	SDL_FreeSurface(surface);

	width = currFrame_ptr->width;
	height = currFrame_ptr->height;

	FRMFrameCount++;
	if (FRMFrameCount >= file->framesPerDir)
	{
		FRMFrameCount = 0;
	}

	return true;
}

void FallFRMWindow::drawWindow()
{
	if (!getVisible()) return;
	ImGui::Begin("Fonline 2D Graphics Tool");

	ImGui::Text("Width:%i", FRMwidth);
	ImGui::SameLine();
	ImGui::Text("Height:%i", FRMheight);
	ImGui::SameLine();
	ImGui::Text("FPS:%i", (FRMFile != nullptr ? FRMFile->fps : 0));
	ImGui::SameLine();
	ImGui::Text("Frames:%i/%i", FRMFrameCount + 1, (FRMFile != nullptr ? FRMFile->framesPerDir : 0));
	if (ImGui::Button("<") && FRMFile != nullptr)
	{
		FRMDir--;
		if (FRMDir < 0) FRMDir = FRMFile->dirs - 1;
	}
	ImGui::SameLine();
	ImGui::Text("Dir:%i/%i", FRMDir + 1, (FRMFile != nullptr ? FRMFile->dirs : 0));
	ImGui::SameLine();
	if (ImGui::Button(">") && FRMFile != nullptr)
	{
		FRMDir++;
		if (FRMDir >= FRMFile->dirs) FRMDir = 0;
	}

	ImGui::InputText("FOnline 2D Graphics file path", &FRMfilename);
	if (ImGui::Button("Load File"))
	{
		std::filesystem::path filepath = FRMfilename;
		std::filesystem::path palPath = filepath;
		palPath.replace_extension(".pal");
		if(!SetupPalette(palPath.string()))
		{
			if (!SetupPalette("Fallout/color.pal"))
			{
				return;
			}
		}

		readFRM(filepath, FRMFile);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		exportFRM(FRMFile, FRMfilename);
	}

	if (FRMFile != nullptr && FRMFPSTimer <= SDL_GetTicks())
	{
		renderFRM(FRMFile, FRMwidth, FRMheight, FRMDir, &FRMTex, renderer);
		if(FRMFile->fps != 0) FRMFPSTimer = SDL_GetTicks() + (1000 / FRMFile->fps);
	}
	ImGui::Image((void*)FRMTex, ImVec2(FRMwidth, FRMheight));
	ImGui::End();
}

void FallFRMWindow::initWindow()
{}