#include "FallProto_win.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <filesystem>
#include <fstream>

const std::string ProtoTypeNames[6] = { "Items", "Critters", "Scenery", "Walls", "Tiles", "Misc" };
const std::string FRMType[8] = { "items", "critters", "scenery", "walls", "tiles", "backgrnd", "intrface", "inven" };
const std::string ItemSubType[7] = { "Armor", "Containers", "Drug", "Weapon", "Ammo", "Misc", "Key" };
const std::string MaterialName[8] = { "Glass", "Metal", "Plastic", "Wood", "Dirt", "Stone", "Cement", "Leather" };
const std::string WeapAnimCodes[11] = { "None (A)", "Knife (D)", "Club (E)", "Sledgehammer (F)", "Spear (G)", "Pistol (H)", "SMG (I)", "Rifle (J)", "Big Gun (K)", "Minigun (L)", "Rocket Launcher (M)" };
const std::string DmgType[7] = { "Normal", "Laser", "Fire", "Plasma", "Electrical", "EMP", "Explosive" };
const std::string CritBodyTypes[3] = { "Biped", "Quadruped", "Robotic" };

bool ReadFallProto(std::filesystem::path& filename, FallProto_t*& file)
{
	delete file;
	file = nullptr;
	ByteReader* reader = new ByteReader;
	if (!reader->Reset(filename.string(), ByteReader::BigEndian)) return false;
	file = new FallProto_t(reader);
	file->Filename = filename.stem().string();
	reader->Close();
	delete reader;

	return true;
}

void ExportFallProto(FallProto_t*& file)
{
	std::string str = "";



	std::ofstream out;
	out.open(file->Filename + ".txt");
	if (out.is_open())
	{
		out << str << std::endl;
	}
	out.close();
}

std::string GetProtoFlags(uint32_t& flags)
{
	std::string result = "" + std::to_string(flags) + "\n";

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

std::string GetItemFlags(uint32_t& flags)
{
	std::string result = "" + std::to_string(flags) + "\n";

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

std::string GetCritterFlags(uint32_t& flags)
{
	std::string result = "" + std::to_string(flags) + "\n";

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

void RenderItemProto(ItemProto_t* itemPro)
{
	ImGui::SameLine();
	ImGui::BeginChild("Item", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
	ImGui::Text("Item Flags: %s", GetItemFlags(itemPro->FlagsExt).c_str());
	//ImGui::Text("Item Script ID: %i", itemPro->ScriptId);
	ImGui::Text("Item Subtype: %s", ItemSubType[itemPro->Subtype].c_str());
	ImGui::Text("Item Material: %s", MaterialName[itemPro->Material].c_str());
	ImGui::Text("Item Volume: %i", itemPro->Volume);
	ImGui::Text("Item Weight: %i", itemPro->Weight);
	ImGui::Text("Item Cost: %i", itemPro->Cost);
	ImGui::Text("Item Inventory FID Type: %s", FRMType[itemPro->InvFID->PIDType].c_str());
	ImGui::Text("Item Inventory FID .LST Num: %i", itemPro->InvFID->PIDNum);
	ImGui::Text("Item Sound ID: %i", itemPro->SoundID);
	ImGui::EndChild();
	if (itemPro->ArmorData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Armor", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Armor AC: %i", itemPro->ArmorData->AC);
		ImGui::Text("Armor Resists:");
		for (size_t i = 0; i < 7; i++)
		{
			ImGui::Text("Resist %i, Threshold %i", itemPro->ArmorData->Resists[i], itemPro->ArmorData->Thresholds[i]);
		}
		ImGui::Text("Armor Perk: %i", itemPro->ArmorData->Perk);
		ImGui::Text("Armor Male FID Type: %s", FRMType[itemPro->ArmorData->MaleCRType->PIDType].c_str());
		ImGui::Text("Armor Male FID .LST Num: %i", itemPro->ArmorData->MaleCRType->PIDNum);
		ImGui::Text("Armor Female FID Type: %s", FRMType[itemPro->ArmorData->FemaleCRType->PIDType].c_str());
		ImGui::Text("Armor Female FID .LST Num: %i", itemPro->ArmorData->FemaleCRType->PIDNum);
		ImGui::EndChild();
	}
	if (itemPro->ContData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Container", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Container Max Volume: %i", itemPro->ContData->MaxVolume);
		ImGui::Text("Container Flags: %i", itemPro->ContData->ContFlags);
		if (ISFLAG(itemPro->ContData->ContFlags, 0x00000001))
		{
			ImGui::Text("CANT_PICKUP: %i", itemPro->ContData->ContFlags);
		}
		if (ISFLAG(itemPro->ContData->ContFlags, 0x00000008))
		{
			ImGui::Text("GROUND_LEVEL: %i", itemPro->ContData->ContFlags);
		}
		ImGui::EndChild();
	}
	if (itemPro->DrugData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Drug", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Drug Stats:");
		for (size_t i = 0; i < 3; i++)
		{
			ImGui::SameLine();
			ImGui::Text("%i", itemPro->DrugData->Stats[i]);
		}
		ImGui::Text("Drug Mods1:");
		for (size_t i = 0; i < 3; i++)
		{
			ImGui::SameLine();
			ImGui::Text("%i", itemPro->DrugData->FirstMod[i]);
		}
		ImGui::Text("Drug Duration 1: %i", itemPro->DrugData->Duration1);
		ImGui::Text("Drug Mods2:");
		for (size_t i = 0; i < 3; i++)
		{
			ImGui::SameLine();
			ImGui::Text("%i", itemPro->DrugData->SecondMod[i]);
		}
		ImGui::Text("Drug Duration 2: %i", itemPro->DrugData->Duration2);
		ImGui::Text("Drug Mods3:");
		for (size_t i = 0; i < 3; i++)
		{
			ImGui::SameLine();
			ImGui::Text("%i", itemPro->DrugData->ThirdMod[i]);
		}
		ImGui::Text("Drug Addict Rate: %i", itemPro->DrugData->AddictRate);
		ImGui::Text("Drug Addict Effect: %i", itemPro->DrugData->AddictEffect);
		ImGui::Text("Drug Addict Delay: %i", itemPro->DrugData->AddictDelay);
		ImGui::EndChild();
	}
	if (itemPro->WeapData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Weapon", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Weapon Anim Code: %s", WeapAnimCodes[itemPro->WeapData->AnimCode].c_str());
		ImGui::Text("Weapon Min DMG: %i", itemPro->WeapData->MinDmg);
		ImGui::Text("Weapon Max DMG: %i", itemPro->WeapData->MaxDmg);
		ImGui::Text("Weapon DMG Type: %s", DmgType[itemPro->WeapData->DmgType].c_str());
		ImGui::Text("Weapon Max Range 1: %i", itemPro->WeapData->MaxRange1);
		ImGui::Text("Weapon Max Range 2: %i", itemPro->WeapData->MaxRange2);
		ImGui::Text("Weapon Projectile PID Type: %i", itemPro->WeapData->ProjPID->PIDType);
		ImGui::Text("Weapon Projectile PID .LST Num: %i", itemPro->WeapData->ProjPID->PIDNum);
		ImGui::Text("Weapon Min Strengh: %i", itemPro->WeapData->MinST);
		ImGui::Text("Weapon AP Cost 1: %i", itemPro->WeapData->APCost1);
		ImGui::Text("Weapon AP Cost 2: %i", itemPro->WeapData->APCost2);
		ImGui::Text("Weapon Crit Fail: %i", itemPro->WeapData->CritFail);
		ImGui::Text("Weapon Perk: %i", itemPro->WeapData->Perk);
		ImGui::Text("Weapon Rounds: %i", itemPro->WeapData->Rounds);
		ImGui::Text("Weapon Caliber: %i", itemPro->WeapData->Caliber);
		ImGui::Text("Weapon Ammo PID Type: %i", itemPro->WeapData->AmmoPID->PIDType);
		ImGui::Text("Weapon Ammo PID .LST Num: %i", itemPro->WeapData->AmmoPID->PIDNum);
		ImGui::Text("Weapon Max Ammo: %i", itemPro->WeapData->MaxAmmo);
		ImGui::Text("Weapon Sound ID: %i", itemPro->WeapData->SoundId);
		ImGui::EndChild();
	}
	if (itemPro->AmmoData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Ammo", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Ammo Caliber: %i", itemPro->AmmoData->Caliber);
		ImGui::Text("Ammo Quantity: %i", itemPro->AmmoData->Quantity);
		ImGui::Text("Ammo AC Modifier: %i", itemPro->AmmoData->ACMod);
		ImGui::Text("Ammo DR Modifier: %i", itemPro->AmmoData->DRMod);
		ImGui::Text("Ammo DMG Multiplier: %i", itemPro->AmmoData->DmgMult);
		ImGui::Text("Ammo DMG Divisioner: %i", itemPro->AmmoData->DmgDiv);
		ImGui::EndChild();
	}
	if (itemPro->MiscData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Misc", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Misc Power PID Type: %i", itemPro->MiscData->PowerPID->PIDType);
		ImGui::Text("Misc Power PID .LST Num: %i", itemPro->MiscData->PowerPID->PIDNum);
		ImGui::Text("Misc Power Type: %i", itemPro->MiscData->PowerType);
		ImGui::Text("Misc Charges: %i", itemPro->MiscData->Charges);
		ImGui::EndChild();
	}
	if (itemPro->KeyData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Key", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Key ID: %i", itemPro->KeyData->KeyID);
		ImGui::EndChild();
	}
}

void RenderCritProto(CritterProto_t* critPro)
{
	ImGui::SameLine();
	ImGui::BeginChild("Critter", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
	ImGui::Text("Critter Action Flags: %i", critPro->ActionFlags);
	ImGui::Text("Critter Script Type: %i", critPro->ScriptID->PIDType);
	ImGui::Text("Critter Script Num: %i", critPro->ScriptID->PIDNum);
	ImGui::Text("Critter Head FID Type: %s", FRMType[critPro->HeadFID->PIDType].c_str());
	ImGui::Text("Critter Head FID .LST Num: %i", critPro->HeadFID->PIDNum);
	ImGui::Text("Critter AI ID: %i", critPro->AiID);
	ImGui::Text("Critter Team ID: %i", critPro->TeamID);
	ImGui::Text("Critter Flags: %s", GetCritterFlags(critPro->CritFlags).c_str());
	ImGui::Text("Critter Base SPECIAL:");
	for (size_t i = 0; i < 7; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->SPECIAL[i]);
	}
	ImGui::Text("Critter Base HP: %i", critPro->HP);
	ImGui::Text("Critter Base AP: %i", critPro->AP);
	ImGui::Text("Critter Base AC: %i", critPro->AC);
	ImGui::Text("Critter Base Unarmed DMG: %i", critPro->UnarmedDMG);
	ImGui::Text("Critter Base Melee DMG: %i", critPro->MeleeDMG);
	ImGui::Text("Critter Base Carry Weight: %i", critPro->CarryWeight);
	ImGui::Text("Critter Base Seqence: %i", critPro->Sequence);
	ImGui::Text("Critter Base Heal Rate: %i", critPro->HealRate);
	ImGui::Text("Critter Base Crit Chance: %i", critPro->CritChance);
	ImGui::Text("Critter Base Better Criticals: %i", critPro->BetterCrits);
	ImGui::Text("Critter Base DT:");
	for (size_t i = 0; i < 7; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->Thresholds[i]);
	}
	ImGui::Text("Critter Base DR:");
	for (size_t i = 0; i < 9; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->Resists[i]);
	}
	ImGui::Text("Critter Base Age: %i", critPro->Age);
	ImGui::Text("Critter Base Sex: %i", critPro->Sex);
	ImGui::Text("Critter Bonus SPECIAL:");
	for (size_t i = 0; i < 7; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->BonusSPECIAL[i]);
	}
	ImGui::Text("Critter Bonus HP: %i", critPro->BonusHP);
	ImGui::Text("Critter Bonus AP: %i", critPro->BonusAP);
	ImGui::Text("Critter Bonus AC: %i", critPro->BonusAC);
	ImGui::Text("Critter Bonus Unarmed DMG: %i", critPro->BonusUnarmedDMG);
	ImGui::Text("Critter Bonus Melee DMG: %i", critPro->BonusMeleeDMG);
	ImGui::Text("Critter Bonus Carry Weight: %i", critPro->BonusCarryWeight);
	ImGui::Text("Critter Bonus Seqence: %i", critPro->BonusSequence);
	ImGui::Text("Critter Bonus Heal Rate: %i", critPro->BonusHealRate);
	ImGui::Text("Critter Bonus Crit Chance: %i", critPro->BonusCritChance);
	ImGui::Text("Critter Bonus Better Crits: %i", critPro->BonusBetterCrits);
	ImGui::Text("Critter Bonus DT:");
	for (size_t i = 0; i < 7; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->BonusThresholds[i]);
	}
	ImGui::Text("Critter Bonus DR:");
	for (size_t i = 0; i < 9; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->BonusResists[i]);
	}
	ImGui::Text("Critter Bonus Age: %i", critPro->BonusAge);
	ImGui::Text("Critter Bonus Sex: %i", critPro->BonusSex);
	ImGui::Text("Critter Skills:");
	for (size_t i = 0; i < 18; i++)
	{
		ImGui::SameLine();
		ImGui::Text("%i", critPro->Skills[i]);
	}
	ImGui::Text("Critter Body Type: %s", CritBodyTypes[critPro->BodyType].c_str());
	ImGui::Text("Critter EXP For Kill: %i", critPro->ExpVal);
	ImGui::Text("Critter Kill Type: %i", critPro->KillType);
	ImGui::Text("Critter DMG Type: %s", DmgType[critPro->DmgType].c_str());
	ImGui::EndChild();
}

void RenderSceneryProto(ScenProto_t* scenPro)
{
	ImGui::SameLine();
	ImGui::BeginChild("Scenery", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
	ImGui::Text("Scenery Wall Light Flags: %i", scenPro->WallLightFlags);
	ImGui::Text("Scenery Action Flags: %i", scenPro->ActionFlags);
	ImGui::Text("Scenery Script Type: %i", scenPro->ScriptID->PIDType);
	ImGui::Text("Scenery Script Num: %i", scenPro->ScriptID->PIDNum);
	ImGui::Text("Scenery Subtype: %i", scenPro->SubType);
	ImGui::Text("Scenery Material: %s", MaterialName[scenPro->Material].c_str());
	ImGui::Text("Scenery Sound ID: %i", scenPro->SoundID);
	ImGui::EndChild();
	
	if (scenPro->DoorData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Door", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Door WalkThru: %s", (scenPro->DoorData->WalkThru == 0x0000000F ? "TRUE" : "FALSE"));
		ImGui::Text("Door Flag: %i", scenPro->DoorData->DoorFlag);
		ImGui::EndChild();
	}
	if (scenPro->StairData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Stair", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Stair Destination Elevation: %i", scenPro->StairData->DestElev);
		ImGui::Text("Stair Destination Tile: %i", scenPro->StairData->DestTile);
		ImGui::Text("Stair Destination Map: %i", scenPro->StairData->DestMap);
		ImGui::EndChild();
	}
	if (scenPro->ElevData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Elevator", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Elevator Type: %i", scenPro->ElevData->ElevType);
		ImGui::Text("Elevator Level: %i", scenPro->ElevData->ElevLevel);
		ImGui::EndChild();
	}
	if (scenPro->LadderData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Ladder", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Ladder Destination Elevation: %i", scenPro->LadderData->DestElev);
		ImGui::Text("Ladder Destination Tile: %i", scenPro->LadderData->DestTile);
		ImGui::EndChild();
	}
	if (scenPro->GenericData != nullptr)
	{
		ImGui::SameLine();
		ImGui::BeginChild("Generic", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("Generic Unknown: %i", scenPro->GenericData->Unknown);
		ImGui::EndChild();
	}
}

void RenderWallProto(WallProto_t* wallPro)
{
	ImGui::SameLine();
	ImGui::BeginChild("Wall", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
	ImGui::Text("Wall Light Flags:");
	if (ISFLAG(wallPro->WallLightFlags, 0x0000)) { ImGui::Text("North / South"); }
	if (ISFLAG(wallPro->WallLightFlags, 0x0800)) { ImGui::Text("East / West"); }
	if (ISFLAG(wallPro->WallLightFlags, 0x1000)) { ImGui::Text("North Corner"); }
	if (ISFLAG(wallPro->WallLightFlags, 0x2000)) { ImGui::Text("South Corner"); }
	if (ISFLAG(wallPro->WallLightFlags, 0x4000)) { ImGui::Text("East Corner"); }
	if (ISFLAG(wallPro->WallLightFlags, 0x8000)) { ImGui::Text("West Corner"); }
	ImGui::Text("Wall Action Flags: %i", wallPro->ActionFlags);
	ImGui::Text("Wall Script Type: %i", wallPro->ScriptID->PIDType);
	ImGui::Text("Wall Script Num: %i", wallPro->ScriptID->PIDNum);
	ImGui::Text("Wall Material: %s", MaterialName[wallPro->Material].c_str());
	ImGui::EndChild();
}

void RenderTileProto(TileProto_t* tilePro)
{
	ImGui::SameLine();
	ImGui::BeginChild("Tile", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
	ImGui::Text("Tile Material: %s", MaterialName[tilePro->Material].c_str());
	ImGui::EndChild();
}

void FallProtoWindow::DrawWin()
{
	if (!GetVisible()) return;
	ImGui::Begin("Fallout .PRO Reading Tool");

	ImGui::InputText("Fallout .PRO file path", &Filename);
	if (ImGui::Button("Load File"))
	{
		std::filesystem::path filepath = Filename;
		ReadFallProto(filepath, File);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export File"))
	{
		ExportFallProto(File);
	}

	if (File != nullptr)
	{
		ImGui::BeginChild("Proto", ImVec2(100, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		ImGui::Text("PID Type: %s", File->PID->PIDNum);
		ImGui::Text("PID Num: %i", File->PID->PIDNum);
		ImGui::Text("Text ID: %i", File->TextId);
		ImGui::Text("FID Type: %s", FRMType[File->FID->PIDType].c_str());
		ImGui::Text(".LST Num: %i", File->FID->PIDNum);
		ImGui::Text("Light Radius: %i", File->LightRad);
		ImGui::Text("Light Intence: %i", File->LightIntence);
		ImGui::Text("Flags: %s", GetProtoFlags(File->Flags).c_str());
		ImGui::EndChild();
		if (File->ItemPro != nullptr)
		{
			RenderItemProto(File->ItemPro);
		}
		if (File->CritPro != nullptr)
		{
			RenderCritProto(File->CritPro);
		}
		if (File->ScenPro != nullptr)
		{
			RenderSceneryProto(File->ScenPro);
		}
		if (File->WallPro != nullptr)
		{
			RenderWallProto(File->WallPro);
		}
		if (File->TilePro != nullptr)
		{
			RenderTileProto(File->TilePro);
		}
	}

	ImGui::End();
}

void FallProtoWindow::InitWin()
{}