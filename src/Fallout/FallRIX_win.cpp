#include "FallRIX_win.h"
#include "imgui.h"
#include "lodepng.h"
#include "imgui_stdlib.h"
#include <filesystem>

bool ReadFallRIX(std::filesystem::path& filename, FallRIX_t*& file)
{
	delete file;
	file = nullptr;
	ByteReader* fileReader = new ByteReader;
	MemoryReader* reader = new MemoryReader(fileReader);
	if (!reader->Reset(filename.string(), ByteReader::LittleEndian)) return false;
	file = new FallRIX_t(reader);
	file->Filename = filename.stem().string();
	reader->Close();
	delete reader;

	return true;
}

void ExportFallRIX(FallRIX_t*& file)
{
	std::vector<unsigned char> image;

	int width = file->Width;
	int height = file->Height;
	image.resize(width * height * 4);
	for (size_t i = 0; i < width * height; i++)
	{
		ColorRGB* currPixel = &file->Palette[file->Pixels[i]];
		image[i * 4] = currPixel->r * 4;
		image[i * 4 + 1] = currPixel->g * 4;
		image[i * 4 + 2] = currPixel->b * 4;
		image[i * 4 + 3] = 255;
	}

	unsigned error = lodepng::encode((file->Filename + ".png"), image, width, height);
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

bool RenderFallRIX(FallRIX_t* file, uint16_t& width, uint16_t& height, SDL_Texture** fallRIXTex, SDL_Renderer* renderer)
{
	std::vector<uint8_t> image;
	size_t imgLen = (size_t)file->Width * (size_t)file->Height;
	image.resize(imgLen * 4);
	for (size_t i = 0; i < imgLen; i++)
	{
		ColorRGB* currPixel = &file->Palette[file->Pixels[i]];
		image[i * 4] = currPixel->r * 4;
		image[i * 4 + 1] = currPixel->g * 4;
		image[i * 4 + 2] = currPixel->b * 4;
		image[i * 4 + 3] = 255;
	}

	SDL_DestroyTexture(*fallRIXTex);
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)&image[0], file->Width, file->Height, 4 * 8, 4 * file->Width,
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	if (surface == nullptr)
	{
		fprintf(stderr, "Failed to create SDL surface: %s\n", SDL_GetError());
		return false;
	}

	*fallRIXTex = SDL_CreateTextureFromSurface(renderer, surface);

	if (*fallRIXTex == nullptr)
	{
		fprintf(stderr, "Failed to create SDL texture: %s\n", SDL_GetError());
	}

	SDL_FreeSurface(surface);

	width = file->Width;
	height = file->Height;

	return true;
}

void FallRIXWindow::DrawWin()
{
	if (!GetVisible()) return;
	ImGui::Begin("Fallout .rix Graphics Tool");

	ImGui::Text("Width:%i", Width);
	ImGui::SameLine();
	ImGui::Text("Height:%i", Height);
	ImGui::Text("Filename:%s", (File != nullptr ? File->Filename.c_str() : ""));

	ImGui::InputText("Fallout .rix file path", &Filename);
	if (ImGui::Button("Load File"))
	{
		std::filesystem::path filepath = Filename;
		ReadFallRIX(filepath, File);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		ExportFallRIX(File);
	}

	if (File != nullptr)
	{
		RenderFallRIX(File, Width, Height, &Tex, Renderer);
		ImGui::Image((void*)Tex, ImVec2(Width, Height));
	}

	ImGui::End();
}

void FallRIXWindow::InitWin()
{}