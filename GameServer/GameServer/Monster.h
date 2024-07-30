#pragma once
#include "Type.h"
#include <unordered_map>
#include "FieldObject.h"

class CPacket;
class Player;
class BasePacketHandleThread;

enum class MonsterState {
	MS_IDLE,
	MS_MOVING,
	MS_ATTACKING,
	MS_CHASING,
	MS_DEATH,
};


// 이거 몬스터 override를 해서 비선공형 선공형을 나눠야할까?
// 아니면 MonsterType을 만들어서 비선공형 선공형을 나눠야할까?
class Monster : public FieldObject
{
	friend class GuardianFieldThread;
	friend class LobbyFieldThread;
	friend class SpiderFieldThread;
	friend class FieldPacketHandleThread;

public:
	Monster(FieldPacketHandleThread* field, uint16 objectType, uint16 monsterType);
	Monster(FieldPacketHandleThread* field, uint16 objectType, uint16 monsterType, FVector spawnPosition);
	MonsterState GetState() { return _state; };
	void Update(float deltaTime);
	void SetDestination(FVector destination);
	int64 GetObjectId() { return _objectId; };

	/// <summary>
	/// 
	/// </summary>
	/// <param name="damage"></param>
	/// <param name="attacker"></param>
	/// <returns>return true when death</returns>
	bool TakeDamage(int damage, Player* attacker);
	FVector GetPosition() { return _position; };
	FRotator GetRotation() { return _rotation; };
	void MoveToDestination(float deltaTime);
	void AttackPlayer(float deltaTime);
	void ChasePlayer(float deltaTime);
	void SetRandomDestination();
	float GetDistanceToPlayer(Player* targetPlayer);
	FRotator CalculateRotation(const FVector& oldPosition, const FVector& newPosition);
	void OnSpawn();
//private:
//	void SendIdlePacket();
//	void SendAttackPacket();
//	void SendDestinationPacket();
	
	//Player가 나가거나 죽을떄 해당 player를 target으로 하고있던애를 idle 상태로 만들어야함
	void SetTargetPlayerEmpty();

private:
	MonsterInfo _monsterInfo;
	FVector _position; // 현재 위치
	FRotator _rotation{ 0,0,0 }; // 현재 방향

	FVector _destination; // 목적지
	float _speed; // 이동속도
	MonsterState _state;
	
	Player* _targetPlayer; // 공격중인 플레이어
	float _attackRange; // 공격 범위
	float _attackCooldown; // 공격 쿨타임
	float _attackTimer = 0; // 공격 후 타이머 리셋하기
	float _aggroRange; // 어그로 범위
	float _idleTime;// 일정시간 동안 대기할 시간
	float _chaseTime;// 플레이어를 추적하는 시간
	float _maxChaseTime; // 최대 추적시간

	//
	int _damage = 5;
	float _defaultIdleTime = 10;
};

