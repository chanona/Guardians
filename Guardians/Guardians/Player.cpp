#include "stdafx.h"
#include "Player.h"
#include "PlayerManager.h"
#include "HeightMapTerrain.h"
#include "ClientNetEngine.h"
#include "TextureManager.h"
#include "EJMath.h"

CPlayer::CPlayer()
{
	m_pCamera = NULL;
	//m_pCamera = new CCamera(NULL);
		
	m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_d3dxvRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_d3dxvLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//m_d3dxvVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_d3dxvGravity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_fMaxVelocityXZ = 0.0f;
	//m_fMaxVelocityY = 0.0f;
	//m_fFriction = 0.0f;
	
	m_direction = Direction::STOP;
	//m_vDirection = D3DXVECTOR3(0.f, 0.f, 1.f);

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;
	m_speed = PLAYER_SHIFT;

	m_fHeight = 0.f;

	m_pPlayerUpdatedContext = NULL;
	m_pCameraUpdatedContext = NULL;
	
	m_pShader = nullptr;
}


CPlayer::~CPlayer()
{
	if (m_pCamera) delete m_pCamera;
}

void CPlayer::Initalize()
{
	this->SetPlayerUpdatedContext(FRAMEWORK->GetInstance()->m_pScene->m_ppShaders[SHADER_TERRA]->m_pTerrain);
	this->SetCameraUpdatedContext(FRAMEWORK->GetInstance()->m_pScene->m_ppShaders[SHADER_TERRA]->m_pTerrain);
		
	ChangeCamera(FRAMEWORK->GetInstance()->m_pd3dDevice, THIRD_PERSON_CAMERA, FRAMEWORK->GetInstance()->m_GameTimer.GetTimeElapsed());

	m_pCamera->CreateShaderVariables(FRAMEWORK->GetInstance()->m_pd3dDevice);
	m_pCamera->SetViewPort(FRAMEWORK->GetInstance()->m_pd3dDeviceContext, 0, 0, FRAMEWORK->GetInstance()->m_nWndClientWidth, FRAMEWORK->GetInstance()->m_nWndClientHeight,
		0.0f, 1.0f);

	m_pCamera->GenerateProjectionMatrix(1.0f, 5000.0f, float(FRAMEWORK->GetInstance()->m_nWndClientWidth) / float(FRAMEWORK->GetInstance()->m_nWndClientHeight), 60.0f);
	m_pCamera->GenerateViewMatrix();
	m_pCamera->CreateShaderVariables(FRAMEWORK->GetInstance()->m_pd3dDevice);

	//ChangeCamera(FRAMEWORK->GetInstance()->m_pd3dDevice, THIRD_PERSON_CAMERA, FRAMEWORK->GetInstance()->m_GameTimer.GetTimeElapsed());
}

// �÷��̾��� ��ġ�� ȸ�������κ��� ���庯ȯ ����� �����ϴ� �Լ��̴�.
void CPlayer::RegenerateWorldMatrix()
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

// �÷��̾��� ��ġ�� �����ϴ� �Լ�. Ű���带 ������ ����
// �̵����� dwDirection������ �÷��̾ fDistance��ŭ �̵�
void CPlayer::Move(BYTE direction)
{
	D3DXVECTOR3 d3dxvShift = D3DXVECTOR3(0, 0, 0);

	if (direction & Direction::FORWARD)  d3dxvShift += m_d3dxvLook * m_speed;
	if (direction & Direction::BACKWARD) d3dxvShift -= m_d3dxvLook * m_speed;
	if (direction & Direction::LEFT)     d3dxvShift -= m_d3dxvRight * m_speed;
	if (direction & Direction::RIGHT)    d3dxvShift += m_d3dxvRight * m_speed;

	Move(d3dxvShift);
	m_pCamera->Move(d3dxvShift);
	//if (dwDirection)
	//{
		//D3DXVECTOR3 d3dxvShift = D3DXVECTOR3(0, 0, 0);
		//
		////ȭ��ǥ Ű ���衯�� ������ ���� z-�� �������� �̵�(����)�Ѵ�. ���顯�� ������ �ݴ� �������� �̵��Ѵ�.
		//if (dwDirection & DIR_FORWARD) d3dxvShift += m_d3dxvLook * fDistance;
		//if (dwDirection & DIR_BACKWARD) d3dxvShift -= m_d3dxvLook * fDistance;
		//
		////ȭ��ǥ Ű ���桯�� ������ ���� x-�� �������� �̵��Ѵ�. ���硯�� ������ �ݴ� �������� �̵��Ѵ�.
		//if (dwDirection & DIR_RIGHT) d3dxvShift += m_d3dxvRight * fDistance;
		//if (dwDirection & DIR_LEFT) d3dxvShift -= m_d3dxvRight * fDistance;
		//
		////��Page Up���� ������ ���� y-�� �������� �̵��Ѵ�. ��Page Down���� ������ �ݴ� �������� �̵��Ѵ�.
		//if (dwDirection & DIR_UP) d3dxvShift += m_d3dxvUp * fDistance;
		//if (dwDirection & DIR_DOWN) d3dxvShift -= m_d3dxvUp * fDistance;

		////�÷��̾ ���� ��ġ ���Ϳ��� d3dxvShift ���� ��ŭ �̵��Ѵ�.
		//Move(d3dxvShift, bUpdateVelocity);

		/*if (dwDirection & DIR_FORWARD)
		{
			D3DXVECTOR3 vDir = m_vDirection;
			D3DXVec3Normalize(&vDir, &vDir);
			m_d3dxvPosition += vDir * fDistance * 3;
		}
		if (dwDirection & DIR_BACKWARD)
		{
			D3DXVECTOR3 vDir = m_vDirection;
			D3DXVec3Normalize(&vDir, &vDir);
			m_d3dxvPosition -= vDir * fDistance * 3;
		}
		if (dwDirection & DIR_LEFT)
		{
			D3DXMATRIX matView;
			D3DXVECTOR3 vRight;
			matView = m_pCamera->GetViewMatrix();
			D3DXMatrixInverse(&matView, NULL, &matView);
			memcpy(&vRight, &matView.m[0][0], sizeof(D3DXVECTOR3));
			m_d3dxvPosition -= vRight * fDistance * 3;
		}

		if (dwDirection & DIR_RIGHT)
		{
			D3DXMATRIX matView;
			D3DXVECTOR3 vRight;
			matView = m_pCamera->GetViewMatrix();
			D3DXMatrixInverse(&matView, NULL, &matView);
			memcpy(&vRight, &matView.m[0][0], sizeof(D3DXVECTOR3));
			m_d3dxvPosition += vRight * fDistance * 3;
		}*/
		//m_pCamera->Move(d3dxvShift);
	//}
}
void CPlayer::Move(const D3DXVECTOR3& d3dxvShift)
{
	// �÷��̾ ���� ��ġ���Ϳ��� d3dxvShift���� ��ŭ �̵�
	m_d3dxvPosition += d3dxvShift;
	RegenerateWorldMatrix();
	
	//cout << this;
	// �÷��̾��� ��ġ�� ����Ǿ����Ƿ� ī�޶��� ��ġ�� d3dxvShift���͸�ŭ�̵�
	//if(GetID()==NETWORK_ENGINE->GetID())
	//m_pCamera->Move(d3dxvShift);
}

void CPlayer::OtherMove()
{
	if (!m_deadReckoningQueue.empty())
	{
		D3DXVECTOR3 newPos = m_deadReckoningQueue.front();

		EJ_MATH->SmoothStep(m_d3dxvPosition, newPos, 0.3f);	// 1��° 2��° ���� �ٲٸ� �ȵ�
		RegenerateWorldMatrix();
		
		//cout << this;

		if (EJ_MATH->Vector3Equal(m_d3dxvPosition, newPos))
		{
			m_deadReckoningQueue.pop();		// ���������� ����
		}
	}
}

void CPlayer::Rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;
	DWORD nCurrentCameraMode = m_pCamera->GetMode();

	// 1��Ī ī�޶� �Ǵ� 3��Ī ī�޶��� ��� �÷��̾��� ȸ���� �ణ�� ������ ����
	if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) || (nCurrentCameraMode == THIRD_PERSON_CAMERA))
	{
		// ���� x���� �߽����� ȸ�� ȸ�������� -89.0 ~ 89.0 ���̷� ����.
		
		//if (x != 0.0f)
		//{
		//	m_fPitch += x;
		//	if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
		//	if (m_fPitch < -89.9f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		//}
		//// ���� y���� �߽����� ȸ�� ȸ������ ���Ѿ���
		//if (y != 0.0f)
		//{
		//	//m_fYaw += y;
		//	if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
		//	if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		//}

		//// ���� z�� ȸ�� ������ �¿�� ����̴� �� ȸ�������� -20 ~ 20 ���̷� ����
		//// z�� ���� m_fRoll���� ���� ȸ���ϴ� ���� z��ŭ ȸ��
		//if (z != 0.0f)
		//{
		//	m_fRoll += z;
		//	if (m_fRoll > 20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
		//	if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		//}

		// ī�޶� x y z ��ŭ ȸ��. �÷��̾ ȸ���ϸ� ī�޶� ȸ��
		m_pCamera->Rotate(x, y, z);

		if (y != 0.0f)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvUp, (float)D3DXToRadian(y));
			D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
			D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		}
	}
	// ȸ������ ���� �÷��̾��� ���� ���� ���� �������� ������ �����Ƿ� z���� �������� 
	// ���� �����ϰ� �������Ͱ� �ǵ��� �Ѵ�.
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
	D3DXVec3Cross(&m_d3dxvRight, &m_d3dxvUp, &m_d3dxvLook);
	
	D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
	D3DXVec3Cross(&m_d3dxvUp, &m_d3dxvLook, &m_d3dxvRight);
	
	D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);
	
	RegenerateWorldMatrix();
}

void CPlayer::Update(float fTimeElapsed)
{
	//�÷��̾��� �ӵ� ���͸� �߷� ���Ϳ� ���Ѵ�. �߷� ���Ϳ� fTimeElapsed�� ���ϴ� ����
	// �߷��� �ð��� ����ϵ��� �����Ѵٴ� �ǹ�.
	//m_d3dxvVelocity += m_d3dxvGravity * fTimeElapsed;

	//// �÷��̾��� �ӵ� ������ XZ-������ ũ�⸦ ����. �̰��� XZ-����� �ִ� �ӷº���
	//// ũ�� �ӵ� ������ x�� z���� ������ �����Ѵ�.
	//float fLength = sqrtf(m_d3dxvVelocity.x * m_d3dxvVelocity.x + m_d3dxvVelocity.z * m_d3dxvVelocity.z);
	//if (fLength > m_fMaxVelocityXZ)
	//{
	//	m_d3dxvVelocity.x *= (m_fMaxVelocityXZ / fLength);
	//	m_d3dxvVelocity.z *= (m_fMaxVelocityXZ / fLength);
	//}
	//
	//// �÷��̾��� �ӵ� ������ y������ ũ�⸦ ����. �̰��� y������� �ִ�ӷº��� ũ��
	//// �ӵ� ������ y���� ������ �����Ѵ�.
	//fLength = sqrtf(m_d3dxvVelocity.y * m_d3dxvVelocity.y);
	//if (fLength > m_fMaxVelocityY) m_d3dxvVelocity.y *= (m_fMaxVelocityY / fLength);
	//
	//// �÷��̾ �ӵ� ���� ��ŭ �̵��Ѵ�. �ӵ� ���Ϳ� fTimeElapsed�� ���ϴ� ���� �ӵ���
	//// �ð��� ����ϵ��� �����ǹ�.
	//Move(m_d3dxvVelocity*fTimeElapsed, false);
	
	// �÷��̾��� ��ġ�� ����ɶ� �߰��� ������ �۾� ����
	// ex ) �÷��̾� ��ġ�� ��������� �÷��̾� ��ü���� ���������� ����.
	//		�÷��̾��� ���ο� ��ġ�� ��ȿ�� ��ġ�� �ƴҼ��� �ְ� �Ǵ� �浹 �˻���� �������ʿ䰡�ִ�.
	// �̷� ��Ȳ���� �÷��̾��� ��ġ�� ��ȿ�� ��ġ�� �ٽ� ����

	DWORD nCurrentCameraMode = m_pCamera->GetMode();

	if (m_pPlayerUpdatedContext) OnPlayerUpdated(fTimeElapsed);
	
	// �÷��̾��� ��ġ�� ���� -> ī�޶� ���� ����
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->Update(fTimeElapsed);
	
	// ī�޶� ��ġ�� ����ɶ� �߰��� ������ �۾� ����
	if (m_pCameraUpdatedContext) OnCameraUpdated(fTimeElapsed);
	
	// ī�޶� 3��Ī ī�޶�� ī�޶� ����� �÷��̾� ��ġ�� �ٶ󺸵��� �Ѵ�.
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->SetLookAt(m_d3dxvPosition);
	
	// ī�޶��� ī�޶� ��ȯ ��� �ٽ� ����
	m_pCamera->RegenerateViewMatrix();

	// �÷��̾��� �ӵ� ���Ͱ� ������ ������ ������ �Ǿ�� �Ѵٸ� ���� ���͸� ����.
	// �ӵ� ������ �ݴ� ���� ���͸� ���ϰ� ���� ���ͷ� �����. ���� ����� �ð��� ����ϵ���
	// �Ͽ� �������� ����. ���� ���Ϳ� �������� ���� ���� ���͸� ����
	// �ӵ����Ϳ� ���Ӻ��͸� ���Ͽ� �ӵ����͸� ���δ�.
	// �������� �ӷº��� ũ�� �ӷ��� 0

	//D3DXVECTOR3 d3dxvDeceleration = -m_d3dxvVelocity;
	//D3DXVec3Normalize(&d3dxvDeceleration, &d3dxvDeceleration);
	//
	//fLength = D3DXVec3Length(&m_d3dxvVelocity);
	//float fDeceleration = (m_fFriction * fTimeElapsed);
	//if (fDeceleration > fLength) fDeceleration = fLength;
	//m_d3dxvVelocity += d3dxvDeceleration * fDeceleration;
}

// ī�޶� ����� ChangeCamera() ���� ȣ��Ǵ� �Լ�
// nCurrentCameraMode = ������ , nNewCameraMode�� ���� ������ ī�޶���
CCamera* CPlayer::OnChangeCamera(ID3D11Device* pd3dDevice, DWORD nNewCameraMode,
	DWORD nCurrentCameraMode)
{
	CCamera* pNewCamera = NULL;
	// ���ο� ī�޶��� ��忡 ���� ī�޶� ���� ����
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA :
		pNewCamera = new CFirstPersonCamera(m_pCamera);

		break;
	case THIRD_PERSON_CAMERA:
		pNewCamera = new CThirdPersonCamera(m_pCamera);	
	}

	if (pNewCamera)
	{
		// ���� ī�޶� ������ ���ο� ī�޶� ���� ���̴� ���� ����
		if (!m_pCamera) pNewCamera->CreateShaderVariables(pd3dDevice);
		pNewCamera->SetMode(nNewCameraMode);
		// ���� ī�޶� ����ϴ� �÷��̾� ��ü�� �����Ѵ�.
		//pNewCamera->SetPlayer(this);
	}
	if (m_pCamera) delete m_pCamera;

	return pNewCamera;
}


void CPlayer::ChangeCamera(ID3D11Device* pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return;
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		// �÷��̾��� Ư���� 1��Ī ī�޶� ��忡 �°� �����Ѵ�. �߷� ���� ����
		//SetFriction(200.0f);
		//SetGravity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//SetMaxVelocityXZ(125.0f);
		//SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffSet(D3DXVECTOR3(0.0f, 1000.0f, -500.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);

		break;

	case THIRD_PERSON_CAMERA:
		// �÷��̾��� Ư���� 3��Ī ī�޶� ��忡 �°� �����Ѵ�. ���� ȿ���� ī�޶� ������ ����.
		//SetFriction(250.0f);
		//SetGravity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//SetMaxVelocityXZ(125.0f);
		//SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetOffSet(D3DXVECTOR3(0.0f, 600.0f, -500.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);

		break;

	default:
		break;
	}
	// ī�޶� ������ �ð��� ���� ����.
	Update(fTimeElapsed);
}

void CPlayer::CreateShaderVariables(ID3D11Device* pd3dDevice)
{
	
}

void CPlayer::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext)
{
	// �÷��̾��� ���� ī�޶��� updateShaderVariables() ��� �Լ��� ȣ�� �Ѵ�.
	if (m_pCamera) m_pCamera->UpdateShaderVariables(pd3dDeviceContext);
	
}


void CPlayer::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	//DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	//if ((nCurrentCameraMode == THIRD_PERSON_CAMERA) && m_pMesh)
	//{
	//	D3DXMATRIX mtxRotate;
	//	/*3��Ī ī�޶��� �� �÷��̾� �޽��� ���� x-���� �߽����� +90�� ȸ���ϰ� �������Ѵ�. �ֳ��ϸ� ����� �� �޽��� <�׸� 18>�� ���� y-�� ������ ������� ������ �ǵ��� �𵨸��� �Ǿ��� �� �޽��� ī�޶��� z-�� �������� ���ϵ��� �׸� ���̱� �����̴�.*/
	//	D3DXMatrixRotationYawPitchRoll(&mtxRotate, 0.0f, (float)D3DXToRadian(90.0f), 0.0f);
	//	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
	//}

	if (m_pShader)
	{
		m_pShader->UpdateShaderVariables(pd3dDeviceContext, &m_d3dxmtxWorld);
		m_pShader->Render(pd3dDeviceContext);
	}
	
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);
}

void CPlayer::OnPlayerUpdated(float fTimeElapsed)
{
	//m_pPlayerUpdatedContext�� ������ ���� �����ͷ� �����Ǿ���.
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pPlayerUpdatedContext;
	D3DXVECTOR3 d3dxvScale = HMAP->GetScale();
	
	DWORD nCurrentCameraMode = m_pCamera->GetMode();

	D3DXVECTOR3 d3dxvPlayerPosition;

	d3dxvPlayerPosition = GetPosition();
	
	int z = (int)(d3dxvPlayerPosition.z / d3dxvScale.z);
	bool bReverseQuad = ((z % 2) != 0);
	/*���� �ʿ��� �÷��̾��� ���� ��ġ (x, z)�� y ���� ���Ѵ�. �׸��� �÷��̾� �޽��� ���̰� 12�̰� �÷��̾��� �߽��� ������ü�� ����̹Ƿ� y ���� �޽��� ������ ������ ���ϸ� �÷��̾��� ��ġ�� �ȴ�.*/
	m_fHeight = HMAP->GetHeight(d3dxvPlayerPosition.x, d3dxvPlayerPosition.z, bReverseQuad);// +6.0f;
	/*�÷��̾��� �ӵ� ������ y-���� �����̸�(���� ���, �߷��� ����Ǵ� ���) �÷��̾��� ��ġ ������ y-���� ���� �۾����� �ȴ�. �̶� �÷��̾��� ���� ��ġ�� y ���� ������ ���̺��� ������ �÷��̾ ���ӿ� �ְ� �ǹǷ� �÷��̾��� �ӵ� ������ y ���� 0���� ����� �÷��̾��� ��ġ ������ y-���� ���� ����(������ ������ ���� + 6)�� �����Ѵ�. �׷��� �÷��̾�� �� �̻� �ϰ����� �ʰ� ���� ���� �� �ְ� �ȴ�.*/
	//if (d3dxvPlayerPosition.y > m_fHeight)
	//{
		//D3DXVECTOR3 d3dxvPlayerVelocity = GetVelocity();
		//d3dxvPlayerVelocity.y = 0.0f;
		//SetVelocity(d3dxvPlayerVelocity);
		d3dxvPlayerPosition.y = m_fHeight;
		SetPosition(d3dxvPlayerPosition);
	//}
}
void CPlayer::OnCameraUpdated(float fTimeElapsed)
{
	//m_pCameraUpdatedContext�� ������ ���� �����ͷ� �����Ǿ���.
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pCameraUpdatedContext;
	D3DXVECTOR3 d3dxvScale = HMAP->GetScale();
	CCamera *pCamera = GetCamera();
	D3DXVECTOR3 d3dxvCameraPosition = pCamera->GetPosition();
	int z = (int)(d3dxvCameraPosition.z / d3dxvScale.z);
	bool bReverseQuad = ((z % 2) != 0);
	/*���� �ʿ��� ī�޶��� ���� ��ġ (x, z)�� y ���� ���Ѵ�. �� ���� ī�޶��� ��ġ�� �ش��ϴ� ������ ���� ���� ������ ī�޶� ���ӿ� �ְ� �ȴ�. �̷��� �Ǹ� <�׸� 4>�� ���ʰ� ���� ������ �׷����� �ʴ� ��찡 �߻��Ѵ�(ī�޶� ���� �ȿ� �����Ƿ� ���ε� ������ �ٲ��). �̷��� ��� ī�޶��� ��ġ�� �����ϱ� ���Ͽ� ī�޶� ������ ������ ���(����� ������ ���� + 5) ī�޶��� ��ġ�� �� �̻� �������� �ʵ��� �Ѵ�.*/
	float fHeight = HMAP->GetHeight(d3dxvCameraPosition.x, d3dxvCameraPosition.z, bReverseQuad) + 5.0f;
	if (d3dxvCameraPosition.y < fHeight)
	{
		d3dxvCameraPosition.y = fHeight;
		pCamera->SetPosition(d3dxvCameraPosition);
		//ī�޶��� y-��ġ�� ����Ǿ����Ƿ� ī�޶� �÷��̾ �ٶ󺸵��� �Ѵ�.
		if (pCamera->GetMode() == THIRD_PERSON_CAMERA)
		{
			CThirdPersonCamera *p3rdPersonCamera = (CThirdPersonCamera *)pCamera;
			p3rdPersonCamera->SetLookAt(GetPosition());
		}
	}
}


