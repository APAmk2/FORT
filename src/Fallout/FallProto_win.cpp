#include "FallProto_win.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <filesystem>
#include <fstream>

using namespace std;

const string protoTypeNames[6] = { "Items", "Critters", "Scenery", "Walls", "Tiles", "Misc" };
const string frmType[8] = { "items", "critters", "scenery", "walls", "tiles", "backgrnd", "intrface", "inven" };
const string itemSubType[7] = { "Armor", "Containers", "Drug", "Weapon", "Ammo", "Misc", "Key" };
const string materialName[8] = { "Glass", "Metal", "Plastic", "Wood", "Dirt", "Stone", "Cement", "Leather" };
const string weapAnimCodes[11] = { "None (A)", "Knife (D)", "Club (E)", "Sledgehammer (F)", "Spear (G)", "Pistol (H)", "SMG (I)", "Rifle (J)", "Big Gun (K)", "Minigun (L)", "Rocket Launcher (M)" };
const string dmgType[7] = { "Normal", "Laser", "Fire", "Plasma", "Electrical", "EMP", "Explosive" };
const string critBodyTypes[3] = { "Biped", "Quadruped", "Robotic" };

bool readFallProto(std::filesystem::path& filename, FallProto_t*& file)
{
	delete file;
	file = nullptr;
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::BigEndian)) return false;
	file = new FallProto_t(reader);
	file->filename = filename.stem().string();
	reader->Close();
	delete reader;

	return true;
}

void exportFallProto(FallProto_t*& file)
{
	std::string str = "";



	ofstream out;
	out.open(file->filename + ".txt");
	if (out.is_open())
	{
		out << str << endl;
	}
	out.close();
}

string GetProtoFlags(uint32_t& flags)
{
	string result = "" + to_string(flags) + "\n";

	if (ISFLAG(flags, 0x00000008)) { result += "FLAT\n"; }
	if (ISFLAG(flags, 0x00000010)) { result += "NO_BLOCK\n"; }
	if (ISFLAG(flags, 0x00000800)) { result += "MULTIHEX\n"; }
	if (ISFLAG(flags, 0x00001000)) { result += "NO_HIGHLIGHT\n"; }
	if (ISFLAG(flags, 0x00004000)) { result += "TRANS_RED\n"; }
	if (ISFLAG(flags, 0x00008000)) { result += "TRANS_NONE\n"; }
	if (ISFLAG(flags, 0x00010000)) { result += "TRANS_WALL\n"; }
	if (ISFLAG(flags, 0x00020000)) { result += "TRANS_GLASS\n"; }
	if (ISFLAG(flags, 0x00040000)) { result += "TRANS_STEAM\n"; }
	if (ISFLAG(flags, 0x00080000)) { result += "TRANS_ENERGY\n"; }
	if (ISFLAG(flags, 0x10000000)) { result += "WALL_TRANS_END\n"; }
	if (ISFLAG(flags, 0x20000000)) { result += "LIGHT_THRU\n"; }
	if (ISFLAG(flags, 0x80000000)) { result += "SHOOT_THRU\n"; }

	return result;
}

string GetItemFlags(uint32_t& flags)
{
	string result = "" + to_string(flags) + "\n";

	if (ISFLAG(flags, 0x00000100)) { result += "BIG_GUN\n"; }
	if (ISFLAG(flags, 0x00000200)) { result += "2HANDS\n"; }
	if (ISFLAG(flags, 0x00000800)) { result += "USE\n"; }
	if (ISFLAG(flags, 0x00001000)) { result += "USE_ON_SMTH\n"; }
	if (ISFLAG(flags, 0x00008000)) { result += "PICKUP\n"; }
	if (ISFLAG(flags, 0x08000000)) { result += "HIDDEN\n"; }

	result += "Use Flags:\n";
	uint8_t useFlag = flags;
	for (size_t i = 0; i < 2; i++)
	{
		uint8_t currUseFlag = useFlag;
		if (i == 0)
		{
			currUseFlag = useFlag << 4;
			currUseFlag = currUseFlag >> 4;
		}
		else
		{
			currUseFlag = useFlag >> 4;
		}
		if (currUseFlag == 0x00) { result += "NONE\n"; }
		if (currUseFlag == 0x01) { result += "PUNCH\n"; }
		if (currUseFlag == 0x02) { result += "KICK\n"; }
		if (currUseFlag == 0x03) { result += "SWING\n"; }
		if (currUseFlag == 0x04) { result += "THRUST\n"; }
		if (currUseFlag == 0x05) { result += "THROW\n"; }
		if (currUseFlag == 0x06) { result += "FIRE_SINGLE\n"; }
		if (currUseFlag == 0x07) { result += "FIRE_BURST\n"; }
		if (currUseFlag == 0x08) { result += "FLAME\n"; }
	}

	return result;
}

string GetCritterFlags(uint32_t& flags)
{
	string result = "" + to_string(flags) + "\n";

	if (ISFLAG(flags, 0x00000002)) { result += "CAN_BARTER\n"; }
	if (ISFLAG(flags, 0x00000020)) { result += "CANT_STEAL\n"; }
	if (ISFLAG(flags, 0x00000040)) { result += "CANT_DROP\n"; }
	if (ISFLAG(flags, 0x00000080)) { result += "NO_LOOSE_LIMBS\n"; }
	if (ISFLAG(flags, 0x00000100)) { result += "NO_DISAPPEAR\n"; }
	if (ISFLAG(flags, 0x00000200)) { result += "NO_HEAL\n"; }
	if (ISFLAG(flags, 0x00000400)) { result += "INVULNERABLE\n"; }
	if (ISFLAG(flags, 0x00000800)) { result += "NO_DEAD_BODY\n"; }
	if (ISFLAG(flags, 0x00001000)) { result += "SPECIAL_DEAD\n"; }
	if (ISFLAG(flags, 0x00002000)) { result += "RANGED_MELEE\n"; }
	if (ISFLAG(flags, 0x00004000)) { result += "NO_KNOCK\n"; }

	return result;
}

void renderItemProto(ItemProto_t* itemPro)
{
	ImGui::SameLine();
	ImGui::BeginChild("Item", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
	ImGui::Text("Item Flags: %s", GetItemFlags(itemPro->flagsExt).c_str());
	ImGui::Text("Item Script ID: %i", itemPro->scriptId);
	ImGui::Text("Item Subtype: %s", itemSubType[itemPro->subtype].c_str());
	ImGui::Text("Item Material: %s", materialName[itemPro->material].c_str());
	ImGui::Text("Item Volume: %i", itemPro->volume);
	ImGui::Text("Item Weight: %i", itemPro->weight);
	ImGui::Text("Item Cost: %i", itemPro->cost);
	ImGui::Text("Item Inventory FID Type: %s", frmType[itemPro->invFID->FIDType].c_str());
	ImGui::Text("Item Inventory FID .LST Num: %i", itemPro->invFID->FIDNum);
	ImGui::Text("Item Sound ID: %i", itemPro->soundID);
	ImGui::EndChild();
	if (itemPro->armorData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Armor", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Armor AC: %i", itemPro->armorData->AC);
		ImGui::Text("Armor Resists:");
		for (size_t i = 0; i < 7; i++)
		{
			ImGui::Text("Resist %i, Threshold %i", itemPro->armorData->resists[i], itemPro->armorData->thresholds[i]);
		}
		ImGui::Text("Armor Perk: %i", itemPro->armorData->perk);
		ImGui::Text("Armor Male FID Type: %s", frmType[itemPro->armorData->maleCRType->FIDType].c_str());
		ImGui::Text("Armor Male FID .LST Num: %i", itemPro->armorData->maleCRType->FIDNum);
		ImGui::Text("Armor Female FID Type: %s", frmType[itemPro->armorData->femaleCRType->FIDType].c_str());
		ImGui::Text("Armor Female FID .LST Num: %i", itemPro->armorData->femaleCRType->FIDNum);
		ImGui::EndChild();
	}
	if (itemPro->contData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Container", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Container Max Volume: %i", itemPro->contData->maxVolume);
		ImGui::Text("Container Flags: %i", itemPro->contData->contFlags);
		if (ISFLAG(itemPro->contData->contFlags, 0x00000001))
		{
			ImGui::Text("CANT_PICKUP: %i", itemPro->contData->contFlags);
		}
		if (ISFLAG(itemPro->contData->contFlags, 0x00000008))
		{
			ImGui::Text("GROUND_LEVEL: %i", itemPro->contData->contFlags);
		}
		ImGui::EndChild();
	}
	if (itemPro->drugData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Drug", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Drug Stats:");
		for (size_t i = 0; i < 3; i++)
		{
			ImGui::SameLine();
			ImGui::Text("%i", itemPro->drugData->stats[i]);
		}
		ImGui::Text("Drug Mods1:");
		for (size_t i = 0; i < 3; i++)
		{
			ImGui::SameLine();
			ImGui::Text("%i", itemPro->drugData->firstMod[i]);
		}
		ImGui::Text("Drug Duration 1: %i", itemPro->drugData->duration1);
		ImGui::Text("Drug Mods2:");
		for (size_t i = 0; i < 3; i++)
		{
			ImGui::SameLine();
			ImGui::Text("%i", itemPro->drugData->secondMod[i]);
		}
		ImGui::Text("Drug Duration 2: %i", itemPro->drugData->duration2);
		ImGui::Text("Drug Mods3:");
		for (size_t i = 0; i < 3; i++)
		{
			ImGui::SameLine();
			ImGui::Text("%i", itemPro->drugData->thirdMod[i]);
		}
		ImGui::Text("Drug Addict Rate: %i", itemPro->drugData->addictionRate);
		ImGui::Text("Drug Addict Effect: %i", itemPro->drugData->addictionEffect);
		ImGui::Text("Drug Addict Delay: %i", itemPro->drugData->addictionDelay);
		ImGui::EndChild();
	}
	if (itemPro->weapData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Weapon", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Weapon Anim Code: %s", weapAnimCodes[itemPro->weapData->animCode].c_str());
		ImGui::Text("Weapon Min DMG: %i", itemPro->weapData->minDmg);
		ImGui::Text("Weapon Max DMG: %i", itemPro->weapData->maxDmg);
		ImGui::Text("Weapon DMG Type: %s", dmgType[itemPro->weapData->dmgType].c_str());
		ImGui::Text("Weapon Max Range 1: %i", itemPro->weapData->maxRange1);
		ImGui::Text("Weapon Max Range 2: %i", itemPro->weapData->maxRange2);
		ImGui::Text("Weapon Projectile PID Type: %i", itemPro->weapData->projPID->FIDType);
		ImGui::Text("Weapon Projectile PID .LST Num: %i", itemPro->weapData->projPID->FIDNum);
		ImGui::Text("Weapon Min Strengh: %i", itemPro->weapData->minST);
		ImGui::Text("Weapon AP Cost 1: %i", itemPro->weapData->apCost1);
		ImGui::Text("Weapon AP Cost 2: %i", itemPro->weapData->apCost2);
		ImGui::Text("Weapon Crit Fail: %i", itemPro->weapData->critFail);
		ImGui::Text("Weapon Perk: %i", itemPro->weapData->perk);
		ImGui::Text("Weapon Rounds: %i", itemPro->weapData->rounds);
		ImGui::Text("Weapon Caliber: %i", itemPro->weapData->caliber);
		ImGui::Text("Weapon Ammo PID Type: %i", itemPro->weapData->ammoPID->FIDType);
		ImGui::Text("Weapon Ammo PID .LST Num: %i", itemPro->weapData->ammoPID->FIDNum);
		ImGui::Text("Weapon Max Ammo: %i", itemPro->weapData->maxAmmo);
		ImGui::Text("Weapon Sound ID: %i", itemPro->weapData->soundId);
		ImGui::EndChild();
	}
	if (itemPro->ammoData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Ammo", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Ammo Caliber: %i", itemPro->ammoData->caliber);
		ImGui::Text("Ammo Quantity: %i", itemPro->ammoData->quantity);
		ImGui::Text("Ammo AC Modifier: %i", itemPro->ammoData->ACMod);
		ImGui::Text("Ammo DR Modifier: %i", itemPro->ammoData->DRMod);
		ImGui::Text("Ammo DMG Multiplier: %i", itemPro->ammoData->dmgMult);
		ImGui::Text("Ammo DMG Divisioner: %i", itemPro->ammoData->dmgDiv);
		ImGui::EndChild();
	}
	if (itemPro->miscData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Misc", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Misc Power PID Type: %i", itemPro->miscData->powerPID->FIDType);
		ImGui::Text("Misc Power PID .LST Num: %i", itemPro->miscData->powerPID->FIDNum);
		ImGui::Text("Misc Power Type: %i", itemPro->miscData->powerType);
		ImGui::Text("Misc Charges: %i", itemPro->miscData->charges);
		ImGui::EndChild();
	}
	if (itemPro->keyData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Key", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Key ID: %i", itemPro->keyData->keyID);
		ImGui::EndChild();
	}
}

void renderCritProto(CritterProto_t* critPro)
{
	ImGui::SameLine();
	ImGui::BeginChild("Critter", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
	ImGui::Text("Critter Action Flags: %i", critPro->actionFlags);
	ImGui::Text("Critter Script Type: %i", critPro->scriptType);
	ImGui::Text("Critter Script Num: %i", critPro->scriptNum);
	ImGui::Text("Critter Head FID Type: %s", frmType[critPro->headFID->FIDType].c_str());
	ImGui::Text("Critter Head FID .LST Num: %i", critPro->headFID->FIDNum);
	ImGui::Text("Critter AI ID: %i", critPro->aiID);
	ImGui::Text("Critter Team ID: %i", critPro->teamID);
	ImGui::Text("Critter Flags: %s", GetCritterFlags(critPro->critFlags).c_str());
	ImGui::Text("Critter Base SPECIAL:");
	for (size_t i = 0; i < 7; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->SPECIAL[i]);
	}
	ImGui::Text("Critter Base HP: %i", critPro->HP);
	ImGui::Text("Critter Base AP: %i", critPro->AP);
	ImGui::Text("Critter Base AC: %i", critPro->AC);
	ImGui::Text("Critter Base Unarmed DMG: %i", critPro->unarmedDMG);
	ImGui::Text("Critter Base Melee DMG: %i", critPro->meleeDMG);
	ImGui::Text("Critter Base Carry Weight: %i", critPro->carryWeight);
	ImGui::Text("Critter Base Seqence: %i", critPro->sequence);
	ImGui::Text("Critter Base Heal Rate: %i", critPro->healRate);
	ImGui::Text("Critter Base Crit Chance: %i", critPro->critChance);
	ImGui::Text("Critter Base Better Criticals: %i", critPro->betterCrits);
	ImGui::Text("Critter Base DT:");
	for (size_t i = 0; i < 7; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->thresholds[i]);
	}
	ImGui::Text("Critter Base DR:");
	for (size_t i = 0; i < 9; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->resists[i]);
	}
	ImGui::Text("Critter Base Age: %i", critPro->age);
	ImGui::Text("Critter Base Sex: %i", critPro->sex);
	ImGui::Text("Critter Bonus SPECIAL:");
	for (size_t i = 0; i < 7; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->bonusSPECIAL[i]);
	}
	ImGui::Text("Critter Bonus HP: %i", critPro->bonusHP);
	ImGui::Text("Critter Bonus AP: %i", critPro->bonusAP);
	ImGui::Text("Critter Bonus AC: %i", critPro->bonusAC);
	ImGui::Text("Critter Bonus Unarmed DMG: %i", critPro->bonusUnarmedDMG);
	ImGui::Text("Critter Bonus Melee DMG: %i", critPro->bonusMeleeDMG);
	ImGui::Text("Critter Bonus Carry Weight: %i", critPro->bonusCarryWeight);
	ImGui::Text("Critter Bonus Seqence: %i", critPro->bonusSequence);
	ImGui::Text("Critter Bonus Heal Rate: %i", critPro->bonusHealRate);
	ImGui::Text("Critter Bonus Crit Chance: %i", critPro->bonusCritChance);
	ImGui::Text("Critter Bonus Better Crits: %i", critPro->bonusBetterCrits);
	ImGui::Text("Critter Bonus DT:");
	for (size_t i = 0; i < 7; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->bonusThresholds[i]);
	}
	ImGui::Text("Critter Bonus DR:");
	for (size_t i = 0; i < 9; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->bonusResists[i]);
	}
	ImGui::Text("Critter Bonus Age: %i", critPro->bonusAge);
	ImGui::Text("Critter Bonus Sex: %i", critPro->bonusSex);
	ImGui::Text("Critter Skills:");
	for (size_t i = 0; i < 18; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->skills[i]);
	}
	ImGui::Text("Critter Body Type: %s", critBodyTypes[critPro->bodyType].c_str());
	ImGui::Text("Critter EXP For Kill: %i", critPro->expVal);
	ImGui::Text("Critter Kill Type: %i", critPro->killType);
	ImGui::Text("Critter DMG Type: %s", dmgType[critPro->dmgType].c_str());
	ImGui::EndChild();
}

void renderSceneryProto(SceneryProto_t* scenPro)
{
	ImGui::SameLine();
	ImGui::BeginChild("Scenery", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
	ImGui::Text("Scenery Wall Light Flags: %i", scenPro->wallLightFlags);
	ImGui::Text("Scenery Action Flags: %i", scenPro->actionFlags);
	ImGui::Text("Scenery Script Type: %i", scenPro->scriptType);
	ImGui::Text("Scenery Script Num: %i", scenPro->scriptNum);
	ImGui::Text("Scenery Subtype: %i", scenPro->subType);
	ImGui::Text("Scenery Material: %s", materialName[scenPro->material].c_str());
	ImGui::Text("Scenery Sound ID: %i", scenPro->soundID);
	ImGui::EndChild();
	
	if (scenPro->doorData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Door", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Door WalkThru: %s", (scenPro->doorData->walkThruFlag == 0x0000000F ? "TRUE" : "FALSE"));
		ImGui::Text("Door Flag: %i", scenPro->doorData->doorFlag);
		ImGui::EndChild();
	}
	if (scenPro->stairData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Stair", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Stair Destination Elevation: %i", scenPro->stairData->destElev);
		ImGui::Text("Stair Destination Tile: %i", scenPro->stairData->destTile);
		ImGui::Text("Stair Destination Map: %i", scenPro->stairData->destMap);
		ImGui::EndChild();
	}
	if (scenPro->elevData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Elevator", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Elevator Type: %i", scenPro->elevData->elevType);
		ImGui::Text("Elevator Level: %i", scenPro->elevData->elevLevel);
		ImGui::EndChild();
	}
	if (scenPro->ladderData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Ladder", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Ladder Destination Elevation: %i", scenPro->ladderData->destElev);
		ImGui::Text("Ladder Destination Tile: %i", scenPro->ladderData->destTile);
		ImGui::EndChild();
	}
	if (scenPro->genericData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Generic", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Generic Unknown: %i", scenPro->genericData->unknown);
		ImGui::EndChild();
	}
}

void renderWallProto(WallProto_t* wallPro)
{
	ImGui::SameLine();
	ImGui::BeginChild("Wall", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
	ImGui::Text("Wall Light Flags:");
	if (ISFLAG(wallPro->wallLightFlags, 0x0000)) { ImGui::Text("North / South"); }
	if (ISFLAG(wallPro->wallLightFlags, 0x0800)) { ImGui::Text("East / West"); }
	if (ISFLAG(wallPro->wallLightFlags, 0x1000)) { ImGui::Text("North Corner"); }
	if (ISFLAG(wallPro->wallLightFlags, 0x2000)) { ImGui::Text("South Corner"); }
	if (ISFLAG(wallPro->wallLightFlags, 0x4000)) { ImGui::Text("East Corner"); }
	if (ISFLAG(wallPro->wallLightFlags, 0x8000)) { ImGui::Text("West Corner"); }
	ImGui::Text("Wall Action Flags: %i", wallPro->actionFlags);
	ImGui::Text("Wall Script Type: %i", wallPro->scriptType);
	ImGui::Text("Wall Script Num: %i", wallPro->scriptNum);
	ImGui::Text("Wall Material: %s", materialName[wallPro->material].c_str());
	ImGui::EndChild();
}

void renderTileProto(TileProto_t* tilePro)
{
	ImGui::SameLine();
	ImGui::BeginChild("Tile", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
	ImGui::Text("Tile Material: %s", materialName[tilePro->material].c_str());
	ImGui::EndChild();
}

void FallProtoWindow::drawWindow()
{
	if (!GetVisible()) return;
	ImGui::Begin("Fallout .pro Reading Tool");

	ImGui::InputText("Fallout .pro file path", &fallProtofilename);
	if (ImGui::Button("Load File"))
	{
		filesystem::path filepath = fallProtofilename;
		readFallProto(filepath, fallProtoFile);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		exportFallProto(fallProtoFile);
	}

	if (fallProtoFile != nullptr)
	{
		ImGui::BeginChild("Proto", ImVec2(100, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Object Type: %s", protoTypeNames[fallProtoFile->objType].c_str());
		ImGui::Text("PID: %i", fallProtoFile->pid);
		ImGui::Text("Text ID: %i", fallProtoFile->textId);
		ImGui::Text("FID Type: %s", frmType[fallProtoFile->FID->FIDType].c_str());
		ImGui::Text(".LST Num: %i", fallProtoFile->FID->FIDNum);
		ImGui::Text("Light Radius: %i", fallProtoFile->lightRad);
		ImGui::Text("Light Intence: %i", fallProtoFile->lightIntence);
		ImGui::Text("Flags: %s", GetProtoFlags(fallProtoFile->flags).c_str());
		ImGui::EndChild();
		if (fallProtoFile->itemPro != nullptr)
		{
			renderItemProto(fallProtoFile->itemPro);
		}
		if (fallProtoFile->critPro != nullptr)
		{
			renderCritProto(fallProtoFile->critPro);
		}
		if (fallProtoFile->scenPro != nullptr)
		{
			renderSceneryProto(fallProtoFile->scenPro);
		}
		if (fallProtoFile->wallPro != nullptr)
		{
			renderWallProto(fallProtoFile->wallPro);
		}
		if (fallProtoFile->tilePro != nullptr)
		{
			renderTileProto(fallProtoFile->tilePro);
		}
	}

	ImGui::End();
}

void FallProtoWindow::initWindow()
{}