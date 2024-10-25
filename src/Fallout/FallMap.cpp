#include "FallMap.h"
#include "FallProto.h"
#include <stdio.h>

using namespace std;

const string protoTypeNames[6] = { "Items", "Critters", "Scenery", "Walls", "Tiles", "Misc" };

std::vector<std::vector<std::string>> protoLsts;
bool lstsReady = false;
uint32_t mapVer = 0;
std::string gamePath = "Fallout/F2/";

tile_t::tile_t(ByteReader* reader)
{
	tileId = reader->u16();
	floorId = reader->u16();
}

void skip_scripts(ByteReader* reader)
{
	for (size_t script_section = 0; script_section < 5; script_section++) 
	{
		uint32_t script_section_count = reader->u32(); // total number of scripts in section

		printf("... script section has %i scripts\n", script_section_count);

		if (script_section_count > 0) 
		{
			uint32_t loop = script_section_count;
			// find the next multiple of 16 higher than the count
			if (script_section_count % 16 > 0) 
			{
				loop += 16 - script_section_count % 16;
			}

			uint32_t check = 0;
			for (size_t j = 0; j < loop; j++) 
			{
				int32_t pid = reader->i32();
				reader->i32();

				pid = (pid & 0xff000000) >> 24;

				switch (pid)
				{
				case 0:
					break;
				case 1:
					reader->u32();
					reader->u32();
					break;
				case 2:
					reader->u32();
					break;
				case 3:
				case 4:
					break;
				default:
					printf("Unknown script PID = %i\n", pid);
					break;
				}

				reader->u32();
				reader->u32();
				reader->u32();
				reader->u32();
				reader->u32();
				reader->u32();
				reader->u32();
				reader->u32();
				reader->u32();
				reader->u32();
				reader->u32();
				reader->u32();
				reader->u32();
				reader->u32();

				if ((j % 16) == 15) 
				{

					// TODO: write after the batch
					// number of scripts in this batch (sequence)
					uint32_t cur_check = reader->u32();

					check += cur_check;

					reader->u32(); // uknown
				}
			}
		}
	}
}

void SetupProtoLsts()
{
	protoLsts.resize(6);
	for (int i = 0; i < 6; i++)
	{
		std::string currString;
		std::string path = gamePath + "proto/" + protoTypeNames[i] + "/" + protoTypeNames[i] + ".LST";
		std::ifstream input(path);
		if (input.is_open())
		{
			while (getline(input, currString))
			{
				protoLsts[i].push_back(currString);
			}
		}
		input.close();
	}
}

critObject_t::critObject_t(ByteReader* reader)
{
	playerReact = reader->u32();
	currMP = reader->u32();
	combatResult = reader->u32();
	dmgLastTurn = reader->u32();
	aiPack = reader->i32();
	groupId = reader->u32();
	enemy = reader->u32();
	currHP = reader->u32();
	currRAD = reader->u32();
	currPoison = reader->u32();
}

ammoObject_t::ammoObject_t(ByteReader* reader)
{
	currAmmo = reader->u32();
}

keyObject_t::keyObject_t(ByteReader* reader)
{
	keyCode = reader->u32();
}

miscObject_t::miscObject_t(ByteReader* reader)
{
	charges = reader->u32();
}

weapObject_t::weapObject_t(ByteReader* reader)
{
	ammoCount = reader->u32();
	ammoPid = reader->i32();
}

ladderObject_t::ladderObject_t(ByteReader* reader)
{
	destHex = reader->u32();
	if (mapVer == 20)
	{
		destMap = reader->u32();
	}
}

doorObject_t::doorObject_t(ByteReader* reader)
{
	walkThru = reader->u32();
}

stairsObject_t::stairsObject_t(ByteReader* reader)
{
	destHex = reader->u32();
	destMap = reader->u32();
}

elevObject_t::elevObject_t(ByteReader* reader)
{
	elevType = reader->u32();
	elevLevel = reader->u32();
}

gridObject_t::gridObject_t(ByteReader* reader)
{
	toMapId = reader->u32();
	chosPos = reader->u32();
	mapElev = reader->u32();
	chosDir = reader->u32();
}

mapObject_t::mapObject_t(ByteReader* reader)
{
	reader->u32();
	objectPos = reader->i32();
	reader->u32();
	reader->u32();
	reader->u32();
	reader->u32();
	frameNum = reader->u32();
	dir = reader->u32();
	FIDType = reader->u8();
	reader->u8();
	FIDNum = reader->u16();
	flags = reader->u32();
	mapElev = reader->u32();
	PIDType = reader->u8();
	reader->u8();
	PIDNum = reader->u16();
	critIndex = reader->u32();
	lightRadius = reader->u32();
	lightIntense = reader->u32();
	outlineColor = reader->u32();
	reader->u32();
	scriptID = reader->i32();
	invenSize = reader->u32();
	critInvenSlots = reader->u32();
	reader->u32();
	reader->u32();

	if (PIDType == PROTO_CRITTER)
	{
		critData = new critObject_t(reader);
	}

	if (PIDType == PROTO_ITEM)
	{
		FallProto_t* file = nullptr;
		ByteReader* protoReader = new ByteReader;
		std::string path = gamePath + "proto/" + protoTypeNames[PIDType] + "/" + protoLsts[PIDType][PIDNum - 1];
		if (!protoReader->Reset(path, ByteReader::BigEndian)) return;
		file = new FallProto_t(protoReader);
		file->filename = protoLsts[PIDType][PIDNum - 1];
		protoReader->Close();
		delete protoReader;

		if (file->itemPro->ammoData != nullptr)
		{
			ammoData = new ammoObject_t(reader);
		}

		if (file->itemPro->keyData != nullptr)
		{
			keyData = new keyObject_t(reader);
		}

		if (file->itemPro->miscData != nullptr)
		{
			miscData = new miscObject_t(reader);
		}

		if (file->itemPro->weapData != nullptr)
		{
			weapData = new weapObject_t(reader);
		}

		delete file;
	}

	if (PIDType == PROTO_SCENERY)
	{
		FallProto_t* file = nullptr;
		ByteReader* protoReader = new ByteReader;
		std::string path = gamePath + "proto/" + protoTypeNames[PIDType] + "/" + protoLsts[PIDType][PIDNum - 1];
		if (!protoReader->Reset(path, ByteReader::BigEndian)) return;
		file = new FallProto_t(protoReader);
		file->filename = protoLsts[PIDType][PIDNum - 1];
		protoReader->Close();
		delete protoReader;

		if (file->scenPro->ladderData != nullptr)
		{
			ladderData = new ladderObject_t(reader);
		}

		if (file->scenPro->doorData != nullptr)
		{
			doorData = new doorObject_t(reader);
		}

		if (file->scenPro->stairData != nullptr)
		{
			stairsData = new stairsObject_t(reader);
		}

		if (file->scenPro->elevData != nullptr)
		{
			elevData = new elevObject_t(reader);
		}

		delete file;
	}

	if (PIDType == PROTO_MISC)
	{
		if (PIDNum >= 16 && PIDNum <= 23)
		{
			gridData = new gridObject_t(reader);
		}
	}
}

FallMap_t::FallMap_t(ByteReader* reader)
{
	version = reader->u32();
	mapVer = version;
	if (!lstsReady)
	{
		SetupProtoLsts();
		lstsReady = true;
	}
	mapName = reader->string(16);
	defChosPos = reader->i32();
	defChosElv = reader->i32();
	defChosDir = reader->i32();
	lVarsNum = reader->i32();
	scriptId = reader->i32();
	mapFlags = reader->i32();
	mapDark = reader->i32();
	gVarsNum = reader->i32();
	mapId = reader->i32();
	epochTime = reader->u32();

	for (int32_t i = 0; i < lVarsNum; i++)
	{
		lVars.push_back(reader->i32());
	}

	for (int32_t i = 0; i < gVarsNum; i++)
	{
		gVars.push_back(reader->i32());
	}

	for (uint8_t i = 0; i < 44; i++)
	{
		reader->i32();
	}

	int tileLen = 10000;
	int elevation = 1;
	switch (mapFlags & 0xE)
	{
	case 0x0c:
		tileLen = 10000;
		elevation = 1;
		break;
	case 0x08:
		tileLen = 20000;
		elevation = 2;
		break;
	case 0x00:
		tileLen = 30000;
		elevation = 3;
		break;
	}

	for (int i = 0; i < tileLen; i++)
	{
		tile_t currTile(reader);
		tiles.push_back(currTile);
	}

	skip_scripts(reader);

	totalObjects = reader->u32();
	for (size_t elev = 0; elev < elevation; elev++) 
	{
		uint32_t objectsOnElevation = reader->u32();

		for (size_t j = 0; j < objectsOnElevation; j++)
		{
			mapObject_t newObj(reader);
			for (size_t i = 0; i < newObj.invenSize; i++)
			{
				uint32_t amount = reader->u32();
				mapObject_t subObj(reader);
				objects.push_back(subObj);
			}
			objects.push_back(newObj);
		}
	}
}