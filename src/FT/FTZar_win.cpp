#include "FTZar_win.h"
#include "imgui.h"
#include "lodepng.h"
#include "imgui_stdlib.h"
#include <filesystem>

bool ReadFTZar(std::filesystem::path& filename, FTZar_t*& file)
{
	delete file;
	file = nullptr;
	ByteReader* fileReader = new ByteReader;
	MemoryReader* reader = new MemoryReader(fileReader);
	if (!reader->Reset(filename.string(), ByteReader::LittleEndian)) return false;
	file = new FTZar_t(reader);
	file->Filename = filename.stem().string();
	reader->Close();
	delete reader;

	return true;
}

void ExportFTZar(FTZar_t*& file)
{
	std::vector<unsigned char> image;

	int width = file->Width;
	int height = file->Height;
	image.resize(width * height * 4);
	for (size_t i = 0; i < width * height; i++)
	{
		image[i * 4] = file->Pixels[i].r;
		image[i * 4 + 1] = file->Pixels[i].g;
		image[i * 4 + 2] = file->Pixels[i].b;
		image[i * 4 + 3] = file->Pixels[i].a;
	}

	unsigned error = lodepng::encode((file->Filename + ".png"), image, width, height);
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

bool RenderFTZar(FTZar_t* file, uint16_t& width, uint16_t& height, SDL_Texture** Fo2DTex, SDL_Renderer* renderer)
{
	SDL_DestroyTexture(*Fo2DTex);
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)&file->Pixels[0], file->Width, file->Height, 4 * 8, 4 * file->Width,
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

	return true;
}

void FTZarWindow::DrawWin()
{
	if (!GetVisible()) return;
	ImGui::Begin("Fallout:Tactics .zar Graphics Tool");

	ImGui::Text("Width:%i", Width);
	ImGui::SameLine();
	ImGui::Text("Height:%i", Height);
	ImGui::Text("Filename:%s", (File != nullptr ? File->Filename.c_str() : ""));

	ImGui::InputText("Fallout:Tactics .zar file path", &Filename);
	if (ImGui::Button("Load File"))
	{
		std::filesystem::path filepath = Filename;
		ReadFTZar(filepath, File);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		ExportFTZar(File);
	}

	if (File != nullptr)
	{
		RenderFTZar(File, Width, Height, &Tex, Renderer);
		ImGui::Image((void*)Tex, ImVec2(Width, Height));
	}

	ImGui::End();
}

void FTZarWindow::InitWin()
{}

void FTZarWindow::ProcessMenuBtn()
{
	if (ImGui::BeginMenu("[Fallout:Tactics]"))
	{
		if (ImGui::MenuItem("Fallout:Tactics .zar")) { this->SetVisible(!this->GetVisible()); }
		ImGui::EndMenu();
	}
}