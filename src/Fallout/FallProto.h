#pragma once

#include "../Utils/ByteReader.hpp"
#include "../Utils/BaseDataTypes.h"
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

class FID_t
{
public:
	uint8_t FIDType = 0;
	uint16_t FIDNum = 0;

	FID_t(ByteReader* reader);
};

class ItemArmorProto_t
{
public:
	int32_t AC = 0;
	std::vector<int32_t> resists;
	std::vector<int32_t> thresholds;
	int32_t perk = -1;
	FID_t* maleCRType = nullptr;
	FID_t* femaleCRType = nullptr;

	ItemArmorProto_t(ByteReader* reader);
};

class ItemContProto_t
{
public:
	uint32_t maxVolume = 0;
	uint32_t contFlags = 0;

	ItemContProto_t(ByteReader* reader);
};

class ItemDrugProto_t
{
public:
	std::vector<int32_t> stats;
	std::vector<int32_t> firstMod;
	uint32_t duration1 = 0;
	std::vector<int32_t> secondMod;
	uint32_t duration2 = 0;
	std::vector<int32_t> thirdMod;
	uint32_t addictionRate = 0;
	uint32_t addictionEffect = 0;
	uint32_t addictionDelay = 0;

	ItemDrugProto_t(ByteReader* reader);
};

class ItemWeapProto_t
{
public:
	uint32_t animCode = 0;
	uint32_t minDmg = 0;
	uint32_t maxDmg = 0;
	uint32_t dmgType = 0;
	uint32_t maxRange1 = 0;
	uint32_t maxRange2 = 0;
	FID_t* projPID = nullptr;
	uint32_t minST = 0;
	uint32_t apCost1 = 0;
	uint32_t apCost2 = 0;
	uint32_t critFail = 0;
	int32_t perk = -1;
	uint32_t rounds = 0;
	uint32_t caliber = 0;
	FID_t* ammoPID = nullptr;
	uint32_t maxAmmo = 0;
	uint8_t soundId = 0;

	ItemWeapProto_t(ByteReader* reader);
};

class ItemAmmoProto_t
{
public:
	uint32_t caliber = 0;
	uint32_t quantity = 0;
	uint32_t ACMod = 0;
	uint32_t DRMod = 0;
	int32_t dmgMult = 0;
	int32_t dmgDiv = 0;

	ItemAmmoProto_t(ByteReader* reader);
};

class ItemMiscProto_t
{
public:
	FID_t* powerPID = nullptr;
	uint32_t powerType = 0;
	uint32_t charges = 0;

	ItemMiscProto_t(ByteReader* reader);
};

class ItemKeyProto_t
{
public:
	uint32_t keyID = 0;

	ItemKeyProto_t(ByteReader* reader);
};

class ItemProto_t
{
public:
	uint32_t flagsExt = 0;
	uint32_t scriptId = 0;
	uint32_t subtype = 0;
	uint32_t material = MAT_GLASS;
	uint32_t volume = 0;
	uint32_t weight = 0;
	uint32_t cost = 0;
	FID_t* invFID = nullptr;
	uint8_t soundID = 0;

	ItemArmorProto_t* armorData = nullptr;
	ItemContProto_t* contData = nullptr;
	ItemDrugProto_t* drugData = nullptr;
	ItemWeapProto_t* weapData = nullptr;
	ItemAmmoProto_t* ammoData = nullptr;
	ItemMiscProto_t* miscData = nullptr;
	ItemKeyProto_t* keyData = nullptr;

	ItemProto_t(ByteReader* reader);
};

class CritterProto_t
{
public:
	uint32_t actionFlags = 0;
	uint8_t scriptType = 0;
	uint16_t scriptNum = 0;
	FID_t* headFID = nullptr;
	uint32_t aiID = 0;
	uint32_t teamID = 0;
	uint32_t critFlags = 0;
	std::vector<int32_t> SPECIAL;
	int32_t HP = 0;
	int32_t AP = 0;
	int32_t AC = 0;
	int32_t unarmedDMG = 0;
	int32_t meleeDMG = 0;
	uint32_t carryWeight = 0;
	uint32_t sequence = 0;
	uint32_t healRate = 0;
	uint32_t critChance = 0;
	uint32_t betterCrits = 0;
	std::vector<uint32_t> thresholds;
	std::vector<uint32_t> resists;
	uint32_t age = 1;
	uint32_t sex = 0;
	std::vector<int32_t> bonusSPECIAL;
	int32_t bonusHP = 0;
	int32_t bonusAP = 0;
	int32_t bonusAC = 0;
	int32_t bonusUnarmedDMG = 0;
	int32_t bonusMeleeDMG = 0;
	uint32_t bonusCarryWeight = 0;
	uint32_t bonusSequence = 0;
	uint32_t bonusHealRate = 0;
	uint32_t bonusCritChance = 0;
	uint32_t bonusBetterCrits = 0;
	std::vector<uint32_t> bonusThresholds;
	std::vector<uint32_t> bonusResists;
	uint32_t bonusAge = 1;
	uint32_t bonusSex = 0;
	std::vector<int32_t> skills;
	uint32_t bodyType = 0;
	uint32_t expVal = 0;
	uint32_t killType = 0;
	uint32_t dmgType = 0;

	CritterProto_t(ByteReader* reader);
};

class SceneryDoorProto_t
{
public:
	uint32_t walkThruFlag = 0;
	uint32_t doorFlag = 0xCCCCCCCC;

	SceneryDoorProto_t(ByteReader* reader);
};

class SceneryStairProto_t
{
public:
	uint8_t destElev = 0;
	uint16_t destTile = 0;
	uint32_t destMap = 0;

	SceneryStairProto_t(ByteReader* reader);
};

class SceneryElevProto_t
{
public:
	uint32_t elevType = 0;
	uint32_t elevLevel = 0;

	SceneryElevProto_t(ByteReader* reader);
};

class SceneryLadderProto_t
{
public:
	uint8_t destElev = 0;
	uint16_t destTile = 0;

	SceneryLadderProto_t(ByteReader* reader);
};

class SceneryGenericProto_t
{
public:
	uint32_t unknown = 0;

	SceneryGenericProto_t(ByteReader* reader);
};

class SceneryProto_t
{
public:
	uint16_t wallLightFlags = 0;
	uint16_t actionFlags = 0;
	uint8_t scriptType = 0;
	uint16_t scriptNum = 0;
	uint32_t subType = 0;
	uint32_t material = MAT_GLASS;
	uint8_t soundID = 0;

	SceneryDoorProto_t* doorData = nullptr;
	SceneryStairProto_t* stairData = nullptr;
	SceneryElevProto_t* elevData = nullptr;
	SceneryLadderProto_t* ladderData = nullptr;
	SceneryGenericProto_t* genericData = nullptr;

	SceneryProto_t(ByteReader* reader);
};

class WallProto_t
{
public:
	uint16_t wallLightFlags = 0;
	uint16_t actionFlags = 0;
	uint8_t scriptType = 0;
	uint16_t scriptNum = 0;
	uint32_t material = MAT_GLASS;

	WallProto_t(ByteReader* reader);
};

class TileProto_t
{
public:
	uint32_t material = MAT_GLASS;

	TileProto_t(ByteReader* reader);
};

class FallProto_t
{
public:
	uint8_t objType = PROTO_ITEM;
	uint16_t pid = 0;
	uint32_t textId = 0;
	FID_t* FID = nullptr;
	uint32_t lightRad = 0;
	uint32_t lightIntence = 0;
	uint32_t flags = 0;

	ItemProto_t* itemPro = nullptr;
	CritterProto_t* critPro = nullptr;
	SceneryProto_t* scenPro = nullptr;
	WallProto_t* wallPro = nullptr;
	TileProto_t* tilePro = nullptr;

	std::string filename = "test.pro";

	FallProto_t(ByteReader* reader);
};