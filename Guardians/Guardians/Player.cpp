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

// 플레이어의 위치와 회전축으로부터 월드변환 행렬을 생성하는 함수이다.
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

// 플레이어의 위치를 변경하는 함수. 키보드를 누를때 변경
// 이동방향 dwDirection에따라 플레이어를 fDistance만큼 이동
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
		////화살표 키 ‘↑’를 누르면 로컬 z-축 방향으로 이동(전진)한다. ‘↓’를 누르면 반대 방향으로 이동한다.
		//if (dwDirection & DIR_FORWARD) d3dxvShift += m_d3dxvLook * fDistance;
		//if (dwDirection & DIR_BACKWARD) d3dxvShift -= m_d3dxvLook * fDistance;
		//
		////화살표 키 ‘→’를 누르면 로컬 x-축 방향으로 이동한다. ‘←’를 누르면 반대 방향으로 이동한다.
		//if (dwDirection & DIR_RIGHT) d3dxvShift += m_d3dxvRight * fDistance;
		//if (dwDirection & DIR_LEFT) d3dxvShift -= m_d3dxvRight * fDistance;
		//
		////‘Page Up’을 누르면 로컬 y-축 방향으로 이동한다. ‘Page Down’을 누르면 반대 방향으로 이동한다.
		//if (dwDirection & DIR_UP) d3dxvShift += m_d3dxvUp * fDistance;
		//if (dwDirection & DIR_DOWN) d3dxvShift -= m_d3dxvUp * fDistance;

		////플레이어를 현재 위치 벡터에서 d3dxvShift 벡터 만큼 이동한다.
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
	// 플레이어를 현재 위치벡터에서 d3dxvShift벡터 만큼 이동
	m_d3dxvPosition += d3dxvShift;
	RegenerateWorldMatrix();
	
	//cout << this;
	// 플레이어의 위치가 변경되었으므로 카메라의 위치도 d3dxvShift벡터만큼이동
	//if(GetID()==NETWORK_ENGINE->GetID())
	//m_pCamera->Move(d3dxvShift);
}

void CPlayer::OtherMove()
{
	if (!m_deadReckoningQueue.empty())
	{
		D3DXVECTOR3 newPos = m_deadReckoningQueue.front();

		EJ_MATH->SmoothStep(m_d3dxvPosition, newPos, 0.3f);	// 1번째 2번째 인자 바꾸면 안됨
		RegenerateWorldMatrix();
		
		//cout << this;

		if (EJ_MATH->Vector3Equal(m_d3dxvPosition, newPos))
		{
			m_deadReckoningQueue.pop();		// 목적지까지 도착
		}
	}
}

void CPlayer::Rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;
	DWORD nCurrentCameraMode = m_pCamera->GetMode();

	// 1인칭 카메라 또는 3인칭 카메라의 경우 플레이어의 회전은 약간의 제약이 따름
	if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) || (nCurrentCameraMode == THIRD_PERSON_CAMERA))
	{
		// 로컬 x축을 중심으로 회전 회전각도는 -89.0 ~ 89.0 사이로 제한.
		
		//if (x != 0.0f)
		//{
		//	m_fPitch += x;
		//	if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
		//	if (m_fPitch < -89.9f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		//}
		//// 로컬 y축을 중심으로 회전 회전각도 제한없음
		//if (y != 0.0f)
		//{
		//	//m_fYaw += y;
		//	if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
		//	if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		//}

		//// 로컬 z축 회전 몸통을 좌우로 기울이는 것 회전각도는 -20 ~ 20 사이로 제한
		//// z는 현재 m_fRoll에서 실제 회전하는 각도 z만큼 회전
		//if (z != 0.0f)
		//{
		//	m_fRoll += z;
		//	if (m_fRoll > 20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
		//	if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		//}

		// 카메라를 x y z 만큼 회전. 플레이어가 회전하면 카메라가 회전
		m_pCamera->Rotate(x, y, z);

		if (y != 0.0f)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvUp, (float)D3DXToRadian(y));
			D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
			D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		}
	}
	// 회전으로 인해 플레이어의 로컬 축이 서로 직교하지 않을수 잇으므로 z축을 기준으로 
	// 서로 직교하고 단위벡터가 되도록 한다.
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
	D3DXVec3Cross(&m_d3dxvRight, &m_d3dxvUp, &m_d3dxvLook);
	
	D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
	D3DXVec3Cross(&m_d3dxvUp, &m_d3dxvLook, &m_d3dxvRight);
	
	D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);
	
	RegenerateWorldMatrix();
}

void CPlayer::Update(float fTimeElapsed)
{
	//플레이어의 속도 벡터를 중력 벡터와 더한다. 중력 벡터에 fTimeElapsed를 곱하는 것은
	// 중력을 시간에 비례하도록 적용한다는 의미.
	//m_d3dxvVelocity += m_d3dxvGravity * fTimeElapsed;

	//// 플레이어의 속도 벡터의 XZ-성분의 크기를 구함. 이것이 XZ-평면의 최대 속력보다
	//// 크면 속도 벡터의 x와 z방향 성분을 조정한다.
	//float fLength = sqrtf(m_d3dxvVelocity.x * m_d3dxvVelocity.x + m_d3dxvVelocity.z * m_d3dxvVelocity.z);
	//if (fLength > m_fMaxVelocityXZ)
	//{
	//	m_d3dxvVelocity.x *= (m_fMaxVelocityXZ / fLength);
	//	m_d3dxvVelocity.z *= (m_fMaxVelocityXZ / fLength);
	//}
	//
	//// 플레이어의 속도 벡터의 y성분의 크기를 구함. 이것이 y축방향의 최대속력보다 크면
	//// 속도 벡터의 y방향 성분을 조정한다.
	//fLength = sqrtf(m_d3dxvVelocity.y * m_d3dxvVelocity.y);
	//if (fLength > m_fMaxVelocityY) m_d3dxvVelocity.y *= (m_fMaxVelocityY / fLength);
	//
	//// 플레이어를 속도 벡터 만큼 이동한다. 속도 벡터에 fTimeElapsed를 곱하는 것은 속도를
	//// 시간에 비례하도록 적용의미.
	//Move(m_d3dxvVelocity*fTimeElapsed, false);
	
	// 플레이어의 위치가 변경될때 추가로 수행할 작업 수행
	// ex ) 플레이어 위치가 변경됫지만 플레이어 객체에는 지형정보가 없다.
	//		플레이어의 새로운 위치가 유효한 위치가 아닐수도 있고 또는 충돌 검사등을 수행할필요가있다.
	// 이런 상황에서 플레이어의 위치를 유효한 위치로 다시 변경

	DWORD nCurrentCameraMode = m_pCamera->GetMode();

	if (m_pPlayerUpdatedContext) OnPlayerUpdated(fTimeElapsed);
	
	// 플레이어의 위치가 변경 -> 카메라 상태 갱신
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->Update(fTimeElapsed);
	
	// 카메라 위치가 변경될때 추가로 수행할 작업 수행
	if (m_pCameraUpdatedContext) OnCameraUpdated(fTimeElapsed);
	
	// 카메라가 3인칭 카메라면 카메라가 변경된 플레이어 위치를 바라보도록 한다.
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->SetLookAt(m_d3dxvPosition);
	
	// 카메라의 카메라 변환 행렬 다시 생성
	m_pCamera->RegenerateViewMatrix();

	// 플레이어의 속도 벡터가 마찰력 때문에 감속이 되어야 한다면 감속 벡터를 생성.
	// 속도 벡터의 반대 방향 벡터를 구하고 단위 벡터로 만든다. 마찰 계수를 시간에 비례하도록
	// 하여 마찰력을 구함. 단위 벡터에 마찰력을 곱해 감속 벡터를 구함
	// 속도벡터에 감속벡터를 더하여 속도벡터를 줄인다.
	// 마찰력이 속력보다 크면 속력은 0

	//D3DXVECTOR3 d3dxvDeceleration = -m_d3dxvVelocity;
	//D3DXVec3Normalize(&d3dxvDeceleration, &d3dxvDeceleration);
	//
	//fLength = D3DXVec3Length(&m_d3dxvVelocity);
	//float fDeceleration = (m_fFriction * fTimeElapsed);
	//if (fDeceleration > fLength) fDeceleration = fLength;
	//m_d3dxvVelocity += d3dxvDeceleration * fDeceleration;
}

// 카메라 변경시 ChangeCamera() 에서 호출되는 함수
// nCurrentCameraMode = 현재모드 , nNewCameraMode는 새로 설정할 카메라모드
CCamera* CPlayer::OnChangeCamera(ID3D11Device* pd3dDevice, DWORD nNewCameraMode,
	DWORD nCurrentCameraMode)
{
	CCamera* pNewCamera = NULL;
	// 새로운 카메라의 모드에 따라 카메라를 새로 생성
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
		// 기존 카메라가 없으면 새로운 카메라르 위한 쉐이더 변수 생성
		if (!m_pCamera) pNewCamera->CreateShaderVariables(pd3dDevice);
		pNewCamera->SetMode(nNewCameraMode);
		// 현재 카메라를 사용하는 플레이어 객체를 설정한다.
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
		// 플레이어의 특성을 1인칭 카메라 모드에 맞게 변경한다. 중력 적용 ㄴㄴ
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
		// 플레이어의 특성을 3인칭 카메라 모드에 맞게 변경한다. 지연 효과와 카메라 오프셋 설정.
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
	// 카메라 정보를 시간에 따라 갱신.
	Update(fTimeElapsed);
}

void CPlayer::CreateShaderVariables(ID3D11Device* pd3dDevice)
{
	
}

void CPlayer::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext)
{
	// 플레이어의 현재 카메라의 updateShaderVariables() 멤버 함수를 호출 한다.
	if (m_pCamera) m_pCamera->UpdateShaderVariables(pd3dDeviceContext);
	
}


void CPlayer::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	//DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	//if ((nCurrentCameraMode == THIRD_PERSON_CAMERA) && m_pMesh)
	//{
	//	D3DXMATRIX mtxRotate;
	//	/*3인칭 카메라일 때 플레이어 메쉬를 로컬 x-축을 중심으로 +90도 회전하고 렌더링한다. 왜냐하면 비행기 모델 메쉬는 <그림 18>과 같이 y-축 방향이 비행기의 앞쪽이 되도록 모델링이 되었고 이 메쉬를 카메라의 z-축 방향으로 향하도록 그릴 것이기 때문이다.*/
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
	//m_pPlayerUpdatedContext는 지형에 대한 포인터로 설정되었다.
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pPlayerUpdatedContext;
	D3DXVECTOR3 d3dxvScale = HMAP->GetScale();
	
	DWORD nCurrentCameraMode = m_pCamera->GetMode();

	D3DXVECTOR3 d3dxvPlayerPosition;

	d3dxvPlayerPosition = GetPosition();
	
	int z = (int)(d3dxvPlayerPosition.z / d3dxvScale.z);
	bool bReverseQuad = ((z % 2) != 0);
	/*높이 맵에서 플레이어의 현재 위치 (x, z)의 y 값을 구한다. 그리고 플레이어 메쉬의 높이가 12이고 플레이어의 중심이 직육면체의 가운데이므로 y 값에 메쉬의 높이의 절반을 더하면 플레이어의 위치가 된다.*/
	m_fHeight = HMAP->GetHeight(d3dxvPlayerPosition.x, d3dxvPlayerPosition.z, bReverseQuad);// +6.0f;
	/*플레이어의 속도 벡터의 y-값이 음수이면(예를 들어, 중력이 적용되는 경우) 플레이어의 위치 벡터의 y-값이 점점 작아지게 된다. 이때 플레이어의 현재 위치의 y 값이 지형의 높이보다 작으면 플레이어가 땅속에 있게 되므로 플레이어의 속도 벡터의 y 값을 0으로 만들고 플레이어의 위치 벡터의 y-값을 지형 높이(실제로 지형의 높이 + 6)로 설정한다. 그러면 플레이어는 더 이상 하강하지 않고 지형 위에 서 있게 된다.*/
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
	//m_pCameraUpdatedContext는 지형에 대한 포인터로 설정되었다.
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pCameraUpdatedContext;
	D3DXVECTOR3 d3dxvScale = HMAP->GetScale();
	CCamera *pCamera = GetCamera();
	D3DXVECTOR3 d3dxvCameraPosition = pCamera->GetPosition();
	int z = (int)(d3dxvCameraPosition.z / d3dxvScale.z);
	bool bReverseQuad = ((z % 2) != 0);
	/*높이 맵에서 카메라의 현재 위치 (x, z)의 y 값을 구한다. 이 값이 카메라의 위치에 해당하는 지형의 높이 보다 작으면 카메라가 땅속에 있게 된다. 이렇게 되면 <그림 4>의 왼쪽과 같이 지형이 그려지지 않는 경우가 발생한다(카메라가 지형 안에 있으므로 와인딩 순서가 바뀐다). 이러한 경우 카메라의 위치를 보정하기 위하여 카메라가 지형에 근접한 경우(현재는 지형의 높이 + 5) 카메라의 위치를 더 이상 내려가지 않도록 한다.*/
	float fHeight = HMAP->GetHeight(d3dxvCameraPosition.x, d3dxvCameraPosition.z, bReverseQuad) + 5.0f;
	if (d3dxvCameraPosition.y < fHeight)
	{
		d3dxvCameraPosition.y = fHeight;
		pCamera->SetPosition(d3dxvCameraPosition);
		//카메라의 y-위치가 변경되었으므로 카메라가 플레이어를 바라보도록 한다.
		if (pCamera->GetMode() == THIRD_PERSON_CAMERA)
		{
			CThirdPersonCamera *p3rdPersonCamera = (CThirdPersonCamera *)pCamera;
			p3rdPersonCamera->SetLookAt(GetPosition());
		}
	}
}


