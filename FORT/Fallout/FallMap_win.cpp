#include "FallMap_win.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <filesystem>
#include <fstream>
#include <format>

using namespace std;

const string frmType[8] = { "items", "critters", "scenery", "walls", "tiles", "backgrnd", "intrface", "inven" };
const string protoTypeNames[6] = { "Items", "Critters", "Scenery", "Walls", "Tiles", "Misc" };

std::vector<std::string> tiles_names;

bool readFallMap(std::filesystem::path& filename, FallMap_t*& file)
{
	delete file;
	file = nullptr;
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::LittleEndian)) return false;
	file = new FallMap_t(reader);
	file->filename = filename.stem().string();
	reader->Close();
	delete reader;

	return true;
}

void exportFallMap(FallMap_t*& file)
{
	ofstream out;
	out.open(file->filename + ".fomap");
	if (out.is_open())
	{
		out << std::format("[{}]\n", "Header");
		out << std::format("{:20} {}\n", "Version", 4);
		out << std::format("{:20} {}\n", "MaxHexX", 200);

		int mapHexY = 200;
		switch (file->mapFlags & 0xE)
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

		switch (file->mapFlags & 0xE)
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
			tile_t currentTile = file->tiles[i];
			int X = (i % 100) * 2;
			int Y = (i / 100) * 2;

			if (currentTile.floorId > 1) out << std::format("{:10} {:<4} {:<4}           {}\n", "tile", X, Y, ("art\\tiles\\" + tiles_names[currentTile.floorId]));
			if (currentTile.tileId > 1) out << std::format("{:10} {:<4} {:<4}           {}\n", "roof", X, Y, ("art\\tiles\\" + tiles_names[currentTile.tileId]));
		}

		out << "\n";
		out << std::format("[{}]\n", "Objects");
		out << std::format("{:20} {}\n", "MapObjType", 2);
		out << std::format("{:20} {}\n", "ProtoId", 3853);
		out << std::format("{:20} {}\n", "MapX", (file->defChosPos % 200));
		out << std::format("{:20} {}\n", "MapY", ((file->defChosPos / 200) + (200 * file->defChosElv)));
		out << std::format("{:20} {}\n", "Scenery_ToDir", file->defChosDir);

		for (size_t i = 0, len = file->totalObjects; i < len; i++)
		{
			mapObject_t currObject = file->objects[i];
			
			if (currObject.objectPos == -1) continue;

			if (currObject.PIDType == 0 ) // Items
			{
				out << std::format("{:20} {}\n", "MapObjType", 1);
				out << std::format("{:20} {}\n", "ProtoId", currObject.PIDNum);

				int objX = (currObject.objectPos % 200);
				int objY = (currObject.objectPos / 200) + (200 * currObject.mapElev);
				out << std::format("{:20} {}\n", "MapX", objX);
				out << std::format("{:20} {}\n", "MapY", objY);
				if (currObject.weapData != nullptr)
				{
					if (currObject.weapData->ammoPid > 0)
					{
						out << std::format("{:20} {}\n", "Item_AmmoPid", currObject.weapData->ammoPid);
					}
					if (currObject.weapData->ammoCount > 0)
					{
						out << std::format("{:20} {}\n", "Item_AmmoCount", currObject.weapData->ammoCount);
					}
				}
				if (currObject.keyData != nullptr)
				{
					if (currObject.keyData->keyCode > 0)
					{
						out << std::format("{:20} {}\n", "Item_LockerDoorId", currObject.keyData->keyCode);
					}
				}

				if (currObject.doorData != nullptr)
				{
					if (currObject.doorData->walkThru > 0)
					{
						out << std::format("{:20} {}\n", "Item_LockerCondition", currObject.doorData->walkThru);
					}
				}
			}

			if (currObject.PIDType == 1) // Critters
			{
				out << std::format("{:20} {}\n", "MapObjType", 0);
				out << std::format("{:20} {}\n", "ProtoId", currObject.PIDNum);

				int objX = (currObject.objectPos % 200);
				int objY = (currObject.objectPos / 200) + (200 * currObject.mapElev);
				out << std::format("{:20} {}\n", "MapX", objX);
				out << std::format("{:20} {}\n", "MapY", objY);
				out << std::format("{:20} {}\n", "Dir", currObject.dir);
				out << std::format("{:20} {}\n", "Critter_Cond", 1);
				out << std::format("{:20} {}\n", "Critter_ParamIndex0", "ST_DIALOG_ID");
				out << std::format("{:20} {}\n", "Critter_ParamValue0", 0);
				out << std::format("{:20} {}\n", "Critter_ParamIndex1", "ST_AI_ID");
				out << std::format("{:20} {}\n", "Critter_ParamValue1", 1);
				out << std::format("{:20} {}\n", "Critter_ParamIndex2", "ST_BAG_ID");
				out << std::format("{:20} {}\n", "Critter_ParamValue2", 0);
				out << std::format("{:20} {}\n", "Critter_ParamIndex3", "ST_TEAM_ID");
				out << std::format("{:20} {}\n", "Critter_ParamValue3", currObject.critData->groupId);
				out << std::format("{:20} {}\n", "Critter_ParamIndex4", "ST_NPC_ROLE");
				out << std::format("{:20} {}\n", "Critter_ParamValue4", 0);
				out << std::format("{:20} {}\n", "Critter_ParamIndex5", "ST_REPLICATION_TIME");
				out << std::format("{:20} {}\n", "Critter_ParamValue5", 0);
			}

			if (currObject.PIDType == 2) // Scenery
			{
				out << std::format("{:20} {}\n", "MapObjType", 2);
				out << std::format("{:20} {}\n", "ProtoId", 2000 + currObject.PIDNum);

				int objX = (currObject.objectPos % 200);
				int objY = (currObject.objectPos / 200) + (200 * currObject.mapElev);
				out << std::format("{:20} {}\n", "MapX", objX);
				out << std::format("{:20} {}\n", "MapY", objY);
			}

			if (currObject.PIDType == 3) // walls
			{
				out << std::format("{:20} {}\n", "MapObjType", 2);
				out << std::format("{:20} {}\n", "ProtoId", 5000 + currObject.PIDNum);

				int objX = (currObject.objectPos % 200);
				int objY = (currObject.objectPos / 200) + (200 * currObject.mapElev);
				out << std::format("{:20} {}\n", "MapX", objX);
				out << std::format("{:20} {}\n", "MapY", objY);
			}

			if (currObject.PIDType == 5 && currObject.PIDNum != 12) // misc
			{
				out << std::format("{:20} {}\n", "MapObjType", 2);
				out << std::format("{:20} {}\n", "ProtoId", 4000 + currObject.PIDNum);

				int objX = (currObject.objectPos % 200);
				int objY = (currObject.objectPos / 200) + (200 * currObject.mapElev);
				out << std::format("{:20} {}\n", "MapX", objX);
				out << std::format("{:20} {}\n", "MapY", objY);
			}

			if (currObject.PIDType != 4) //Shared data
			{
				if (currObject.lightRadius > 0)
				{
					out << std::format("{:20} {}\n", "LightDistance", currObject.lightRadius);
				}
				if (currObject.lightIntense > 0)
				{
					out << std::format("{:20} {}\n", "LightIntensity", currObject.lightIntense);
				}

				out << "\n";
			}
		}
	}
	
	out.close();
}

void FallMapWindow::drawWindow()
{
	if (!getVisible()) return;
	ImGui::Begin("Fallout .map Reading Tool");

	ImGui::InputText("Fallout .map file path", &fallMapfilename);
	if (ImGui::Button("Load File"))
	{
		filesystem::path filepath = fallMapfilename;
		readFallMap(filepath, fallMapFile);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		exportFallMap(fallMapFile);
	}

	if (fallMapFile != nullptr)
	{
		ImGui::BeginChild("Map", ImVec2(100, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY);
		ImGui::Text("Ver: %i", fallMapFile->version);
		ImGui::Text("Name: %s", fallMapFile->mapName.c_str());
		ImGui::Text("Default Chosen Pos: %i", fallMapFile->defChosPos);
		ImGui::Text("Default Chosen Elevation: %i", fallMapFile->defChosElv);
		ImGui::Text("Default Chosen Direction: %i", fallMapFile->defChosDir);
		ImGui::Text("LVars Count: %i", fallMapFile->lVarsNum);
		ImGui::Text("Script ID: %i", fallMapFile->scriptId);
		ImGui::Text("Map Flags: %i", fallMapFile->mapFlags);
		ImGui::Text("Map Darkness: %i", fallMapFile->mapDark);
		ImGui::Text("GVars Count: %i", fallMapFile->gVarsNum);
		ImGui::Text("Map ID: %i", fallMapFile->mapId);
		ImGui::Text("Time since Epoch (Ticks): %i", fallMapFile->epochTime);
		ImGui::Text("Tiles count: %i", fallMapFile->tiles.size());
		ImGui::EndChild();

		ImGui::BeginChild("TESTOBJECT", ImVec2(100, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY);
		int objX = (fallMapFile->objects[0].objectPos % 200);
		int objY = (fallMapFile->objects[0].objectPos / 200) + (200 * fallMapFile->objects[0].mapElev);
		ImGui::Text("Object Position: X%i Y%i", objX, objY);
		ImGui::Text("Object Frame Num: %i", fallMapFile->objects[0].frameNum);
		ImGui::Text("Object Direction: %i", fallMapFile->objects[0].dir);
		ImGui::Text("Object FID Type: %s", frmType[fallMapFile->objects[0].FIDType].c_str());
		ImGui::Text("Object FID .LST Num: %i", fallMapFile->objects[0].FIDNum);
		ImGui::Text("Object Flags: %i", fallMapFile->objects[0].flags);
		ImGui::Text("Object Map Elevation: %i", fallMapFile->objects[0].mapElev);
		ImGui::Text("Object PID Type: %s", protoTypeNames[fallMapFile->objects[0].PIDType].c_str());
		ImGui::Text("Object PID .LST Num: %i", fallMapFile->objects[0].PIDNum);
		//ImGui::Text("Object Crit Index: %i", fallMapFile->objects[0].critIndex);
		ImGui::Text("Object Light Radius: %i", fallMapFile->objects[0].lightRadius);
		ImGui::Text("Object Light Intense: %i", fallMapFile->objects[0].lightIntense);
		//ImGui::Text("Object Outline Color: %i", fallMapFile->objects[0].outlineColor);
		ImGui::Text("Object Script ID: %i", fallMapFile->objects[0].scriptID);
		ImGui::Text("Object Inventory Size: %i", fallMapFile->objects[0].invenSize);
		ImGui::Text("Object Crit Inventory Slots: %i", fallMapFile->objects[0].critInvenSlots);
		ImGui::EndChild();
	}

	ImGui::End();
}

void FallMapWindow::initWindow()
{
	string currString;
	ifstream input("TILES.LST");
	if (input.is_open())
	{
		while (getline(input, currString))
		{
			tiles_names.push_back(currString);
		}
	}
	input.close();
}