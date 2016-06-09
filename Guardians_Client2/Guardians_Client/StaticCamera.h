/*!
* \file DynamicCamera.h
* \date 2015/08/17 21:30
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Defines.h"
#include "Camera.h"
#include "Transform.h"
#include "Base.h"

namespace Engine
{
	class CTimeMgr;
	class CInput;
}

class CStaticCamera : public Engine::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CStaticCamera(void);
public:
	HRESULT InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

public:
	virtual _int Update(const _float& fTimeDelta);
public:
	static CStaticCamera* Create(LPDIRECT3DDEVICE9 pDevice
		, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);
private:
	void FixMouse(void);

private:
	void SetCameraPos(const _float& fTimeDelta);

public:
	D3DXMATRIX	GetView() { return m_matView; }
	D3DXMATRIX	GetProj() { return m_matProj; }

private:
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInput*			m_pInput;

private:
	float					m_fCamSpeed;
	bool					m_bMouseFix;
	bool					m_bClick;
	bool					m_bStart;
	float m_fHeelUp;
	float m_fHeelDown;

	float m_fHeight;
	float m_fAngle;
public:
	virtual _ulong Release(void);
};

#endif // CStaticCamera