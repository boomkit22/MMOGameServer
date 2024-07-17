#include "Monster.h"
#include <cmath>
#include "Player.h"
#include "GameData.h"
#include <algorithm>
#include "PacketMaker.h"
#include "SerializeBuffer.h"
#include "BasePAcketHandleThread.h"

Monster::Monster()
{

}


void Monster::Init(FVector position, uint16 type)
{
	// monsterId : 1씩 증가
	static int64 monsterIdGenerator = 0;
	_monsterInfo.MonsterID = ++monsterIdGenerator;
	_monsterInfo.Type = type;
	_monsterInfo.Hp= 100;
	_position = position;
	_state = MonsterState::MS_IDLE;
	_speed = 200.0f;
	_destination.Z = 88.1;
	_idleTime = _defaultIdleTime;
	_attackRange = 200.0f;
	_attackCooldown = 5.0f;
	_chaseTime = 0;
	_maxChaseTime = 5.0f;
	_aggroRange = 1000.0f;
	_damage = 5;
	_rotation = { 0,0,0 };
	_targetPlayer = nullptr;
}

std::vector<CPacket*>&  Monster::Update(float deltaTime)
{
	_sendPacketsPerThread.clear();

	//여기서 하는게 좋은 설계인가
	// 공격 쿨타임 계산
	_attackTimer += deltaTime;

	switch(_state)
	{
		case MonsterState::MS_IDLE:
		{
			// idle 상태였으면
			_idleTime -= deltaTime;
			if(_idleTime <= 0)
			{
				// idle 시간이 다 되었으면
				SetRandomDestination();
				_state = MonsterState::MS_MOVING;
			}
		}
		break;

		case MonsterState::MS_MOVING:
		{
			MoveToDestination(deltaTime);
		}
		break;

		case MonsterState::MS_ATTACKING:
		{
			AttackPlayer(deltaTime);
		}
		break;

		case MonsterState::MS_CHASING:
		{
			ChasePlayer(deltaTime);
		}
		break;

		case MonsterState::MS_DEATH:
		{
			//TODO: 얘 아무것도 할거 없고
		}
		break;

		default:
			__debugbreak();
	}

	return _sendPacketsPerThread;
}

void Monster::MoveToDestination(float deltaTime)
{
	float dirX = _destination.X - _position.X;
	float dirY = _destination.Y - _position.Y;
	float distance = sqrt(dirX * dirX + dirY * dirY);

	if (distance < 10.0f)
	{
		// 목적지에 도착했으면
		_state = MonsterState::MS_IDLE;
		_idleTime = 5.0f;
	}
	else
	{
		// 목적지로 이동

		// 속도 * deltaTime
		float moveDist = _speed * deltaTime;
		if (moveDist > distance)
		{
			moveDist = distance;
		}


		// 이동거리 * x방향 / 거리
		float moveX = moveDist * dirX / distance;
		float moveY = moveDist * dirY / distance;

		_position.X += moveX;
		_position.Y += moveY;
	}
}

// 캐릭터가 나가버리거나 하면 이 targetPlayer를 들고있던 애는 어떻게해야하는거지
void Monster::AttackPlayer(float deltaTime)
{
	if (_targetPlayer)
	{
		float distance = GetDistanceToPlayer(_targetPlayer);
		if (distance <= _attackRange)
		{
			//공격범위안에들어왔으면
			// 추적 성공
			//추적 시간 초기화
			_chaseTime = 0.f;

			if (_attackTimer >= _attackCooldown)
			{
				CalculateRotation(_position, _targetPlayer->Position);
				//일단 Stop을 해야하네
				
				//Stop Packet도 그냥 보냅시다
	/*			CPacket*  monsterStopPacket = CPacket::Alloc();
				MP_SC_GAME_RES_MONSTER_STOP(monsterStopPacket, _monsterInfo.MonsterID, _position, _rotation);
				_GuardianFieldThread->SendPacket_BroadCast(monsterStopPacket);
				CPacket::Free(monsterStopPacket);*/

				//여기서 공격하면 된다
				//클라이언트에 몬스터 공격 애니메이션 전송 및
				//근데 로테이션을 캐릭터를 향해서 가져와야하네
				CPacket* monsterSkilPacket = CPacket::Alloc();
				int32 SkillID = 1;
				MP_SC_GAME_RES_MONSTER_SKILL(monsterSkilPacket, _monsterInfo.MonsterID, _position, _rotation, SkillID);
				_sendPacketsPerThread.push_back(monsterSkilPacket);
				
				/*_PacketHandleThread->SendPacket_BroadCast(monsterSkilPacket);
				CPacket::Free(monsterSkilPacket);*/
				
				//데미지 패킷 전송
				//공격했으면 데미지 패킷 보내여지
				CPacket* resDamagePacket = CPacket::Alloc();
				int32 AttackerType = TYPE_MONSTER;
				int64 AttackerID = _monsterInfo.MonsterID;
				int32 targetType = TYPE_PLAYER;
				int64 TargetID = _targetPlayer->playerInfo.PlayerID;
				int32 Damage = _damage;
				MP_SC_GAME_RES_DAMAGE(resDamagePacket, AttackerType, AttackerID, targetType, TargetID, Damage);
				_sendPacketsPerThread.push_back(resDamagePacket);
				_attackTimer = 0;
			}
		}
		else {
			_state = MonsterState::MS_CHASING;
		}

	}
}

void Monster::ChasePlayer(float deltaTime)
{
	if (_targetPlayer)
	{
		_chaseTime += deltaTime;

		if (_chaseTime > _maxChaseTime)
		{
			//idle상태라고 보내기 MonsterStop 패킷
			/*CPacket* idlePacket = CPacket::Alloc();
			MP_SC_GAME_RES_MONSTER_STOP(idlePacket, _monsterInfo.MonsterID, _position, _rotation);
			_GuardianFieldThread->SendPacket_BroadCast(idlePacket);
			CPacket::Free(idlePacket)*/;

			// maxChaseTime 초과하면
			_state = MonsterState::MS_IDLE;
			_idleTime = 5.0f;
			_targetPlayer = nullptr;
			_chaseTime = 0;
			return;
		}

		float distance = GetDistanceToPlayer(_targetPlayer);
		if (distance <= _attackRange)
		{
			_state = MonsterState::MS_ATTACKING;

		}
		else {
			//플에이어 몬스터 사이 거리
			float dirX = _targetPlayer->Position.X - _position.X;
			float dirY = _targetPlayer->Position.Y - _position.Y;
			float length = sqrt(dirX * dirX + dirY * dirY);

			//정규화 하고
			dirX /= length;
			dirY /= length;

			// attackRagne곱해서
			FVector Destination{
				_targetPlayer->Position.X - dirX * (_attackRange - 10), // 범위 안에서
				_targetPlayer->Position.Y - dirY * (_attackRange - 10),
				_position.Z
			};

			SetDestination(Destination);
			MoveToDestination(deltaTime);
		}
	}
}


void Monster::SetDestination(FVector dest)
{
	_destination = dest;
	//TODO: 클라이언트에 몬스터 이동 패킷 전송
	CPacket* packet = CPacket::Alloc();
	MP_SC_MONSTER_MOVE(packet, _monsterInfo.MonsterID, dest, _rotation);
	_sendPacketsPerThread.push_back(packet);
	CalculateRotation(_position, _destination);
}

void Monster::SetRandomDestination()
{
	//TODO: 현재 몬스터 위치 기반으로
	// 랜덤 목적지 정한다
	float range = 500.0f; // 범위 설정
	_destination.X = _position.X + (rand() % static_cast<int>(range * 2)) - range;
	_destination.Y = _position.Y + (rand() % static_cast<int>(range * 2)) - range;

	_destination.X = std::clamp(_destination.X, MIN_MAP_SIZE_X, MAX_MAP_SIZE_X);
	_destination.Y = std::clamp(_destination.Y, MIN_MAP_SIZE_Y, MAX_MAP_SIZE_Y);
	CalculateRotation(_position, _destination);


	//여기서부터 이동할거니까 이동패킷 보내면 되나
	CPacket* packet = CPacket::Alloc();
	MP_SC_MONSTER_MOVE(packet, _monsterInfo.MonsterID, _destination, _rotation);
	_sendPacketsPerThread.push_back(packet);

}

bool Monster::TakeDamage(int damage, Player* attacker)
{
	bool bDeath = false;
	if (_targetPlayer!= attacker)
	{
		// 다르면
		//할게 뭐가있지
		_chaseTime = 0.0f;
	}

	_monsterInfo.Hp -= damage;
	if (_monsterInfo.Hp > 0)
	{
		_targetPlayer = attacker;
		_state = MonsterState::MS_ATTACKING;
	}
	else {
		// 몬스터 풀에 넣어야하나?
		_state = MonsterState::MS_DEATH;
		bDeath = true;
	}
	return bDeath;
}

float Monster::GetDistanceToPlayer(Player* player)
{
	float dirX = player->Position.X - _position.X;
	float dirY = player->Position.Y - _position.Y;
	return sqrt(dirX * dirX + dirY * dirY);
}

FRotator Monster::CalculateRotation(const FVector& oldPosition, const FVector& newPosition)
{
	double dx = newPosition.X - oldPosition.X;
	double dy = newPosition.Y - oldPosition.Y;
	if (dx != 0 || dy != 0) { 
		// 움직임이 있을 경우에만 calculate rotation
		_rotation.Yaw = std::atan2(dy, dx) * 180 / PI; // 라디안에서 도, 언리얼 degree
	}

	return FRotator{0, _rotation.Yaw, 0 };
}

void Monster::SetTargetPlayerEmpty()
{
	_targetPlayer = nullptr;
	_state = MonsterState::MS_IDLE;
	_idleTime = _defaultIdleTime;

	//idle상태라고 보내기 MonsterStop 패킷
	CPacket* idlePacket = CPacket::Alloc();
	MP_SC_GAME_RES_MONSTER_STOP(idlePacket, _monsterInfo.MonsterID, _position, _rotation);
	_sendPacketsPerThread.push_back(idlePacket);
}
