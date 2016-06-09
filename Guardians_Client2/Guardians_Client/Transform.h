#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class  CTransform : public Engine::CComponent
{
public:
	enum ANGLE {ANGLE_X, ANGLE_Y, ANGLE_Z, ANGLE_END};
private:
	explicit CTransform(void);
	virtual ~CTransform(void);
public:
	HRESULT Initialize(void);
public:
	virtual _int Update(const _float& fTimeDelta);
public:
	static CTransform* Create(void);
public:
	_matrix					m_matWorld;
	_vec3					m_vPosition;
	_vec3					m_vScale;
	_float					m_fAngle[ANGLE_END];
public:
	virtual _ulong Release(void);
};

END

#endif // Transform_h__
