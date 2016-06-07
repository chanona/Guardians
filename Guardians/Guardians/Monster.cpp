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
	//�÷��̾��� ��ġ�� ����� �� ������ ������ ���� �÷��̾��� ��ġ�� ������ �� �ֵ��� �����Ѵ�.
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
	////bUpdateVelocity�� ���̸� �÷��̷��� �̵����� �ʰ� �ӵ� ���� ����.
	//if (bVelocity)
	//{
	//	m_d3dxvVelocity += d3dxvShift;
	//}
	//else
	//{
	//	// �÷��̾ ���� ��ġ���Ϳ��� d3dxvShift���� ��ŭ �̵�
	//	D3DXVECTOR3 d3dxvPosition = m_d3dxvPosition + d3dxvShift;
	//	m_d3dxvPosition = d3dxvPosition;
	//	RegenerateWorldMatrix();
	//}
	if (!m_deadReckoningQueue.empty())
	{
		D3DXVECTOR3 newPos = m_deadReckoningQueue.front();

		EJ_MATH->SmoothStep(m_d3dxvPosition, newPos, 0.5);	// 1��° 2��° ���� �ٲٸ� �ȵ�

		if (m_d3dxvPosition.x <= 0.f)	m_d3dxvPosition.x = 1.f;
		if (m_d3dxvPosition.z <= 0.f)	m_d3dxvPosition.z = 1.f;
				
		RegenerateWorldMatrix();

		if (EJ_MATH->Vector3Equal(m_d3dxvPosition,newPos))
		{
			m_deadReckoningQueue.pop();		// ���������� ����
		}
	}
}

void CMonster::Rotate(float x, float y, float z)
{
}

void CMonster::Update(float fTimeElapsed)
{
	//m_d3dxvVelocity += m_d3dxvGravity * fTimeElapsed;

	//// �÷��̾��� �ӵ� ������ XZ-������ ũ�⸦ ����. �̰��� XZ-����� �ִ� �ӷº���
	//// ũ�� �ӵ� ������ x�� z���� ������ �����Ѵ�.
	//float fLength = sqrtf(m_d3dxvVelocity.x * m_d3dxvVelocity.x + m_d3dxvVelocity.z * m_d3dxvVelocity.z);
	//if (fLength > m_fMaxVelocityXZ)
	//{
	//	m_d3dxvVelocity.x *= (m_fMaxVelocityXZ / fLength);
	//	m_d3dxvVelocity.z *= (m_fMaxVelocityXZ / fLength);
	//}

	//// �÷��̾��� �ӵ� ������ y������ ũ�⸦ ����. �̰��� y������� �ִ�ӷº��� ũ��
	//// �ӵ� ������ y���� ������ �����Ѵ�.
	//fLength = sqrtf(m_d3dxvVelocity.y * m_d3dxvVelocity.y);
	//if (fLength > m_fMaxVelocityY) m_d3dxvVelocity.y *= (m_fMaxVelocityY / fLength);

	//// �÷��̾ �ӵ� ���� ��ŭ �̵��Ѵ�. �ӵ� ���Ϳ� fTimeElapsed�� ���ϴ� ���� �ӵ���
	//// �ð��� ����ϵ��� �����ǹ�.
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
	/*���� �ʿ��� �÷��̾��� ���� ��ġ (x, z)�� y ���� ���Ѵ�. �׸��� �÷��̾� �޽��� ���̰� 12�̰� �÷��̾��� �߽��� ������ü�� ����̹Ƿ� y ���� �޽��� ������ ������ ���ϸ� �÷��̾��� ��ġ�� �ȴ�.*/

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
