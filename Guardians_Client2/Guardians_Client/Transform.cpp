#include "stdafx.h"
#include "Transform.h"

USING(Engine)

Engine::CTransform::CTransform(void)
{

}

Engine::CTransform::~CTransform(void)
{

}


_int Engine::CTransform::Update(const _float& fTimeDelta)
{
	_matrix			matScale, matRotX, matRotY, matRotZ, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationX(&matRotX, m_fAngle[ANGLE_X]);
	D3DXMatrixRotationY(&matRotY, m_fAngle[ANGLE_Y]);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle[ANGLE_Z]);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;

	return 0;
}

HRESULT Engine::CTransform::Initialize(void)
{
	m_vScale = _vec3(1.f, 1.f, 1.f);

	m_vPosition = _vec3(0.0f, 0.0f, 0.f);

	ZeroMemory(m_fAngle, sizeof(_float) * ANGLE_END);

	D3DXMatrixIdentity(&m_matWorld);
	
	return S_OK;
}

CTransform* Engine::CTransform::Create(void)
{
	CTransform*	pTransform = new CTransform;

	if(FAILED(pTransform->Initialize()))
	{
		MSG_BOX("pTransform Create Failed");
		Engine::Safe_Release(pTransform);
	}
	return pTransform;
}

_ulong Engine::CTransform::Release()
{
	_ulong dwRefCnt = Engine::CComponent::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}
	return dwRefCnt;
}