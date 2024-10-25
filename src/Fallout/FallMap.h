#pragma once

#include "../Utils/ByteReader.hpp"
#include "../Utils/BaseDataTypes.h"
#include <vector>
#include <string>

extern std::string gamePath;

class tile_t
{
public:
	uint16_t tileId = 1;
	uint16_t floorId = 1;

	tile_t(ByteReader* reader);
};

class critObject_t
{
public:
	uint32_t playerReact = 0;
	uint32_t currMP = 0;
	uint32_t combatResult = 0;
	uint32_t dmgLastTurn = 0;
	int32_t aiPack = 0;
	uint32_t groupId = 0;
	uint32_t enemy = 0;
	uint32_t currHP = 0;
	uint32_t currRAD = 0;
	uint32_t currPoison = 0;
	
	critObject_t(ByteReader* reader);
};

class ammoObject_t
{
public:
	uint32_t currAmmo = 0;

	ammoObject_t(ByteReader* reader);
};

class keyObject_t
{
public:
	uint32_t keyCode = 0;

	keyObject_t(ByteReader* reader);
};

class miscObject_t
{
public:
	uint32_t charges = 0;

	miscObject_t(ByteReader* reader);
};

class weapObject_t
{
public:
	uint32_t ammoCount = 0;
	int32_t ammoPid = 0;

	weapObject_t(ByteReader* reader);
};

class ladderObject_t
{
public:
	uint32_t destHex = 0;
	uint32_t destMap = 0;

	ladderObject_t(ByteReader* reader);
};

class stairsObject_t
{
public:
	uint32_t destHex = 0;
	uint32_t destMap = 0;

	stairsObject_t(ByteReader* reader);
};

class doorObject_t
{
public:
	uint32_t walkThru = 0;

	doorObject_t(ByteReader* reader);
};

class elevObject_t
{
public:
	uint32_t elevType = 0;
	uint32_t elevLevel = 0;

	elevObject_t(ByteReader* reader);
};

class gridObject_t
{
public:
	uint32_t toMapId = 0;
	uint32_t chosPos = 0;
	uint32_t mapElev = 0;
	uint32_t chosDir = 0;

	gridObject_t(ByteReader* reader);
};

class mapObject_t
{
public:
	int32_t objectPos = 0;
	uint32_t frameNum = 0;
	uint32_t dir = 0;
	uint8_t FIDType = 0;
	uint16_t FIDNum = 0;
	uint32_t flags = 0;
	uint32_t mapElev = 0;
	uint8_t PIDType = 0;
	uint16_t PIDNum = 0;
	int32_t critIndex = -1;
	uint32_t lightRadius = 0;
	uint32_t lightIntense = 0;
	uint32_t outlineColor = 0;
	int32_t scriptID = 0;
	uint32_t invenSize = 0;
	uint32_t critInvenSlots = 0;

	critObject_t* critData = nullptr;
	ammoObject_t* ammoData = nullptr;
	keyObject_t* keyData = nullptr;
	miscObject_t* miscData = nullptr;
	weapObject_t* weapData = nullptr;
	ladderObject_t* ladderData = nullptr;
	doorObject_t* doorData = nullptr;
	stairsObject_t* stairsData = nullptr;
	elevObject_t* elevData = nullptr;
	gridObject_t* gridData = nullptr;

	mapObject_t(ByteReader* reader);
};

class FallMap_t
{
public:
	uint32_t version = 19;
	std::string mapName;
	int32_t defChosPos = 0;
	int32_t defChosElv = 0;
	int32_t defChosDir = 0;
	int32_t lVarsNum = 0;
	int32_t scriptId = 0;
	int32_t mapFlags = 0;
	int32_t mapDark = 0;
	int32_t gVarsNum = 0;
	int32_t mapId = 0;
	uint32_t epochTime = 0;
	uint32_t totalObjects = 0;
	
	std::vector<int32_t> lVars;
	std::vector<int32_t> gVars;

	std::vector<tile_t> tiles;
	std::vector<mapObject_t> objects;

	std::string filename = "test.map";

	FallMap_t(ByteReader* reader);
};