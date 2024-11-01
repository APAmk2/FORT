#include "FallMSK_win.h"
#include "imgui.h"
#include "lodepng.h"
#include "imgui_stdlib.h"
#include <filesystem>

bool ReadFallMSK(std::filesystem::path& filename, FallMSK_t*& file)
{
	delete file;
	file = nullptr;
	ByteReader* fileReader = new ByteReader;
	MemoryReader* reader = new MemoryReader(fileReader);
	if (!reader->Reset(filename.string(), ByteReader::LittleEndian)) return false;
	file = new FallMSK_t(reader);
	file->Filename = filename.stem().string();
	reader->Close();
	delete reader;

	return true;
}

void ExportFallMSK(FallMSK_t*& file)
{
	unsigned error = lodepng::encode((file->Filename + ".png"), file->PixelMask, MSK_WIDTH, MSK_HEIGHT, LCT_GREY, 1);
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

bool RenderFallMSK(FallMSK_t* file, SDL_Texture** fallMSKTex, SDL_Renderer* renderer)
{
	SDL_DestroyTexture(*fallMSKTex);
	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom((void*)&file->PixelMask[0], MSK_WIDTH, MSK_HEIGHT, 1, MSK_WIDTH / 8, SDL_PIXELFORMAT_INDEX1MSB);
	SDL_Color colors[2] = { {0, 0, 0, 255}, {255, 255, 255, 255} };
	SDL_SetPaletteColors(surface->format->palette, colors, 0, 2);

	if (surface == nullptr)
	{
		fprintf(stderr, "Failed to create SDL surface: %s\n", SDL_GetError());
		return false;
	}

	*fallMSKTex = SDL_CreateTextureFromSurface(renderer, surface);

	if (*fallMSKTex == nullptr)
	{
		fprintf(stderr, "Failed to create SDL texture: %s\n", SDL_GetError());
	}

	SDL_FreeSurface(surface);

	return true;
}

void FallMSKWindow::DrawWin()
{
	if (!GetVisible()) return;
	ImGui::Begin("Fallout .msk Graphics Tool");

	ImGui::Text("Filename:%s", (File != nullptr ? File->Filename.c_str() : ""));

	ImGui::InputText("Fallout .msk file path", &Filename);
	if (ImGui::Button("Load File"))
	{
		std::filesystem::path filepath = Filename;
		ReadFallMSK(filepath, File);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		ExportFallMSK(File);
	}

	if (File != nullptr)
	{
		RenderFallMSK(File, &Tex, Renderer);
		ImGui::Image((void*)Tex, ImVec2(MSK_WIDTH, MSK_HEIGHT));
	}

	ImGui::End();
}

void FallMSKWindow::InitWin()
{}