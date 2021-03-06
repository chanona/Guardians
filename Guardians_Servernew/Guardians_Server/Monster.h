#pragma once

#include "Entity.h"

class CMonster : public CEntity
{
public:
	CMonster();
	virtual ~CMonster();

	void Initalize();
	void Clear() override;
	
	void Move();		// 나중에 펫일때랑 몬스터일때랑 구분해서...
	bool UpdateSector(int new_sector_x, int new_sector_y);
	void Awake();
	void Sleep();
	int GetDamagaed(const int hit_damage);
private :
	
	MonsterType   m_monsterType;
	UINT          m_index;
	int           m_preAiTime;

	atomic<bool>  m_bPet;
	atomic<bool>  m_bAlive;
	atomic<bool>  m_bActive;

	// StateMachine , 현재상태 등등 나중에 추가 예정
public :
	// Setter
	void SetPositionX(const float x) {
		m_position.x = x; m_collisionBoundingSphere.Center.x = x; m_viewBoundingSphere.Center.x = x; }	// Warning : x,y,z 의 기준점이 어디인지 잘 파악하고  써야함... 지금은 중앙이라고 가정하고 사용
	void SetPositionY(const float y) {
		m_position.y = y; m_collisionBoundingSphere.Center.y = y; m_viewBoundingSphere.Center.y = y; }	// 3D때는 Z도 고쳐야함
	void SetPositionZ(const float z) {
		m_position.z = z; m_collisionBoundingSphere.Center.z = z; m_viewBoundingSphere.Center.z = z; }
	void SetPosition(const XMFLOAT3& position) {
		SetPositionX(position.x); SetPositionY(position.y); SetPositionZ(position.z);
	}
	void SetPosition(const float x, const float y, const float z) { 
		SetPositionX(x); SetPositionY(y); SetPositionZ(z); 
	}
	void SetMonsterType(const MonsterType type) { m_monsterType = type; }
	void SetIndex(const UINT index) { m_index = index; }
	void SetAlive(const bool alive) { m_bAlive = alive; }
	void SetActive(const bool active) { m_bActive = active; }
	void SetPreAiTime(const int time) { m_preAiTime = time; }

	// Getter
	const MonsterType	 GetMonsterType()         { return m_monsterType; }
	const UINT			 GetIndex()         const { return m_index; }
	const atomic<bool>&  GetAlive()		          { return m_bAlive; }
	const atomic<bool>&  IsActive()         const { return m_bActive; }
	const atomic<bool>&  IsPet()            const { return m_bPet; }
	const int			 GetPreAiTime()		const { return m_preAiTime; }
	
};

