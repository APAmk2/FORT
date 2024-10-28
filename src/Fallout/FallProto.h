#pragma once

#include "../Utils/ByteReader.hpp"
#include "../Utils/BaseDataTypes.h"
#include "FallShared.h"
#include <vector>
#include <string>

enum proto_type
{
	PROTO_ITEM = 0,
	PROTO_CRITTER,
	PROTO_SCENERY,
	PROTO_WALLS,
	PROTO_TILES,
	PROTO_MISC
};

enum item_subtype
{
	ITEM_TYPE_ARMOR = 0,
	ITEM_TYPE_CONTAINER,
	ITEM_TYPE_DRUG,
	ITEM_TYPE_WEAPON,
	ITEM_TYPE_AMMO,
	ITEM_TYPE_MISC,
	ITEM_TYPE_KEY
};

enum scen_subtype
{
	SCEN_TYPE_DOOR = 0,
	SCEN_TYPE_STAIR,
	SCEN_TYPE_ELEV,
	SCEN_TYPE_LADDER_BTM,
	SCEN_TYPE_LADDER_TOP,
	SCEN_TYPE_GENERIC
};

enum item_material
{
	MAT_GLASS = 0,
	MAT_METAL,
	MAT_PLASTIC,
	MAT_WOOD,
	MAT_DIRT,
	MAT_STONE,
	MAT_CEMENT,
	MAT_LEATHER
};

class ItemArmorProto_t
{
public:
	int32_t AC = 0;
	std::vector<int32_t> Resists;
	std::vector<int32_t> Thresholds;
	int32_t Perk = -1;
	PID_t* MaleCRType = nullptr;
	PID_t* FemaleCRType = nullptr;

	ItemArmorProto_t(ByteReader* reader);
};

class ItemContProto_t
{
public:
	uint32_t MaxVolume = 0;
	uint32_t ContFlags = 0;

	ItemContProto_t(ByteReader* reader);
};

class ItemDrugProto_t
{
public:
	std::vector<int32_t> Stats;
	std::vector<int32_t> FirstMod;
	uint32_t Duration1 = 0;
	std::vector<int32_t> SecondMod;
	uint32_t Duration2 = 0;
	std::vector<int32_t> ThirdMod;
	uint32_t AddictRate = 0;
	uint32_t AddictEffect = 0;
	uint32_t AddictDelay = 0;

	ItemDrugProto_t(ByteReader* reader);
};

class ItemWeapProto_t
{
public:
	uint32_t AnimCode = 0;
	uint32_t MinDmg = 0;
	uint32_t MaxDmg = 0;
	uint32_t DmgType = 0;
	uint32_t MaxRange1 = 0;
	uint32_t MaxRange2 = 0;
	PID_t* ProjPID = nullptr;
	uint32_t MinST = 0;
	uint32_t APCost1 = 0;
	uint32_t APCost2 = 0;
	uint32_t CritFail = 0;
	int32_t Perk = -1;
	uint32_t Rounds = 0;
	uint32_t Caliber = 0;
	PID_t* AmmoPID = nullptr;
	uint32_t MaxAmmo = 0;
	uint8_t SoundId = 0;

	ItemWeapProto_t(ByteReader* reader);
};

class ItemAmmoProto_t
{
public:
	uint32_t Caliber = 0;
	uint32_t Quantity = 0;
	uint32_t ACMod = 0;
	uint32_t DRMod = 0;
	int32_t DmgMult = 0;
	int32_t DmgDiv = 0;

	ItemAmmoProto_t(ByteReader* reader);
};

class ItemMiscProto_t
{
public:
	PID_t* PowerPID = nullptr;
	uint32_t PowerType = 0;
	uint32_t Charges = 0;

	ItemMiscProto_t(ByteReader* reader);
};

class ItemKeyProto_t
{
public:
	uint32_t KeyID = 0;

	ItemKeyProto_t(ByteReader* reader);
};

class ItemProto_t
{
public:
	uint32_t FlagsExt = 0;
	uint32_t ScriptId = 0;
	uint32_t Subtype = 0;
	uint32_t Material = MAT_GLASS;
	uint32_t Volume = 0;
	uint32_t Weight = 0;
	uint32_t Cost = 0;
	PID_t* InvFID = nullptr;
	uint8_t SoundID = 0;

	ItemArmorProto_t* ArmorData = nullptr;
	ItemContProto_t* ContData = nullptr;
	ItemDrugProto_t* DrugData = nullptr;
	ItemWeapProto_t* WeapData = nullptr;
	ItemAmmoProto_t* AmmoData = nullptr;
	ItemMiscProto_t* MiscData = nullptr;
	ItemKeyProto_t* KeyData = nullptr;

	ItemProto_t(ByteReader* reader);
};

class CritterProto_t
{
public:
	uint32_t ActionFlags = 0;
	PID_t* ScriptID = nullptr;
	PID_t* HeadFID = nullptr;
	uint32_t AiID = 0;
	uint32_t TeamID = 0;
	uint32_t CritFlags = 0;
	std::vector<int32_t> SPECIAL;
	int32_t HP = 0;
	int32_t AP = 0;
	int32_t AC = 0;
	int32_t UnarmedDMG = 0;
	int32_t MeleeDMG = 0;
	uint32_t CarryWeight = 0;
	uint32_t Sequence = 0;
	uint32_t HealRate = 0;
	uint32_t CritChance = 0;
	uint32_t BetterCrits = 0;
	std::vector<uint32_t> Thresholds;
	std::vector<uint32_t> Resists;
	uint32_t Age = 1;
	uint32_t Sex = 0;
	std::vector<int32_t> BonusSPECIAL;
	int32_t BonusHP = 0;
	int32_t BonusAP = 0;
	int32_t BonusAC = 0;
	int32_t BonusUnarmedDMG = 0;
	int32_t BonusMeleeDMG = 0;
	uint32_t BonusCarryWeight = 0;
	uint32_t BonusSequence = 0;
	uint32_t BonusHealRate = 0;
	uint32_t BonusCritChance = 0;
	uint32_t BonusBetterCrits = 0;
	std::vector<uint32_t> BonusThresholds;
	std::vector<uint32_t> BonusResists;
	uint32_t BonusAge = 1;
	uint32_t BonusSex = 0;
	std::vector<int32_t> Skills;
	uint32_t BodyType = 0;
	uint32_t ExpVal = 0;
	uint32_t KillType = 0;
	uint32_t DmgType = 0;

	CritterProto_t(ByteReader* reader);
};

class ScenDoorProto_t
{
public:
	uint32_t WalkThru = 0;
	uint32_t DoorFlag = 0xCCCCCCCC;

	ScenDoorProto_t(ByteReader* reader);
};

class ScenStairProto_t
{
public:
	uint8_t DestElev = 0;
	uint16_t DestTile = 0;
	uint32_t DestMap = 0;

	ScenStairProto_t(ByteReader* reader);
};

class ScenElevProto_t
{
public:
	uint32_t ElevType = 0;
	uint32_t ElevLevel = 0;

	ScenElevProto_t(ByteReader* reader);
};

class ScenLadderProto_t
{
public:
	uint8_t DestElev = 0;
	uint16_t DestTile = 0;

	ScenLadderProto_t(ByteReader* reader);
};

class ScenGenericProto_t
{
public:
	uint32_t Unknown = 0;

	ScenGenericProto_t(ByteReader* reader);
};

class ScenProto_t
{
public:
	uint16_t WallLightFlags = 0;
	uint16_t ActionFlags = 0;
	PID_t* ScriptID = nullptr;
	uint32_t SubType = 0;
	uint32_t Material = MAT_GLASS;
	uint8_t SoundID = 0;

	ScenDoorProto_t* DoorData = nullptr;
	ScenStairProto_t* StairData = nullptr;
	ScenElevProto_t* ElevData = nullptr;
	ScenLadderProto_t* LadderData = nullptr;
	ScenGenericProto_t* GenericData = nullptr;

	ScenProto_t(ByteReader* reader);
};

class WallProto_t
{
public:
	uint16_t WallLightFlags = 0;
	uint16_t ActionFlags = 0;
	PID_t* ScriptID = nullptr;
	uint32_t Material = MAT_GLASS;

	WallProto_t(ByteReader* reader);
};

class TileProto_t
{
public:
	uint32_t Material = MAT_GLASS;

	TileProto_t(ByteReader* reader);
};

class FallProto_t
{
public:
	PID_t* PID = nullptr;
	uint32_t TextId = 0;
	PID_t* FID = nullptr;
	uint32_t LightRad = 0;
	uint32_t LightIntence = 0;
	uint32_t Flags = 0;

	ItemProto_t* ItemPro = nullptr;
	CritterProto_t* CritPro = nullptr;
	ScenProto_t* ScenPro = nullptr;
	WallProto_t* WallPro = nullptr;
	TileProto_t* TilePro = nullptr;

	std::string Filename = "test.pro";

	FallProto_t(ByteReader* reader);
};