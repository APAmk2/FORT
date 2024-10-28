#include "FallFRM_win.h"
#include "imgui.h"
#include "lodepng.h"
#include "imgui_stdlib.h"
#include <stdio.h>
#include <filesystem>

static int FrameCount = 0;
static std::vector<ColorRGB> Palette;

bool SetupPalette(const std::filesystem::path& filename)
{
	Palette.resize(0);
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::BigEndian)) return false;

	for (size_t i = 0; i < 256; i++)
	{
		ColorRGB currColor;
		currColor.r = reader->u8();
		currColor.g = reader->u8();
		currColor.b = reader->u8();
		Palette.push_back(currColor);
	}

	reader->Close();
	delete reader;

	return true;
}

bool ReadFRM(std::filesystem::path& filename, FallFrm_t*& file)
{
	delete file;
	file = nullptr;
	FrameCount = 0;
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::BigEndian)) return false;
	file = new FallFrm_t(reader);
	reader->Close();
	delete reader;

	return true;
}

void ExportFRM(FallFrm_t*& file, const std::string& filename)
{
	for (uint16_t dir = 0; dir < 6; dir++)
	{
		for (uint16_t frame = 0, len = file->FramesPerDir; frame < len; frame++)
		{
			uint16_t destInd = (dir * file->FramesPerDir) + frame;
			if (destInd >= file->Frames.size())
			{
				break;
			}
			FrmFrame_t* currFrame = file->Frames[destInd];

			std::vector<uint8_t> image;
			size_t imgLen = (size_t)currFrame->Width * (size_t)currFrame->Height;
			image.resize(imgLen * 4);
			for (size_t i = 0; i < imgLen; i++)
			{
				image[i * 4] = Palette[currFrame->FrameData[i]].r * 4; //Red
				image[i * 4 + 1] = Palette[currFrame->FrameData[i]].g * 4; //Green
				image[i * 4 + 2] = Palette[currFrame->FrameData[i]].b * 4; //Blue
				image[i * 4 + 3] = (!currFrame->FrameData[i] ? 0 : 255);//Alpha
			}

			std::filesystem::path path = filename;
			path.replace_extension(std::to_string(dir) + "_" + std::to_string(frame) + ".png");
			unsigned error = lodepng::encode(path.string(), image, currFrame->Width, currFrame->Height, LCT_RGBA, 8);
			if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		}
	}
}

bool RenderFRM(FallFrm_t* file, uint16_t& width, uint16_t& height, int16_t& dir, SDL_Texture** FRMTex, SDL_Renderer* renderer)
{
	uint16_t destInd = (dir * file->FramesPerDir) + FrameCount;
	if (destInd >= file->Frames.size())
	{
		return false;
	}

	FrmFrame_t* currFrame = file->Frames[destInd];

	SDL_DestroyTexture(*FRMTex);

	std::vector<uint8_t> dataToWrite;
	for (uint32_t i = 0, len = currFrame->Width * currFrame->Height; i < len; i++)
	{
		ColorRGB *currColor = &Palette[currFrame->FrameData[i]];
		dataToWrite.push_back(currColor->r * 4);
		dataToWrite.push_back(currColor->g * 4);
		dataToWrite.push_back(currColor->b * 4);
		dataToWrite.push_back(!currFrame->FrameData[i] ? 0 : 255);
	}

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)&dataToWrite[0], currFrame->Width, currFrame->Height, 4 * 8, 4 * currFrame->Width,
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

	width = currFrame->Width;
	height = currFrame->Height;

	FrameCount++;
	if (FrameCount >= file->FramesPerDir)
	{
		FrameCount = 0;
	}

	return true;
}

void FallFRMWindow::DrawWin()
{
	if (!GetVisible()) return;
	ImGui::Begin("Fallout FRM Graphics Tool");

	ImGui::Text("Width:%i", Width);
	ImGui::SameLine();
	ImGui::Text("Height:%i", Height);
	ImGui::SameLine();
	ImGui::Text("FPS:%i", (File != nullptr ? File->Fps : 0));
	ImGui::SameLine();
	ImGui::Text("Frames:%i/%i", FrameCount + 1, (File != nullptr ? File->FramesPerDir : 0));
	uint16_t frameInd = (File != nullptr ? (Dir * File->FramesPerDir) + FrameCount : 0);
	ImGui::Text("X Shift:%i + %i", (File != nullptr ? File->XShift[Dir] : 0), (File != nullptr ? File->Frames[frameInd]->XShift : 0));
	ImGui::Text("Y Shift:%i + %i", (File != nullptr ? File->YShift[Dir] : 0), (File != nullptr ? File->Frames[frameInd]->YShift : 0));
	if (ImGui::Button("<") && File != nullptr)
	{
		Dir--;
		if (Dir < 0) Dir = File->DirCount - 1;
	}
	ImGui::SameLine();
	ImGui::Text("Dir:%i/%i", Dir + 1, (File != nullptr ? File->DirCount : 0));
	ImGui::SameLine();
	if (ImGui::Button(">") && File != nullptr)
	{
		Dir++;
		if (Dir >= File->DirCount) Dir = 0;
	}

	ImGui::InputText("Fallout .FRM file path", &Filename);
	if (ImGui::Button("Load File"))
	{
		std::filesystem::path filepath = Filename;
		std::filesystem::path palPath = filepath;
		palPath.replace_extension(".pal");
		if(!SetupPalette(palPath.string()))
		{
			if (!SetupPalette("Fallout/color.pal"))
			{
				return;
			}
		}

		ReadFRM(filepath, File);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		ExportFRM(File, Filename);
	}

	if (File != nullptr && FPSTimer <= SDL_GetTicks())
	{
		RenderFRM(File, Width, Height, Dir, &Tex, Renderer);
		if(File->Fps != 0) FPSTimer = SDL_GetTicks() + (1000 / File->Fps);
	}
	ImGui::Image((void*)Tex, ImVec2(Width, Height));
	ImGui::End();
}

void FallFRMWindow::InitWin()
{}