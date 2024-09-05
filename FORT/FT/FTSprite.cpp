#include "FTSprite.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "lodepng.h"
#include "zlib.h"
#include <filesystem>
#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

ZarImage_t::ZarImage_t(ByteReader* reader)
{
	std::string signature = reader->string(6);
	if(signature != "<zar>")
	{
		cout << "ZarImageSign wrong!\n" << signature << "\n";
		return;
	}
	uint8_t zar_type = reader->u8();
	if (zar_type != 0x33 && zar_type != 0x34)
	{
		cout << "Wrong Zar Type!\n" << zar_type << "\n";
		return;
	}
	reader->u8();
	width = reader->u32();
	height = reader->u32();
	bool palPresent = reader->u8() == 1;
	if (palPresent)
	{
		cout << "Zar mustN include PAL!\n";
		return;
	}
	uint32_t RLE_size = reader->u32();

}

AnimImgHdr_t::AnimImgHdr_t(ByteReader* reader)
{
	std::string signature = reader->string(14);
	if (signature != "<spranim_img>")
	{
		cout << "AnimImgHdrSign wrong!\n" << signature << "\n";
		return;
	}

	isPacked = reader->u16() == '2';
	if (isPacked)
	{
		unpacked_size = reader->u32();
	}
	pos = reader->CurrPos();
}

AnimHdr_t::AnimHdr_t(ByteReader* reader)
{
	std::string signature = reader->string(12);
	if (signature != "<spranim>")
	{
		cout << "AnimHdrSign wrong!\n" << signature << "\n"; 
		return;
	}

	uint32_t image_data_offset = reader->u32();
	uint32_t stringLen = reader->u32();
	anim_name = reader->string(stringLen);
	frame_count = reader->u32();
	dirs = reader->u32();

	int len = frame_count * dirs;
	bboxes.resize(len);
	reader->Read(&bboxes[0], len * sizeof(rect));
	
	int oldPos = reader->CurrPos();
	reader->Pos(image_data_offset);
	animImgHdr = new AnimImgHdr_t(reader);
	reader->Pos(oldPos);
}

SequenceHdr_t::SequenceHdr_t(ByteReader* reader)
{
	items_count = reader->u32();
	for (size_t i = 0; i < items_count; i++)
	{
		frame_num.push_back(reader->i16());
	}
	for (size_t i = 0; i < items_count; i++)
	{
		reader->u32();
	}

	uint32_t stringLen = reader->u32();
	seq_name = reader->string(stringLen);
	anim_hdr_indx = reader->u16();
}

FTSprite_t::FTSprite_t(ByteReader* reader)
{
	std::string signature = reader->string(11);
	if (signature != "<sprite>")
	{
		printf("FTSprite: Signature is INVALID, aborting...\n");
		return;
	}
	else
	{
		printf("FTSprite: Signature is VALID, continue...\n");
	}
	for(size_t i = 0; i < 3; i++) { reader->u8(); }
	center_x = reader->u32();
	center_y = reader->u32();
	for (size_t i = 0; i < 3; i++) { reader->u8(); }
	seq_count = reader->u32();

	for (size_t i = 0; i < seq_count; i++)
	{
		SequenceHdr_t newHdr(reader);
		seq_hdrs.push_back(newHdr);
	}

	anim_count = reader->u32();
	for (size_t i = 0; i < anim_count; i++)
	{
		AnimHdr_t newHdr(reader);
		anim_hdrs.push_back(newHdr);
	}

	for (size_t i = 0; i < anim_count; i++)
	{
		AnimHdr_t* currHdr = &anim_hdrs[i];
		AnimHdr_t* nextHdr = ((i != (anim_count - 1)) ? &anim_hdrs[i + 1] : nullptr);
		string packed_data;
		string unpacked_data;
		reader->Pos(currHdr->animImgHdr->pos);
		cout << "Pos for copy: " << currHdr->animImgHdr->pos << endl;
		size_t size = ((nextHdr != nullptr) ? nextHdr->animImgHdr->pos - currHdr->animImgHdr->pos : reader->Bytes() - currHdr->animImgHdr->pos);
		packed_data.resize(size);
		reader->Read(&packed_data[0], size);
		unpacked_data.resize(currHdr->animImgHdr->unpacked_size);
		if (currHdr->animImgHdr->isPacked)
		{
			uncompress((Bytef*)&unpacked_data[0], (uLong*)&currHdr->animImgHdr->unpacked_size, (Bytef*)&packed_data[0], size);
		}
		else
		{
			unpacked_data = packed_data;
		}
		stringstream stream(unpacked_data);

		vector<ucolor> pal;
		for (size_t j = 0; j < 3; j++)
		{
			uint32_t palSize;
			cout << "Pos for size: " << stream.tellg() << endl;
			stream.read((char*)&palSize, sizeof(uint32_t));
			if (palSize > 256) { cout << "Too many colors in PAL\n" << palSize << endl; return; }

			for (size_t color = 0; color < palSize; color++)
			{
				ucolor newColor;
				stream.read((char*)&newColor.r, sizeof(uint8_t));
				stream.read((char*)&newColor.g, sizeof(uint8_t));
				stream.read((char*)&newColor.b, sizeof(uint8_t));
				stream.read((char*)&newColor.a, sizeof(uint8_t));
				pal.push_back(newColor);
			}
		}

		while (stream.good())
		{
			char tag;
			stream.read(&tag, sizeof(char));

			if (tag == '<')
			{
				break;
			}
			else if (tag == '\0')
			{

			}
			else if (tag == '\1')
			{
				int32_t x;
				stream.read((char*)&x, sizeof(int32_t));
				int32_t y;
				stream.read((char*)&y, sizeof(int32_t));
			}
		}
	}

}

bool readFTSprite(std::filesystem::path& filename, FTSprite_t*& file)
{
	delete file;
	file = nullptr;
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::BigEndian)) return false;
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
	if (!getVisible()) return;

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
	ImGui::Image((void*)(intptr_t)FTSTex, ImVec2(640, 480));

	if (FTSFile != nullptr)
	{
		// Left
		static int selected = 0;
		{
			ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
			for (int i = 0; i < FTSFile->seq_count; i++)
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
	}
	ImGui::End();
}
void FTSpriteWindow::initWindow()
{}