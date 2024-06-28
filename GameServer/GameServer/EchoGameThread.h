#pragma once
#include "GameThread.h"
#include <map>
#include "Player.h"
#include "GameServer.h"

class EchoGameThread : public GameThread
{
public:
	EchoGameThread(GameServer* gameServer, int threadId);

public:
	int64 GetPlayerSize() override
	{
		return _playerMap.size();
	}

private:
	GameServer* _gameServer;

public:
	// GameThread을(를) 통해 상속됨
	void HandleRecvPacket(int64 sessionId, CPacket* packet) override;
	void OnLeaveThread(int64 sessionId, bool disconnect) override;
	void OnEnterThread(int64 sessionId, void* ptr) override;

private:
	std::unordered_map<int64, Player*> _playerMap;

private:
	void HandleCharacterMove(Player* p, CPacket* packet);
private:
	uint16 serverPacketCode = Data::serverPacketCode;
	void MP_SC_ECHO(CPacket* packet, CPacket* echoPacket);
	void MP_SC_FIELD_MOVE(CPacket* packet, uint8& status);

	void MP_SC_LOGIN(CPacket* packet, uint8& status, int64& accountNo);
	void MP_SC_SPAWN_MY_CHARACTER(CPacket* packet, SpawnMyCharacterInfo& spawnMyCharacterInfo);
	void MP_SC_SPAWN_OTHER_CHARACTER(CPacket* packet, SpawnOtherCharacterInfo& spawnOtherCharacterInfo);
	void MP_SC_GAME_RES_CHARACTER_MOVE(CPacket* packet, int64& charaterNo, FVector& Destination);



};

