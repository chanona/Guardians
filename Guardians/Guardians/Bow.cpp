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
	//�÷��̾��� ��ġ�� ����� �� ������ ������ ���� �÷��̾��� ��ġ�� ������ �� �ֵ��� �����Ѵ�.
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
	//bUpdateVelocity�� ���̸� �÷��̷��� �̵����� �ʰ� �ӵ� ���� ����.
	if (bVelocity)
	{
		m_d3dxvVelocity += d3dxvShift;
	}
	else
	{
		// �÷��̾ ���� ��ġ���Ϳ��� d3dxvShift���� ��ŭ �̵�
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
	/*���� �ʿ��� �÷��̾��� ���� ��ġ (x, z)�� y ���� ���Ѵ�. �׸��� �÷��̾� �޽��� ���̰� 12�̰� �÷��̾��� �߽��� ������ü�� ����̹Ƿ� y ���� �޽��� ������ ������ ���ϸ� �÷��̾��� ��ġ�� �ȴ�.*/
	//float fHeight = pTerrain->m_pHeightMap->GetHeight(d3dxMonsterPosition.x, d3dxMonsterPosition.z, bReverseQuad) + 6.0f;
	///*�÷��̾��� �ӵ� ������ y-���� �����̸�(���� ���, �߷��� ����Ǵ� ���) �÷��̾��� ��ġ ������ y-���� ���� �۾����� �ȴ�. �̶� �÷��̾��� ���� ��ġ�� y ���� ������ ���̺��� ������ �÷��̾ ���ӿ� �ְ� �ǹǷ� �÷��̾��� �ӵ� ������ y ���� 0���� ����� �÷��̾��� ��ġ ������ y-���� ���� ����(������ ������ ���� + 6)�� �����Ѵ�. �׷��� �÷��̾�� �� �̻� �ϰ����� �ʰ� ���� ���� �� �ְ� �ȴ�.*/
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
