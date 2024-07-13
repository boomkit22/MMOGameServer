#include "PacketHeader.h"
#include "Packet.h"
#include "SerializeBuffer.h"
#include "Type.h"
#include "GamePacketMaker.h"
#include "Data.h"


void MP_SC_FIELD_MOVE(CPacket* packet, uint8& status)
{
	NetHeader header;
	header._code = Data::serverPacketCode;
	header._randKey = rand();
	packet->PutData((char*)&header, sizeof(NetHeader));

	uint16 type = PACKET_SC_GAME_RES_FIELD_MOVE;
	*packet << type << status;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}


void MP_SC_SPAWN_MY_CHARACTER(CPacket* packet, PlayerInfo playerInfo, FVector spawnLocation)
{
	NetHeader header;
	header._code = Data::serverPacketCode;
	header._randKey = rand();
	packet->PutData((char*)&header, sizeof(NetHeader));

	uint16 type = PACKET_SC_GAME_SPAWN_MY_CHARACTER;
	*packet << type << playerInfo << spawnLocation;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void MP_SC_SPAWN_OTHER_CHARACTER(CPacket* packet, PlayerInfo playerInfo, FVector spawnLocation)
{
	NetHeader header;
	header._code = Data::serverPacketCode;
	header._randKey = rand();
	packet->PutData((char*)&header, sizeof(NetHeader));

	uint16 type = PACKET_SC_GAME_SPAWN_OTHER_CHAACTER;
	*packet << type << playerInfo << spawnLocation;


	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void MP_SC_GAME_RES_CHARACTER_MOVE(CPacket* packet, int64& charaterNo, FVector& Destination, FRotator& StartRotation)
{
	NetHeader header;
	header._code = Data::serverPacketCode;
	header._randKey = rand();
	packet->PutData((char*)&header, sizeof(NetHeader));

	uint16 type = PACKET_SC_GAME_RES_CHARACTER_MOVE;
	*packet << type << charaterNo << Destination << StartRotation;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}



void MP_SC_GAME_RES_DAMAGE(CPacket* packet, int32& AttackerType, int64& AttackerID, int32& targetType, int64& TargetID, int32& Damage)
{
	NetHeader header;
	header._code = Data::serverPacketCode;
	header._randKey = rand();
	packet->PutData((char*)&header, sizeof(NetHeader));

	uint16 type = PACKET_SC_GAME_RES_DAMAGE;
	*packet << type << AttackerType << AttackerID << targetType << TargetID << Damage;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void MP_SC_GAME_RES_CHARACTER_SKILL(CPacket* packet, int64& CharacterID, FVector& StartLocation, FRotator& StartRotation, int32& SkillID)
{
	NetHeader header;
	header._code = Data::serverPacketCode;
	header._randKey = rand();
	packet->PutData((char*)&header, sizeof(NetHeader));

	uint16 type = PACKET_SC_GAME_RES_CHARACTER_SKILL;
	*packet << type << CharacterID << StartLocation << StartRotation << SkillID;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void MP_SC_GAME_RES_MONSTER_SKILL(CPacket* packet, int64& MonsterNO, int32& SkillID)
{
	NetHeader header;
	header._code = Data::serverPacketCode;
	header._randKey = rand();
	packet->PutData((char*)&header, sizeof(NetHeader));

	uint16 type = PACKET_SC_GAME_RES_MONSTER_SKILL;
	*packet << type << MonsterNO << SkillID;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void MP_SC_SPAWN_MONSTER(CPacket* packet, MonsterInfo monsterInfo, FVector spawnLocation)
{
	NetHeader header;
	header._code = Data::serverPacketCode;
	header._randKey = rand();
	packet->PutData((char*)&header, sizeof(NetHeader));

	uint16 type = PACKET_SC_GAME_SPAWN_MONSTER;
	*packet << type << monsterInfo << spawnLocation;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void MP_SC_MONSTER_MOVE(CPacket* packet, int64& monsterId, FVector& Destination, FRotator& StartRotation)
{
	NetHeader header;
	header._code = Data::serverPacketCode;
	header._randKey = rand();
	packet->PutData((char*)&header, sizeof(NetHeader));

	uint16 type = PACKET_SC_GAME_MONSTER_MOVE;
	*packet << type << monsterId << Destination << StartRotation;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void MP_SC_GAME_RSE_CHARACTER_STOP(CPacket* packet, int64& characterID, FVector& position, FRotator& rotation)
{
	NetHeader header;
	header._code = Data::serverPacketCode;
	header._randKey = rand();
	packet->PutData((char*)&header, sizeof(NetHeader));

	uint16 type = PACKET_SC_GAME_RES_CHARACTER_STOP;
	*packet << type << characterID << position << rotation;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void MP_SC_GAME_RES_MONSTER_STOP(CPacket* packet, int64& monsterID, FVector& position, FRotator& rotation)
{
	NetHeader header;
	header._code = Data::serverPacketCode;
	header._randKey = rand();
	packet->PutData((char*)&header, sizeof(NetHeader));

	uint16 type = PACKET_SC_GAME_RES_MONSTER_STOP;
	*packet << type << monsterID << position << rotation;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void MP_SC_GAME_RES_CHARACTER_DEATH(CPacket* packet, int64& characterID, FVector DeathLocation, FRotator DeathRotation)
{
	NetHeader header;
	header._code = Data::serverPacketCode;
	header._randKey = rand();
	packet->PutData((char*)&header, sizeof(NetHeader));

	uint16 type = PACKET_SC_GAME_RES_CHARACTER_DEATH;
	*packet << type << characterID << DeathLocation << DeathRotation;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void MP_SC_GAME_RES_MONSTER_DEATH(CPacket* packet, int64& monsterID, FVector DeathLocation, FRotator DeathRotation)
{
	NetHeader header;
	header._code = Data::serverPacketCode;
	header._randKey = rand();
	packet->PutData((char*)&header, sizeof(NetHeader));

	uint16 type = PACKET_SC_GAME_RES_MONSTER_DEATH;
	*packet << type << monsterID << DeathLocation << DeathRotation;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}




