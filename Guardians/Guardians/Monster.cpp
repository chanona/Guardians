#include "stdafx.h"
#include "Monster.h"
#include "Shader.h"
#include "protocol.h"
#include "EJMath.h"

CMonster::CMonster()
{
	m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_d3dxvRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_d3dxvLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_d3dxvVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_d3dxvGravity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	m_fHeight = 0.f;

	m_pShader = nullptr;

	m_bCircleInit = true;

	m_pMonsterUpdatedContext = NULL;
}

CMonster::~CMonster()
{
}

void CMonster::Initalize()
{
	//pMonster->SetPosition(D3DXVECTOR3(0.f, 550.0f, 0.f));
	//플레이어의 위치가 변경될 때 지형의 정보에 따라 플레이어의 위치를 변경할 수 있도록 설정한다.
	SetMonsterUpdatedContext(FRAMEWORK->GetInstance()->m_pScene->m_ppShaders[SHADER_TERRA]->m_pTerrain);
}

void CMonster::Clear()
{
	//m_d3dxvPosition = D3DXVECTOR3(MONSTER_INIT_X, MONSTER_INIT_Y, MONSTER_INIT_Z);
	m_d3dxvRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_d3dxvLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_d3dxvVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_d3dxvGravity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	SetAlive(false);
	// add hp, mp, etx
}

void CMonster::Move(ULONG nDirection, float fDistance, bool bVelocity)
{
}

void CMonster::Move()
{
	////bUpdateVelocity가 참이면 플레이러르 이동하지 않고 속도 벡터 변경.
	//if (bVelocity)
	//{
	//	m_d3dxvVelocity += d3dxvShift;
	//}
	//else
	//{
	//	// 플레이어를 현재 위치벡터에서 d3dxvShift벡터 만큼 이동
	//	D3DXVECTOR3 d3dxvPosition = m_d3dxvPosition + d3dxvShift;
	//	m_d3dxvPosition = d3dxvPosition;
	//	RegenerateWorldMatrix();
	//}
	if (!m_deadReckoningQueue.empty())
	{
		D3DXVECTOR3 newPos = m_deadReckoningQueue.front();

		EJ_MATH->SmoothStep(m_d3dxvPosition, newPos, 0.5);	// 1번째 2번째 인자 바꾸면 안됨

		if (m_d3dxvPosition.x <= 0.f)	m_d3dxvPosition.x = 1.f;
		if (m_d3dxvPosition.z <= 0.f)	m_d3dxvPosition.z = 1.f;
				
		RegenerateWorldMatrix();

		if (EJ_MATH->Vector3Equal(m_d3dxvPosition,newPos))
		{
			m_deadReckoningQueue.pop();		// 목적지까지 도착
		}
	}
}

void CMonster::Rotate(float x, float y, float z)
{
}

void CMonster::Update(float fTimeElapsed)
{
	//m_d3dxvVelocity += m_d3dxvGravity * fTimeElapsed;

	//// 플레이어의 속도 벡터의 XZ-성분의 크기를 구함. 이것이 XZ-평면의 최대 속력보다
	//// 크면 속도 벡터의 x와 z방향 성분을 조정한다.
	//float fLength = sqrtf(m_d3dxvVelocity.x * m_d3dxvVelocity.x + m_d3dxvVelocity.z * m_d3dxvVelocity.z);
	//if (fLength > m_fMaxVelocityXZ)
	//{
	//	m_d3dxvVelocity.x *= (m_fMaxVelocityXZ / fLength);
	//	m_d3dxvVelocity.z *= (m_fMaxVelocityXZ / fLength);
	//}

	//// 플레이어의 속도 벡터의 y성분의 크기를 구함. 이것이 y축방향의 최대속력보다 크면
	//// 속도 벡터의 y방향 성분을 조정한다.
	//fLength = sqrtf(m_d3dxvVelocity.y * m_d3dxvVelocity.y);
	//if (fLength > m_fMaxVelocityY) m_d3dxvVelocity.y *= (m_fMaxVelocityY / fLength);

	//// 플레이어를 속도 벡터 만큼 이동한다. 속도 벡터에 fTimeElapsed를 곱하는 것은 속도를
	//// 시간에 비례하도록 적용의미.
	//Move(m_d3dxvVelocity*fTimeElapsed, false);

	//if (m_pMonsterUpdatedContext) OnMonsterUpdated(fTimeElapsed);

	//D3DXVECTOR3 d3dxvDeceleration = -m_d3dxvVelocity;
	//D3DXVec3Normalize(&d3dxvDeceleration, &d3dxvDeceleration);

	//fLength = D3DXVec3Length(&m_d3dxvVelocity);
	//float fDeceleration = (m_fFriction * fTimeElapsed);
	//if (fDeceleration > fLength) fDeceleration = fLength;
	//m_d3dxvVelocity += d3dxvDeceleration * fDeceleration;
	Move();

	//CircleCollision()
}

void CMonster::RegenerateWorldMatrix()
{
	m_d3dxmtxWorld._11 = m_d3dxvRight.x;
	m_d3dxmtxWorld._12 = m_d3dxvRight.y;
	m_d3dxmtxWorld._13 = m_d3dxvRight.z;

	m_d3dxmtxWorld._21 = m_d3dxvUp.x;
	m_d3dxmtxWorld._22 = m_d3dxvUp.y;
	m_d3dxmtxWorld._23 = m_d3dxvUp.z;

	m_d3dxmtxWorld._31 = m_d3dxvLook.x;
	m_d3dxmtxWorld._32 = m_d3dxvLook.y;
	m_d3dxmtxWorld._33 = m_d3dxvLook.z;

	m_d3dxmtxWorld._41 = m_d3dxvPosition.x;
	m_d3dxmtxWorld._42 = m_d3dxvPosition.y;
	m_d3dxmtxWorld._43 = m_d3dxvPosition.z;
}

void CMonster::OnMonsterUpdated(float fTimeElpased)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pMonsterUpdatedContext;
	D3DXVECTOR3 d3dxvScale = HMAP->GetScale();
	D3DXVECTOR3 d3dxMonsterPosition = GetPosition();
	int z = (int)(d3dxMonsterPosition.z / d3dxvScale.z);
	bool bReverseQuad = ((z % 2) != 0);
	/*높이 맵에서 플레이어의 현재 위치 (x, z)의 y 값을 구한다. 그리고 플레이어 메쉬의 높이가 12이고 플레이어의 중심이 직육면체의 가운데이므로 y 값에 메쉬의 높이의 절반을 더하면 플레이어의 위치가 된다.*/

	//OBJECT_MANAGER->GetMonsterMap()

	//m_fHeight = HMAP->GetHeight(d3dxMonsterPosition.x, d3dxMonsterPosition.z, bReverseQuad) + 6.0f;

	//if (d3dxMonsterPosition.y < m_fHeight)
	//{
	//	D3DXVECTOR3 d3dxvMonsterVelocity = GetVelocity();
	//	d3dxvMonsterVelocity.y = 0.0f;
	//	SetVelocity(d3dxvMonsterVelocity);
	//	d3dxMonsterPosition.y = m_fHeight;
	//	SetPosition(d3dxMonsterPosition);
	//}
}

void CMonster::Render(ID3D11DeviceContext * pd3dDeviceContext)
{
	if (m_pShader)
	{
		m_pShader->UpdateShaderVariables(pd3dDeviceContext, &m_d3dxmtxWorld);
		m_pShader->Render(pd3dDeviceContext);
	}
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);
}

void CMonster::SetServerPos(float fX, float fZ)
{
	m_d3dxvPosition.x = fX;
	m_d3dxvPosition.z = fZ;
}
