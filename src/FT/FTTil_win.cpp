#include "FTTil_win.h"
#include "lodepng.h"
#include "../FORT.h"
#include <filesystem>

int FTTilFrameCounter = 0;

bool ReadFTTil(std::filesystem::path& filename, FTTil_t*& file)
{
	delete file;
	file = nullptr;
	FTTilFrameCounter = 0;
	ByteReader* fileReader = new ByteReader;
	MemoryReader* reader = new MemoryReader(fileReader);
	if (!reader->Reset(filename.string(), ByteReader::LittleEndian)) return false;
	file = new FTTil_t(reader);
	file->Filename = filename.stem().string();
	reader->Close();
	delete reader;

	return true;
}

void ExportFTTil(FTTil_t*& file)
{
	for (size_t currFrame = 0; currFrame < file->FrameCount; currFrame++)
	{
		FTZar_t* currFramePtr = file->ZarFrames[currFrame];
		std::vector<unsigned char> image;

		int width = currFramePtr->Width;
		int height = currFramePtr->Height;
		image.resize(width * height * 4);
		for (size_t i = 0; i < width * height; i++)
		{
			image[i * 4] = currFramePtr->Pixels[i].r;
			image[i * 4 + 1] = currFramePtr->Pixels[i].g;
			image[i * 4 + 2] = currFramePtr->Pixels[i].b;
			image[i * 4 + 3] = currFramePtr->Pixels[i].a;
		}

		unsigned error = lodepng::encode((file->Filename + "_" + std::to_string(currFrame) + ".png"), image, width, height);
		if (error) ImGui::DebugLog("encoder error %i:%s\n", error, lodepng_error_text(error));

	}
}

bool RenderFTTil(FTTil_t* file, uint16_t& width, uint16_t& height, SDL_Texture** Fo2DTex, SDL_Renderer* renderer)
{
	FTZar_t* currFramePtr = file->ZarFrames[FTTilFrameCounter];

	SDL_DestroyTexture(*Fo2DTex);
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)&currFramePtr->Pixels[0], file->Width, file->Height, 4 * 8, 4 * file->Width,
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

	width = file->Width;
	height = file->Height;

	FTTilFrameCounter++;
	if (FTTilFrameCounter >= file->ZarFrames.size())
	{
		FTTilFrameCounter = 0;
	}

	return true;
}

void FTTilWindow::DrawWin()
{
	if (!GetVisible()) return;
	ImGui::Begin("Fallout:Tactics .til Graphics Tool", &Visible);

	ImGui::Text("Width:%i", Width);
	ImGui::SameLine();
	ImGui::Text("Height:%i", Height);
	ImGui::Text("FPS:10");
	ImGui::SameLine();
	ImGui::Text("Frames:%i/%i", FTTilFrameCounter, (File != nullptr ? File->FrameCount - 1 : 0));
	ImGui::Text("Filename:%s", (File != nullptr ? File->Filename.c_str() : ""));

	ImGui::InputText("Fallout:Tactics .til file path", &Filename);
	if (ImGui::Button("Load File"))
	{
		std::filesystem::path filepath = Filename;
		ReadFTTil(filepath, File);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		ExportFTTil(File);
	}

	if (File != nullptr && FPSTimer <= SDL_GetTicks())
	{
		RenderFTTil(File, Width, Height, &Tex, Renderer);
		FPSTimer = SDL_GetTicks() + (1000 / 10);
	}

	ImGui::Image((void*)Tex, ImVec2(Width, Height));

	ImGui::End();
}

void FTTilWindow::InitWin()
{
	ImGui::DebugLog("Initializing Fallout:Tactics .til Tool...\n");
	ImGui::DebugLog("Fallout:Tactics .til Tool Init Done.\n");
}

void FTTilWindow::DestroyWin() { }

void FTTilWindow::ProcessMenuBtn()
{
	if (ImGui::BeginMenu("[Fallout:Tactics]"))
	{
		if (ImGui::MenuItem("Fallout:Tactics .til")) { this->SetVisible(!this->GetVisible()); }
		ImGui::EndMenu();
	}
}