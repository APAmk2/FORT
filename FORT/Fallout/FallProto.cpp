#include "FallProto.h"
#include <stdio.h>

using namespace std;

FID_t::FID_t(ByteReader* reader)
{
	FIDType = reader->u8();
	reader->u8();
	FIDNum = reader->u16();
}

ItemArmorProto_t::ItemArmorProto_t(ByteReader* reader)
{
	AC = reader->i32();
	for (size_t i = 0; i < 7; i++)
	{
		resists.push_back(reader->i32());
	}

	for (size_t i = 0; i < 7; i++)
	{
		thresholds.push_back(reader->i32());
	}

	perk = reader->i32();
	maleCRType = new FID_t(reader);
	femaleCRType = new FID_t(reader);
}

ItemContProto_t::ItemContProto_t(ByteReader* reader)
{
	maxVolume = reader->u32();
	contFlags = reader->u32();
}

ItemDrugProto_t::ItemDrugProto_t(ByteReader* reader)
{
	for (size_t i = 0; i < 3; i++)
	{
		stats.push_back(reader->i32());
	}
	for (size_t i = 0; i < 3; i++)
	{
		firstMod.push_back(reader->i32());
	}
	duration1 = reader->u32();
	for (size_t i = 0; i < 3; i++)
	{
		secondMod.push_back(reader->i32());
	}
	duration2 = reader->u32();
	for (size_t i = 0; i < 3; i++)
	{
		thirdMod.push_back(reader->i32());
	}
	addictionRate = reader->u32();
	addictionEffect = reader->u32();
	addictionDelay = reader->u32();
}

ItemWeapProto_t::ItemWeapProto_t(ByteReader* reader)
{
	animCode = reader->u32();
	minDmg = reader->u32();
	maxDmg = reader->u32();
	dmgType = reader->u32();
	maxRange1 = reader->u32();
	maxRange2 = reader->u32();
	projPID = new FID_t(reader);
	minST = reader->u32();
	apCost1 = reader->u32();
	apCost2 = reader->u32();
	critFail = reader->u32();
	perk = reader->i32();
	rounds = reader->u32();
	caliber = reader->u32();
	ammoPID = new FID_t(reader);
	maxAmmo = reader->u32();
	soundId = reader->u8();
}

ItemAmmoProto_t::ItemAmmoProto_t(ByteReader* reader)
{
	caliber = reader->u32();
	quantity = reader->u32();
	ACMod = reader->u32();
	DRMod = reader->u32();
	dmgMult = reader->i32();
	dmgDiv = reader->i32();
}

ItemMiscProto_t::ItemMiscProto_t(ByteReader* reader)
{
	powerPID = new FID_t(reader);
	powerType = reader->u32();
	charges = reader->u32();
}

ItemKeyProto_t::ItemKeyProto_t(ByteReader* reader)
{
	keyID = reader->u32();
}

ItemProto_t::ItemProto_t(ByteReader* reader)
{
	flagsExt = reader->u32();
	scriptId = reader->u32();
	subtype = reader->u32();
	material = reader->u32();
	volume = reader->u32();
	weight = reader->u32();
	cost = reader->u32();
	invFID = new FID_t(reader);
	soundID = reader->u8();

	if (subtype == ITEM_TYPE_ARMOR)
	{
		armorData = new ItemArmorProto_t(reader);
	}
	else if (subtype == ITEM_TYPE_CONTAINER)
	{
		contData = new ItemContProto_t(reader);
	}
	else if (subtype == ITEM_TYPE_DRUG)
	{
		drugData = new ItemDrugProto_t(reader);
	}
	else if (subtype == ITEM_TYPE_WEAPON)
	{
		weapData = new ItemWeapProto_t(reader);
	}
	else if (subtype == ITEM_TYPE_AMMO)
	{
		ammoData = new ItemAmmoProto_t(reader);
	}
	else if (subtype == ITEM_TYPE_MISC)
	{
		miscData = new ItemMiscProto_t(reader);
	}
	else if (subtype == ITEM_TYPE_KEY)
	{
		keyData = new ItemKeyProto_t(reader);
	}
}

CritterProto_t::CritterProto_t(ByteReader* reader)
{
	actionFlags = reader->u32();
	scriptType = reader->u8();
	reader->u8();
	scriptNum = reader->u16();
	headFID = new FID_t(reader);
	aiID = reader->u32();
	teamID = reader->u32();
	critFlags = reader->u32();
	for (size_t i = 0; i < 7; i++)
	{
		SPECIAL.push_back(reader->i32());
	}
	HP = reader->i32();
	AP = reader->i32();
	AC = reader->i32();
	unarmedDMG = reader->i32();
	meleeDMG = reader->i32();
	carryWeight = reader->u32();
	sequence = reader->u32();
	healRate = reader->u32();
	critChance = reader->u32();
	betterCrits = reader->u32();
	for (size_t i = 0; i < 7; i++)
	{
		thresholds.push_back(reader->u32());
	}
	for (size_t i = 0; i < 9; i++)
	{
		resists.push_back(reader->u32());
	}
	age = reader->u32();
	sex = reader->u32();
	for (size_t i = 0; i < 7; i++)
	{
		bonusSPECIAL.push_back(reader->i32());
	}
	bonusHP = reader->i32();
	bonusAP = reader->i32();
	bonusAC = reader->i32();
	bonusUnarmedDMG = reader->i32();
	bonusMeleeDMG = reader->i32();
	bonusCarryWeight = reader->u32();
	bonusSequence = reader->u32();
	bonusHealRate = reader->u32();
	bonusCritChance = reader->u32();
	bonusBetterCrits = reader->u32();
	for (size_t i = 0; i < 7; i++)
	{
		bonusThresholds.push_back(reader->u32());
	}
	for (size_t i = 0; i < 9; i++)
	{
		bonusResists.push_back(reader->u32());
	}
	bonusAge = reader->u32();
	bonusSex = reader->u32();
	for (size_t i = 0; i < 18; i++)
	{
		skills.push_back(reader->i32());
	}
	bodyType = reader->u32();
	expVal = reader->u32();
	killType = reader->u32();
	dmgType = reader->u32();
}

SceneryDoorProto_t::SceneryDoorProto_t(ByteReader* reader)
{
	walkThruFlag = reader->u32();
	doorFlag = reader->u32();
}

SceneryStairProto_t::SceneryStairProto_t(ByteReader* reader)
{
	destElev = reader->u8();
	reader->u8();
	destTile = reader->u16();
	destMap = reader->u32();
}

SceneryElevProto_t::SceneryElevProto_t(ByteReader* reader)
{
	elevType = reader->u32();
	elevLevel = reader->u32();
}

SceneryLadderProto_t::SceneryLadderProto_t(ByteReader* reader)
{
	destElev = reader->u8();
	reader->u8();
	destTile = reader->u16();
}

SceneryGenericProto_t::SceneryGenericProto_t(ByteReader* reader)
{
	unknown = reader->u32();
}

SceneryProto_t::SceneryProto_t(ByteReader* reader)
{
	wallLightFlags = reader->u16();
	actionFlags = reader->u16();
	scriptType = reader->u8();
	reader->u8();
	scriptNum = reader->u16();
	subType = reader->u32();
	material = reader->u32();
	soundID = reader->u8();

	if (subType == SCEN_TYPE_DOOR)
	{
		doorData = new SceneryDoorProto_t(reader);
	}
	else if (subType == SCEN_TYPE_STAIR)
	{
		stairData = new SceneryStairProto_t(reader);
	}
	else if (subType == SCEN_TYPE_ELEV)
	{
		elevData = new SceneryElevProto_t(reader);
	}
	else if (subType == SCEN_TYPE_LADDER_BTM || subType == SCEN_TYPE_LADDER_TOP)
	{
		ladderData = new SceneryLadderProto_t(reader);
	}
	else if (subType == SCEN_TYPE_GENERIC)
	{
		genericData = new SceneryGenericProto_t(reader);
	}
}

WallProto_t::WallProto_t(ByteReader* reader)
{
	wallLightFlags = reader->u16();
	actionFlags = reader->u16();
	scriptType = reader->u8();
	reader->u8();
	scriptNum = reader->u16();
	material = reader->u32();
}

TileProto_t::TileProto_t(ByteReader* reader)
{
	material = reader->u32();
}

FallProto_t::FallProto_t(ByteReader* reader)
{
	objType = reader->u8();
	reader->u8();
	pid = reader->u16();
	textId = reader->u32();
	FID = new FID_t(reader);
	lightRad = reader->u32();
	lightIntence = reader->u32();
	flags = reader->u32();

	if (objType == PROTO_ITEM)
	{
		itemPro = new ItemProto_t(reader);
	}
	if (objType == PROTO_CRITTER)
	{
		critPro = new CritterProto_t(reader);
	}
	if (objType == PROTO_SCENERY)
	{
		scenPro = new SceneryProto_t(reader);
	}
	if (objType == PROTO_WALLS)
	{
		wallPro = new WallProto_t(reader);
	}
	if (objType == PROTO_TILES)
	{
		tilePro = new TileProto_t(reader);
	}
}