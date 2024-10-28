#include "FallProto.h"

ItemArmorProto_t::ItemArmorProto_t(ByteReader* reader)
{
	AC = reader->i32();
	for (size_t i = 0; i < 7; i++)
	{
		Resists.push_back(reader->i32());
	}

	for (size_t i = 0; i < 7; i++)
	{
		Thresholds.push_back(reader->i32());
	}

	Perk = reader->i32();
	MaleCRType = new PID_t(reader);
	FemaleCRType = new PID_t(reader);
}

ItemContProto_t::ItemContProto_t(ByteReader* reader)
{
	MaxVolume = reader->u32();
	ContFlags = reader->u32();
}

ItemDrugProto_t::ItemDrugProto_t(ByteReader* reader)
{
	for (size_t i = 0; i < 3; i++)
	{
		Stats.push_back(reader->i32());
	}
	for (size_t i = 0; i < 3; i++)
	{
		FirstMod.push_back(reader->i32());
	}
	Duration1 = reader->u32();
	for (size_t i = 0; i < 3; i++)
	{
		SecondMod.push_back(reader->i32());
	}
	Duration2 = reader->u32();
	for (size_t i = 0; i < 3; i++)
	{
		ThirdMod.push_back(reader->i32());
	}
	AddictRate = reader->u32();
	AddictEffect = reader->u32();
	AddictDelay = reader->u32();
}

ItemWeapProto_t::ItemWeapProto_t(ByteReader* reader)
{
	AnimCode = reader->u32();
	MinDmg = reader->u32();
	MaxDmg = reader->u32();
	DmgType = reader->u32();
	MaxRange1 = reader->u32();
	MaxRange2 = reader->u32();
	ProjPID = new PID_t(reader);
	MinST = reader->u32();
	APCost1 = reader->u32();
	APCost2 = reader->u32();
	CritFail = reader->u32();
	Perk = reader->i32();
	Rounds = reader->u32();
	Caliber = reader->u32();
	AmmoPID = new PID_t(reader);
	MaxAmmo = reader->u32();
	SoundId = reader->u8();
}

ItemAmmoProto_t::ItemAmmoProto_t(ByteReader* reader)
{
	Caliber = reader->u32();
	Quantity = reader->u32();
	ACMod = reader->u32();
	DRMod = reader->u32();
	DmgMult = reader->i32();
	DmgDiv = reader->i32();
}

ItemMiscProto_t::ItemMiscProto_t(ByteReader* reader)
{
	PowerPID = new PID_t(reader);
	PowerType = reader->u32();
	Charges = reader->u32();
}

ItemKeyProto_t::ItemKeyProto_t(ByteReader* reader)
{
	KeyID = reader->u32();
}

ItemProto_t::ItemProto_t(ByteReader* reader)
{
	FlagsExt = reader->u32();
	ScriptId = reader->u32();
	Subtype = reader->u32();
	Material = reader->u32();
	Volume = reader->u32();
	Weight = reader->u32();
	Cost = reader->u32();
	InvFID = new PID_t(reader);
	SoundID = reader->u8();

	if (Subtype == ITEM_TYPE_ARMOR)
	{
		ArmorData = new ItemArmorProto_t(reader);
	}
	else if (Subtype == ITEM_TYPE_CONTAINER)
	{
		ContData = new ItemContProto_t(reader);
	}
	else if (Subtype == ITEM_TYPE_DRUG)
	{
		DrugData = new ItemDrugProto_t(reader);
	}
	else if (Subtype == ITEM_TYPE_WEAPON)
	{
		WeapData = new ItemWeapProto_t(reader);
	}
	else if (Subtype == ITEM_TYPE_AMMO)
	{
		AmmoData = new ItemAmmoProto_t(reader);
	}
	else if (Subtype == ITEM_TYPE_MISC)
	{
		MiscData = new ItemMiscProto_t(reader);
	}
	else if (Subtype == ITEM_TYPE_KEY)
	{
		KeyData = new ItemKeyProto_t(reader);
	}
}

CritterProto_t::CritterProto_t(ByteReader* reader)
{
	ActionFlags = reader->u32();
	ScriptID = new PID_t(reader);
	HeadFID = new PID_t(reader);
	AiID = reader->u32();
	TeamID = reader->u32();
	CritFlags = reader->u32();
	for (size_t i = 0; i < 7; i++)
	{
		SPECIAL.push_back(reader->i32());
	}
	HP = reader->i32();
	AP = reader->i32();
	AC = reader->i32();
	UnarmedDMG = reader->i32();
	MeleeDMG = reader->i32();
	CarryWeight = reader->u32();
	Sequence = reader->u32();
	HealRate = reader->u32();
	CritChance = reader->u32();
	BetterCrits = reader->u32();
	for (size_t i = 0; i < 7; i++)
	{
		Thresholds.push_back(reader->u32());
	}
	for (size_t i = 0; i < 9; i++)
	{
		Resists.push_back(reader->u32());
	}
	Age = reader->u32();
	Sex = reader->u32();
	for (size_t i = 0; i < 7; i++)
	{
		BonusSPECIAL.push_back(reader->i32());
	}
	BonusHP = reader->i32();
	BonusAP = reader->i32();
	BonusAC = reader->i32();
	BonusUnarmedDMG = reader->i32();
	BonusMeleeDMG = reader->i32();
	BonusCarryWeight = reader->u32();
	BonusSequence = reader->u32();
	BonusHealRate = reader->u32();
	BonusCritChance = reader->u32();
	BonusBetterCrits = reader->u32();
	for (size_t i = 0; i < 7; i++)
	{
		BonusThresholds.push_back(reader->u32());
	}
	for (size_t i = 0; i < 9; i++)
	{
		BonusResists.push_back(reader->u32());
	}
	BonusAge = reader->u32();
	BonusSex = reader->u32();
	for (size_t i = 0; i < 18; i++)
	{
		Skills.push_back(reader->i32());
	}
	BodyType = reader->u32();
	ExpVal = reader->u32();
	KillType = reader->u32();
	DmgType = reader->u32();
}

ScenDoorProto_t::ScenDoorProto_t(ByteReader* reader)
{
	WalkThru = reader->u32();
	DoorFlag = reader->u32();
}

ScenStairProto_t::ScenStairProto_t(ByteReader* reader)
{
	DestElev = reader->u8();
	reader->u8();
	DestTile = reader->u16();
	DestMap = reader->u32();
}

ScenElevProto_t::ScenElevProto_t(ByteReader* reader)
{
	ElevType = reader->u32();
	ElevLevel = reader->u32();
}

ScenLadderProto_t::ScenLadderProto_t(ByteReader* reader)
{
	DestElev = reader->u8();
	reader->u8();
	DestTile = reader->u16();
}

ScenGenericProto_t::ScenGenericProto_t(ByteReader* reader)
{
	Unknown = reader->u32();
}

ScenProto_t::ScenProto_t(ByteReader* reader)
{
	WallLightFlags = reader->u16();
	ActionFlags = reader->u16();
	ScriptID = new PID_t(reader);
	SubType = reader->u32();
	Material = reader->u32();
	SoundID = reader->u8();

	if (SubType == SCEN_TYPE_DOOR)
	{
		DoorData = new ScenDoorProto_t(reader);
	}
	else if (SubType == SCEN_TYPE_STAIR)
	{
		StairData = new ScenStairProto_t(reader);
	}
	else if (SubType == SCEN_TYPE_ELEV)
	{
		ElevData = new ScenElevProto_t(reader);
	}
	else if (SubType == SCEN_TYPE_LADDER_BTM || SubType == SCEN_TYPE_LADDER_TOP)
	{
		LadderData = new ScenLadderProto_t(reader);
	}
	else if (SubType == SCEN_TYPE_GENERIC)
	{
		GenericData = new ScenGenericProto_t(reader);
	}
}

WallProto_t::WallProto_t(ByteReader* reader)
{
	WallLightFlags = reader->u16();
	ActionFlags = reader->u16();
	ScriptID = new PID_t(reader);
	Material = reader->u32();
}

TileProto_t::TileProto_t(ByteReader* reader)
{
	Material = reader->u32();
}

FallProto_t::FallProto_t(ByteReader* reader)
{
	PID = new PID_t(reader);
	TextId = reader->u32();
	FID = new PID_t(reader);
	LightRad = reader->u32();
	LightIntence = reader->u32();
	Flags = reader->u32();

	if (PID->PIDType == PROTO_ITEM)
	{
		ItemPro = new ItemProto_t(reader);
	}
	if (PID->PIDType == PROTO_CRITTER)
	{
		CritPro = new CritterProto_t(reader);
	}
	if (PID->PIDType == PROTO_SCENERY)
	{
		ScenPro = new ScenProto_t(reader);
	}
	if (PID->PIDType == PROTO_WALLS)
	{
		WallPro = new WallProto_t(reader);
	}
	if (PID->PIDType == PROTO_TILES)
	{
		TilePro = new TileProto_t(reader);
	}
}