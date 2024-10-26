#include "FTSprite_win.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "lodepng.h"
#include <filesystem>
#include <stdio.h>

using namespace std;

bool readFTSprite(std::filesystem::path& filename, FTSprite_t*& file)
{
	delete file;
	file = nullptr;
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::LittleEndian)) return false;
	file = new FTSprite_t(reader);
	file->filename = filename.stem().string();
	reader->Close();
	delete reader;

	return true;
}

void exportFTSprite(FTSprite_t*& file)
{

}

bool renderFTSprite(FTSprite_t* file, int& width, int& height, int& dir, SDL_Texture& FTSTex)
{
	return true;
}

void FTSpriteWindow::drawWindow()
{
	if (!GetVisible()) return;

	ImGui::Begin("Fallout:Tactics Sprite Tool");
	ImGui::Text("Width: %i", FTSwidth);
	ImGui::SameLine();
	ImGui::Text("Height: %i", FTSheight);
	ImGui::SameLine();
	ImGui::Text("Dir: %i", FTSDir);
	ImGui::InputText("Fallout:Tactics .spr file path", &FTSfilename);
	if (ImGui::Button("Load File"))
	{
		filesystem::path filepath = FTSfilename;
		readFTSprite(filepath, FTSFile);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		exportFTSprite(FTSFile);
	}

	if (FTSFile != nullptr)
	{
		// Left
		static int selected = 0;
		{
			ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
			for (uint32_t i = 0; i < FTSFile->seq_count; i++)
			{
				if (ImGui::Selectable(FTSFile->seq_hdrs[i].seq_name.c_str(), selected == i))
					selected = i;
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();

		// Right
		{
			ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
			ImGui::Text("Sequence: %d", selected);
			ImGui::Separator();
			if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Data"))
				{
					ImGui::Text("Items Count:%i", FTSFile->seq_hdrs[selected].items_count);
					for (size_t i = 0, len = FTSFile->seq_hdrs[selected].frame_num.size(); i < len; i++)
					{
						ImGui::Text("Frame Num%i:%i", i, FTSFile->seq_hdrs[selected].frame_num[i]);
					}
					ImGui::Text("Anim HDR Index:%i", FTSFile->seq_hdrs[selected].anim_hdr_indx);
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndChild();
			ImGui::EndGroup();
		}
		ImGui::Image((void*)(intptr_t)FTSTex, ImVec2(640, 480));
	}
	
	ImGui::End();
}
void FTSpriteWindow::initWindow()
{}