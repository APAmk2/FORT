#include "Fo2D_win.h"
#include "lodepng.h"
#include "../FORT.h"
#include <filesystem>

int Fo2DFrameCounter = 0;

bool ReadFo2D(std::filesystem::path& filename, Fo2D_t*& file)
{
	delete file;
	file = nullptr;
	Fo2DFrameCounter = 0;
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::LittleEndian)) return false;
	file = new Fo2D_t(reader);
    file->Filename = filename.stem().string();
	reader->Close();
	delete reader;

	return true;
}

void ExportFo2D(Fo2D_t*& file)
{
	for (size_t currData = 0; currData < file->DirCount; currData++)
	{
		for (size_t currFrame = 0; currFrame < file->FrameCount; currFrame++)
		{
			Fo2DFrame_t* currFramePtr = &file->Data[currData].Frames[currFrame];
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

			unsigned error = lodepng::encode((file->Filename + "_" + std::to_string(currData) + "_" + std::to_string(currFrame)) + ".png", image, width, height);
			if (error) ImGui::DebugLog("encoder error %i:%s\n", error, lodepng_error_text(error));
		}
	}
}

bool RenderFo2D(Fo2D_t* file, uint16_t& width, uint16_t& height, int16_t& dir, SDL_Texture** Fo2DTex, SDL_Renderer* renderer)
{
	Fo2DFrame_t* currFramePtr = &file->Data[dir].Frames[Fo2DFrameCounter];
	
	SDL_DestroyTexture(*Fo2DTex);
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)&currFramePtr->Pixels[0], currFramePtr->Width, currFramePtr->Height, 4 * 8, 4 * currFramePtr->Width,
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
	
	width = currFramePtr->Width;
	height = currFramePtr->Height;
	
	Fo2DFrameCounter++;
	if (Fo2DFrameCounter >= file->Data[dir].Frames.size())
	{
		Fo2DFrameCounter = 0;
	}

	return true;
}

void Fo2DWindow::DrawWin()
{
	if (!GetVisible()) return;
	ImGui::Begin("Fonline 2D Graphics Tool", &Visible);

	ImGui::Text("Width:%i", Width);
	ImGui::SameLine();
	ImGui::Text("Height:%i", Height);
	ImGui::SameLine();
	ImGui::Text("FPS:%i", (File != nullptr ? (File->AnimTicks / File->FrameCount) / 10 : 0));
	ImGui::SameLine();
	ImGui::Text("Frames:%i/%i", Fo2DFrameCounter, (File != nullptr ? File->FrameCount - 1: 0));
	if (ImGui::Button("<") && File != nullptr)
	{
		Dir--;
		if (Dir < 0) Dir = File->DirCount - 1;
	}
	ImGui::SameLine();
	ImGui::Text("Dir:%i/%i", Dir, (File != nullptr ? File->DirCount - 1 : 0));
	ImGui::SameLine();
	if (ImGui::Button(">") && File != nullptr)
	{
		Dir++;
		if (Dir >= File->DirCount) Dir = 0;
	}

	ImGui::Text("Filename:%s", (File != nullptr ? File->Filename.c_str() : ""));

	ImGui::InputText("FOnline 2D Graphics file path", &Filename);
	if (ImGui::Button("Load File"))
	{
		std::filesystem::path filepath = Filename;
		ReadFo2D(filepath, File);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		ExportFo2D(File);
	}

	if (File != nullptr && FPSTimer <= SDL_GetTicks())
	{
		RenderFo2D(File, Width, Height, Dir, &Tex, Renderer);
		if(File->AnimTicks > 0) FPSTimer = SDL_GetTicks() + (File->AnimTicks / File->FrameCount);
		else FPSTimer = SDL_GetTicks() + (1000 / 10);
	}
	ImGui::Image((void*)Tex, ImVec2(Width, Height));

	ImGui::End();
}

void Fo2DWindow::InitWin()
{
	ImGui::DebugLog("Initializing Fonline2D Tool...\n");
	ImGui::DebugLog("Fonline2D Tool Init Done.\n");
}

void Fo2DWindow::DestroyWin() { }

void Fo2DWindow::ProcessMenuBtn()
{
	if (ImGui::BeginMenu("[New Fonline]"))
	{
		if (ImGui::MenuItem("Fonline2D")) { this->SetVisible(!this->GetVisible()); }
		ImGui::EndMenu();
	}
}