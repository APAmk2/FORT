#include "fotxtb_win.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <stdio.h>
#include <filesystem>
#include <fstream>

using namespace std;

bool readFotxtb(std::filesystem::path& filename, Fotxtb_t*& file)
{
	delete file;
	file = nullptr;
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::LittleEndian)) return false;
	file = new Fotxtb_t(reader);
	file->filename = filename.stem().string();
	reader->Close();
	delete reader;

	return true;
}

void exportFotxtb(Fotxtb_t*& file)
{
	std::string str = "";
	for (const auto& [key, val] : file->entries)
	{
		str += "{" + to_string(key) + "}{}{" + val + "}\n";
	}

	ofstream out;
	out.open(file->filename + ".txt");
	if (out.is_open())
	{
		out << str << endl;
	}
	out.close();
}

void FotxtbWindow::drawWindow()
{
	if (!getVisible()) return;
	ImGui::Begin("Fonline TXTb Reading Tool");

	ImGui::InputText("Fonline .fotxtb file path", &Fotxtbfilename);
	if (ImGui::Button("Load File"))
	{
		filesystem::path filepath = Fotxtbfilename;
		readFotxtb(filepath, FotxtbFile);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		exportFotxtb(FotxtbFile);
	}

	if (FotxtbFile != nullptr)
	{
		ImGui::Text("Entries:%i", FotxtbFile->entriesCount);
		ImGui::NewLine();
		std::string strToDisplay = "";

		for (const auto& [key, val] : FotxtbFile->entries)
		{
			strToDisplay = "{" + to_string(key) + "}{}{" + val + "}";
			ImGui::TextUnformatted(strToDisplay.c_str());
		}
	}

	ImGui::End();
}

void FotxtbWindow::initWindow()
{}