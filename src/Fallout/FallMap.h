#pragma once

#include "../Utils/ByteReader.hpp"
#include "../Utils/BaseDataTypes.h"
#include <vector>
#include <string>

extern std::string GamePath;

class FallMapTile_t
{
public:
	uint16_t RoofId = 1;
	uint16_t TileId = 1;

	FallMapTile_t(ByteReader* reader);
};

class FallMapCritObject_t
{
public:
	uint32_t PlayerReact = 0;
	uint32_t CurrMP = 0;
	uint32_t CombatResult = 0;
	uint32_t DmgLastTurn = 0;
	int32_t AiPack = 0;
	uint32_t GroupId = 0;
	uint32_t Enemy = 0;
	uint32_t CurrHP = 0;
	uint32_t CurrRAD = 0;
	uint32_t CurrPoison = 0;
	
	FallMapCritObject_t(ByteReader* reader);
};

class FallMapAmmoObject_t
{
public:
	uint32_t CurrAmmo = 0;

	FallMapAmmoObject_t(ByteReader* reader);
};

class FallMapKeyObject_t
{
public:
	uint32_t KeyCode = 0;

	FallMapKeyObject_t(ByteReader* reader);
};

class FallMapMiscObject_t
{
public:
	uint32_t Charges = 0;

	FallMapMiscObject_t(ByteReader* reader);
};

class FallMapWeapObject_t
{
public:
	uint32_t AmmoCount = 0;
	int32_t AmmoPid = 0;

	FallMapWeapObject_t(ByteReader* reader);
};

class FallMapLadderObject_t
{
public:
	uint32_t DestHex = 0;
	uint32_t DestMap = 0;

	FallMapLadderObject_t(ByteReader* reader);
};

class FallMapStairsObject_t
{
public:
	uint32_t DestHex = 0;
	uint32_t DestMap = 0;

	FallMapStairsObject_t(ByteReader* reader);
};

class FallMapDoorObject_t
{
public:
	uint32_t WalkThru = 0;

	FallMapDoorObject_t(ByteReader* reader);
};

class FallMapElevObject_t
{
public:
	uint32_t ElevType = 0;
	uint32_t ElevLevel = 0;

	FallMapElevObject_t(ByteReader* reader);
};

class FallMapGridObject_t
{
public:
	uint32_t ToMapId = 0;
	uint32_t ChosPos = 0;
	uint32_t MapElev = 0;
	uint32_t ChosDir = 0;

	FallMapGridObject_t(ByteReader* reader);
};

class FallMapObject_t
{
public:
	int32_t ObjectPos = 0;
	uint32_t FrameNum = 0;
	uint32_t Dir = 0;
	uint8_t FIDType = 0;
	uint16_t FIDNum = 0;
	uint32_t Flags = 0;
	uint32_t MapElev = 0;
	uint8_t PIDType = 0;
	uint16_t PIDNum = 0;
	int32_t CritIndex = -1;
	uint32_t LightRadius = 0;
	uint32_t LightIntense = 0;
	uint32_t OutlineColor = 0;
	int32_t ScriptID = 0;
	uint32_t InvenSize = 0;
	uint32_t CritInvenSlots = 0;

	FallMapCritObject_t* CritData = nullptr;
	FallMapAmmoObject_t* AmmoData = nullptr;
	FallMapKeyObject_t* KeyData = nullptr;
	FallMapMiscObject_t* MiscData = nullptr;
	FallMapWeapObject_t* WeapData = nullptr;
	FallMapLadderObject_t* LadderData = nullptr;
	FallMapDoorObject_t* DoorData = nullptr;
	FallMapStairsObject_t* StairsData = nullptr;
	FallMapElevObject_t* ElevData = nullptr;
	FallMapGridObject_t* GridData = nullptr;

	FallMapObject_t(ByteReader* reader);
};

class FallMap_t
{
public:
	uint32_t Version = 19;
	std::string MapName;
	int32_t DefChosPos = 0;
	int32_t DefChosElv = 0;
	int32_t DefChosDir = 0;
	int32_t LVarsNum = 0;
	int32_t ScriptId = 0;
	int32_t MapFlags = 0;
	int32_t MapDark = 0;
	int32_t GVarsNum = 0;
	int32_t MapId = 0;
	uint32_t EpochTime = 0;
	uint32_t TotalObjects = 0;
	
	std::vector<int32_t> LVars;
	std::vector<int32_t> GVars;

	std::vector<FallMapTile_t> Tiles;
	std::vector<FallMapObject_t> Objects;

	std::string Filename = "test.map";

	FallMap_t(ByteReader* reader);
};