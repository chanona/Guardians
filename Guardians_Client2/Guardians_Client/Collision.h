#ifndef Collision_h__
#define Collision_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class  CCollision
{
	DECLARE_SINGLETON(CCollision)
private:
	typedef struct tagOBB
	{
		_vec3			vPoint[8];
		_vec3			vCenter;
		_vec3			vProjAxis[3];
		_vec3			vAxis[3];		
	}OBB;

private:
	explicit CCollision(void);
	~CCollision(void);
public:
	_bool Collision_AABB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld
		, const _vec3& vSourMin, const _vec3& vSourMax, const _matrix& mSourWorld);
	_bool Collision_OBB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld
		, const _vec3& vSourMin, const _vec3& vSourMax, const _matrix& mSourWorld);
private:
	OBB						m_tOBB[2];

private:
	void Set_Points(OBB* pOBB, const _vec3& vMin, const _vec3& vMax);
	void Set_Axis(OBB* pOBB);

public:
	_ulong Release(void);
};

END

#endif // Collision_h__
