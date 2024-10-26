#include "FoTxtb_win.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <filesystem>
#include <fstream>

bool ReadFoTxtb(std::filesystem::path& filename, FoTxtb_t*& file)
{
	delete file;
	file = nullptr;
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::LittleEndian)) return false;
	file = new FoTxtb_t(reader);
	file->Filename = filename.stem().string();
	reader->Close();
	delete reader;

	return true;
}

void ExportFoTxtb(FoTxtb_t*& file)
{
	std::string str = "";
	for (const auto& [key, val] : file->Entries)
	{
		str += "{" + std::to_string(key) + "}{}{" + val + "}\n";
	}

	std::ofstream out;
	out.open(file->Filename + ".txt");
	if (out.is_open())
	{
		out << str << std::endl;
	}
	out.close();
}

void FoTxtbWindow::DrawWin()
{
	if (!GetVisible()) return;
	ImGui::Begin("Fonline TXTb Reading Tool");

	ImGui::InputText("Fonline .fotxtb file path", &Filename);
	if (ImGui::Button("Load File"))
	{
		std::filesystem::path filepath = Filename;
		ReadFoTxtb(filepath, File);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		ExportFoTxtb(File);
	}

	if (File != nullptr)
	{
		ImGui::Text("Entries:%i", File->EntriesCount);
		ImGui::NewLine();
		std::string strToDisplay = "";

		for (const auto& [key, val] : File->Entries)
		{
			strToDisplay = "{" + std::to_string(key) + "}{}{" + val + "}";
			ImGui::TextUnformatted(strToDisplay.c_str());
		}
	}

	ImGui::End();
}

void FoTxtbWindow::InitWin()
{}