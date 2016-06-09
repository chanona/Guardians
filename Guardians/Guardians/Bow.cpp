#include "stdafx.h"
#include "Bow.h"
#include "Shader.h"
#include "protocol.h"
#include "ClientNetEngine.h"

CBow::CBow()
{
	m_d3dxvPosition = D3DXVECTOR3(1028.0f, 500.0f, 1028.0f);
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

	m_pBowUpdatedContext = NULL;
}

CBow::~CBow()
{
}

void CBow::Initalize()
{
	//pMonster->SetPosition(D3DXVECTOR3(0.f, 550.0f, 0.f));
	//플레이어의 위치가 변경될 때 지형의 정보에 따라 플레이어의 위치를 변경할 수 있도록 설정한다.
	SetMonsterUpdatedContext(FRAMEWORK->GetInstance()->m_pScene->m_ppShaders[SHADER_TERRA]->m_pTerrain);
}

void CBow::Clear()
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

void CBow::Move(ULONG nDirection, float fDistance, bool bVelocity)
{
}

void CBow::Move(const D3DXVECTOR3 & d3dxvShift, bool bVelocity)
{
	//bUpdateVelocity가 참이면 플레이러르 이동하지 않고 속도 벡터 변경.
	if (bVelocity)
	{
		m_d3dxvVelocity += d3dxvShift;
	}
	else
	{
		// 플레이어를 현재 위치벡터에서 d3dxvShift벡터 만큼 이동
		D3DXVECTOR3 d3dxvPosition = m_d3dxvPosition + d3dxvShift + 10.f * m_d3dxvLook;
		m_d3dxvPosition = d3dxvPosition;
		RegenerateWorldMatrix();
	}
}

void CBow::Rotate(float x, float y, float z)
{
}

void CBow::Update(float fTimeElapsed)
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

	//if (m_pBowUpdatedContext) OnMonsterUpdated(fTimeElapsed);

	//D3DXVECTOR3 d3dxvDeceleration = -m_d3dxvVelocity;
	//D3DXVec3Normalize(&d3dxvDeceleration, &d3dxvDeceleration);

	//fLength = D3DXVec3Length(&m_d3dxvVelocity);
	//float fDeceleration = (m_fFriction * fTimeElapsed);
	//if (fDeceleration > fLength) fDeceleration = fLength;
	//m_d3dxvVelocity += d3dxvDeceleration * fDeceleration;

	//D3DXVECTOR3 d3dxvPosition = m_d3dxvPosition;
	//m_d3dxvPosition = d3dxvPosition;

	m_d3dxvPosition = NETWORK_ENGINE->GetMyPlayer()->GetPosition();
	RegenerateWorldMatrix();
}

void CBow::RegenerateWorldMatrix()
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

void CBow::OnMonsterUpdated(float fTimeElpased)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pBowUpdatedContext;
	D3DXVECTOR3 d3dxvScale = pTerrain->m_pHeightMap->GetScale();
	D3DXVECTOR3 d3dxMonsterPosition = GetPosition();
	int z = (int)(d3dxMonsterPosition.z / d3dxvScale.z);
	bool bReverseQuad = ((z % 2) != 0);
	/*높이 맵에서 플레이어의 현재 위치 (x, z)의 y 값을 구한다. 그리고 플레이어 메쉬의 높이가 12이고 플레이어의 중심이 직육면체의 가운데이므로 y 값에 메쉬의 높이의 절반을 더하면 플레이어의 위치가 된다.*/
	//float fHeight = pTerrain->m_pHeightMap->GetHeight(d3dxMonsterPosition.x, d3dxMonsterPosition.z, bReverseQuad) + 6.0f;
	///*플레이어의 속도 벡터의 y-값이 음수이면(예를 들어, 중력이 적용되는 경우) 플레이어의 위치 벡터의 y-값이 점점 작아지게 된다. 이때 플레이어의 현재 위치의 y 값이 지형의 높이보다 작으면 플레이어가 땅속에 있게 되므로 플레이어의 속도 벡터의 y 값을 0으로 만들고 플레이어의 위치 벡터의 y-값을 지형 높이(실제로 지형의 높이 + 6)로 설정한다. 그러면 플레이어는 더 이상 하강하지 않고 지형 위에 서 있게 된다.*/
	//if (d3dxMonsterPosition.y < fHeight)
	//{
	//	D3DXVECTOR3 d3dxvMonsterVelocity = GetVelocity();
	//	d3dxvMonsterVelocity.y = 0.0f;
	//	SetVelocity(d3dxvMonsterVelocity);
	//	d3dxMonsterPosition.y = fHeight;
	//	SetPosition(d3dxMonsterPosition);
	//}
}

void CBow::Render(ID3D11DeviceContext * pd3dDeviceContext)
{
	if (m_pShader)
	{
		m_pShader->UpdateShaderVariables(pd3dDeviceContext, &m_d3dxmtxWorld);
		m_pShader->Render(pd3dDeviceContext);
	}
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);
}
