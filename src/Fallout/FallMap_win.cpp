#include "FallMap_win.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <filesystem>
#include <fstream>
#include <format>

const std::string FrmType[8] = { "items", "critters", "scenery", "walls", "tiles", "backgrnd", "intrface", "inven" };
const std::string ProtoTypeNames[6] = { "Items", "Critters", "Scenery", "Walls", "Tiles", "Misc" };

std::vector<std::string> tiles_names;

bool ReadFallMap(std::filesystem::path& filename, FallMap_t*& file)
{
	delete file;
	file = nullptr;
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::BigEndian)) return false;
	file = new FallMap_t(reader);
	file->Filename = filename.stem().string();
	reader->Close();
	delete reader;

	return true;
}

void ExportFallMap(FallMap_t*& file)
{
	std::ofstream out;
	out.open(file->Filename + ".fomap");
	if (out.is_open())
	{
		out << std::format("[{}]\n", "Header");
		out << std::format("{:20} {}\n", "Version", 4);
		out << std::format("{:20} {}\n", "MaxHexX", 200);

		int mapHexY = 200;
		switch (file->MapFlags & 0xE)
		{
		case 0x0c:
			mapHexY = 200;
			break;
		case 0x08:
			mapHexY = 400;
			break;
		case 0x00:
			mapHexY = 600;
			break;
		}

		out << std::format("{:20} {}\n", "MaxHexY", mapHexY);
		out << std::format("{:20} {}\n", "WorkHexX", 100);
		out << std::format("{:20} {}\n", "WorkHexY", 100);
		out << std::format("{:20} {}\n", "ScriptModule", "-");
		out << std::format("{:20} {}\n", "ScriptFunc", "-");
		out << std::format("{:20} {}\n", "NoLogOut", 0);
		out << std::format("{:20} {}\n", "Time", -1);
		out << std::format("{:20} {:<4} {:<4} {:<4} {:<4}\n", "DayTime", 5, 5, 5, 5);
		out << std::format("{:20} {:<3} {:<3} {:<3}\n", "DayColor0", 18, 18, 53);
		out << std::format("{:20} {:<3} {:<3} {:<3}\n", "DayColor1", 18, 18, 53);
		out << std::format("{:20} {:<3} {:<3} {:<3}\n", "DayColor2", 18, 18, 53);
		out << std::format("{:20} {:<3} {:<3} {:<3}\n", "DayColor3", 18, 18, 53);
		
		out << "\n";
		out << std::format("[{}]\n", "Tiles");

		int tilesLen = 10000;

		switch (file->MapFlags & 0xE)
		{
		case 0x0c:
			tilesLen = 10000;
			break;
		case 0x08:
			tilesLen = 20000;
			break;
		case 0x00:
			tilesLen = 30000;
			break;
		}

		for (int i = 0; i < tilesLen; i++)
		{
			FallMapTile_t currentTile = file->Tiles[i];
			int X = (i % 100) * 2;
			int Y = (i / 100) * 2;

			if (currentTile.TileId > 1) out << std::format("{:10} {:<4} {:<4}           {}\n", "tile", X, Y, ("art\\tiles\\" + tiles_names[currentTile.TileId]));
			if (currentTile.RoofId > 1) out << std::format("{:10} {:<4} {:<4}           {}\n", "roof", X, Y, ("art\\tiles\\" + tiles_names[currentTile.RoofId]));
		}

		out << "\n";
		out << std::format("[{}]\n", "Objects");
		out << std::format("{:20} {}\n", "MapObjType", 2);
		out << std::format("{:20} {}\n", "ProtoId", 3853);
		out << std::format("{:20} {}\n", "MapX", (file->DefChosPos % 200));
		out << std::format("{:20} {}\n", "MapY", ((file->DefChosPos / 200) + (200 * file->DefChosElv)));
		out << std::format("{:20} {}\n", "Scenery_ToDir", file->DefChosDir);

		for (size_t i = 0, len = file->TotalObjects; i < len; i++)
		{
			FallMapObject_t currObject = file->Objects[i];
			
			if (currObject.ObjectPos == -1) continue;

			if (currObject.PIDType == 0 ) // Items
			{
				out << std::format("{:20} {}\n", "MapObjType", 1);
				out << std::format("{:20} {}\n", "ProtoId", currObject.PIDNum);

				int objX = (currObject.ObjectPos % 200);
				int objY = (currObject.ObjectPos / 200) + (200 * currObject.MapElev);
				out << std::format("{:20} {}\n", "MapX", objX);
				out << std::format("{:20} {}\n", "MapY", objY);
				if (currObject.WeapData != nullptr)
				{
					if (currObject.WeapData->AmmoPid > 0)
					{
						out << std::format("{:20} {}\n", "Item_AmmoPid", currObject.WeapData->AmmoPid);
					}
					if (currObject.WeapData->AmmoCount > 0)
					{
						out << std::format("{:20} {}\n", "Item_AmmoCount", currObject.WeapData->AmmoCount);
					}
				}
				if (currObject.KeyData != nullptr)
				{
					if (currObject.KeyData->KeyCode > 0)
					{
						out << std::format("{:20} {}\n", "Item_LockerDoorId", currObject.KeyData->KeyCode);
					}
				}

				if (currObject.DoorData != nullptr)
				{
					if (currObject.DoorData->WalkThru > 0)
					{
						out << std::format("{:20} {}\n", "Item_LockerCondition", currObject.DoorData->WalkThru);
					}
				}
			}

			if (currObject.PIDType == 1) // Critters
			{
				out << std::format("{:20} {}\n", "MapObjType", 0);
				out << std::format("{:20} {}\n", "ProtoId", currObject.PIDNum);

				int objX = (currObject.ObjectPos % 200);
				int objY = (currObject.ObjectPos / 200) + (200 * currObject.MapElev);
				out << std::format("{:20} {}\n", "MapX", objX);
				out << std::format("{:20} {}\n", "MapY", objY);
				out << std::format("{:20} {}\n", "Dir", currObject.Dir);
				out << std::format("{:20} {}\n", "Critter_Cond", 1);
				out << std::format("{:20} {}\n", "Critter_ParamIndex0", "ST_DIALOG_ID");
				out << std::format("{:20} {}\n", "Critter_ParamValue0", 0);
				out << std::format("{:20} {}\n", "Critter_ParamIndex1", "ST_AI_ID");
				out << std::format("{:20} {}\n", "Critter_ParamValue1", 1);
				out << std::format("{:20} {}\n", "Critter_ParamIndex2", "ST_BAG_ID");
				out << std::format("{:20} {}\n", "Critter_ParamValue2", 0);
				out << std::format("{:20} {}\n", "Critter_ParamIndex3", "ST_TEAM_ID");
				out << std::format("{:20} {}\n", "Critter_ParamValue3", currObject.CritData->GroupId);
				out << std::format("{:20} {}\n", "Critter_ParamIndex4", "ST_NPC_ROLE");
				out << std::format("{:20} {}\n", "Critter_ParamValue4", 0);
				out << std::format("{:20} {}\n", "Critter_ParamIndex5", "ST_REPLICATION_TIME");
				out << std::format("{:20} {}\n", "Critter_ParamValue5", 0);
			}

			if (currObject.PIDType == 2) // Scenery
			{
				out << std::format("{:20} {}\n", "MapObjType", 2);
				out << std::format("{:20} {}\n", "ProtoId", 2000 + currObject.PIDNum);

				int objX = (currObject.ObjectPos % 200);
				int objY = (currObject.ObjectPos / 200) + (200 * currObject.MapElev);
				out << std::format("{:20} {}\n", "MapX", objX);
				out << std::format("{:20} {}\n", "MapY", objY);
			}

			if (currObject.PIDType == 3) // walls
			{
				out << std::format("{:20} {}\n", "MapObjType", 2);
				out << std::format("{:20} {}\n", "ProtoId", 5000 + currObject.PIDNum);

				int objX = (currObject.ObjectPos % 200);
				int objY = (currObject.ObjectPos / 200) + (200 * currObject.MapElev);
				out << std::format("{:20} {}\n", "MapX", objX);
				out << std::format("{:20} {}\n", "MapY", objY);
			}

			if (currObject.PIDType == 5 && currObject.PIDNum != 12) // misc
			{
				out << std::format("{:20} {}\n", "MapObjType", 2);
				out << std::format("{:20} {}\n", "ProtoId", 4000 + currObject.PIDNum);

				int objX = (currObject.ObjectPos % 200);
				int objY = (currObject.ObjectPos / 200) + (200 * currObject.MapElev);
				out << std::format("{:20} {}\n", "MapX", objX);
				out << std::format("{:20} {}\n", "MapY", objY);
			}

			if (currObject.PIDType != 4) //Shared data
			{
				if (currObject.LightRadius > 0)
				{
					out << std::format("{:20} {}\n", "LightDistance", currObject.LightRadius);
				}
				if (currObject.LightIntense > 0)
				{
					out << std::format("{:20} {}\n", "LightIntensity", currObject.LightIntense);
				}

				out << "\n";
			}
		}
	}
	
	out.close();
}

void FallMapWindow::DrawWin()
{
	if (!GetVisible()) return;
	ImGui::Begin("Fallout .map Reading Tool");

	ImGui::InputText("Fallout .map file path", &Filename);
	ImGui::InputText("Fallout Game Path", &GamePath);
	if (ImGui::Button("Load File"))
	{
		std::filesystem::path filepath = Filename;
		ReadFallMap(filepath, File);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		ExportFallMap(File);
	}

	if (File != nullptr)
	{
		ImGui::BeginChild("Map", ImVec2(100, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY);
		ImGui::Text("Ver: %i", File->Version);
		ImGui::Text("Name: %s", File->MapName.c_str());
		ImGui::Text("Default Chosen Pos: %i", File->DefChosPos);
		ImGui::Text("Default Chosen Elevation: %i", File->DefChosElv);
		ImGui::Text("Default Chosen Direction: %i", File->DefChosDir);
		ImGui::Text("LVars Count: %i", File->LVarsNum);
		ImGui::Text("Script ID: %i", File->ScriptId);
		ImGui::Text("Map Flags: %i", File->MapFlags);
		ImGui::Text("Map Darkness: %i", File->MapDark);
		ImGui::Text("GVars Count: %i", File->GVarsNum);
		ImGui::Text("Map ID: %i", File->MapId);
		ImGui::Text("Time since Epoch (Ticks): %i", File->EpochTime);
		ImGui::Text("Tiles count: %i", File->Tiles.size());
		ImGui::EndChild();

		ImGui::BeginChild("TESTOBJECT", ImVec2(100, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY);
		int objX = (File->Objects[0].ObjectPos % 200);
		int objY = (File->Objects[0].ObjectPos / 200) + (200 * File->Objects[0].MapElev);
		ImGui::Text("Object Position: X%i Y%i", objX, objY);
		ImGui::Text("Object Frame Num: %i", File->Objects[0].FrameNum);
		ImGui::Text("Object Direction: %i", File->Objects[0].Dir);
		ImGui::Text("Object FID Type: %s", FrmType[File->Objects[0].FIDType].c_str());
		ImGui::Text("Object FID .LST Num: %i", File->Objects[0].FIDNum);
		ImGui::Text("Object Flags: %i", File->Objects[0].Flags);
		ImGui::Text("Object Map Elevation: %i", File->Objects[0].MapElev);
		ImGui::Text("Object PID Type: %s", ProtoTypeNames[File->Objects[0].PIDType].c_str());
		ImGui::Text("Object PID .LST Num: %i", File->Objects[0].PIDNum);
		//ImGui::Text("Object Crit Index: %i", File->Objects[0].CritIndex);
		ImGui::Text("Object Light Radius: %i", File->Objects[0].LightRadius);
		ImGui::Text("Object Light Intense: %i", File->Objects[0].LightIntense);
		//ImGui::Text("Object Outline Color: %i", File->Objects[0].OutlineColor);
		ImGui::Text("Object Script ID: %i", File->Objects[0].ScriptID);
		ImGui::Text("Object Inventory Size: %i", File->Objects[0].InvenSize);
		ImGui::Text("Object Crit Inventory Slots: %i", File->Objects[0].CritInvenSlots);
		ImGui::EndChild();
	}

	ImGui::End();
}

void FallMapWindow::InitWin()
{
	std::string currString;
	std::ifstream input(GamePath + "art/tiles/TILES.LST");
	if (input.is_open())
	{
		while (getline(input, currString))
		{
			tiles_names.push_back(currString);
		}
	}
	input.close();
}