#include "stdafx.h"
#include "Camera.h"
#include "GameObject.h"
#include "Player.h"
#include "protocol.h"
#include "PlayerManager.h"

CCamera::CCamera(CCamera* pCamera = NULL)
{
	if (pCamera)
	{
		// ī�޶� �̹� ������ ���� ī�޶��� ������ ���ο� ī�޶� �����Ѵ�.
		m_d3dxvPosition = pCamera->GetPosition();
		m_d3dxvRight = pCamera->GetRightVector();
		m_d3dxvLook = pCamera->GetLookVector();
		m_d3dxvUp = pCamera->GetUpVector();
		m_fPitch = pCamera->GetPitch();
		m_fRoll = pCamera->GetRoll();
		m_fYaw = pCamera->GetYaw();
		m_d3dxmtxView = pCamera->GetViewMatrix();
		m_d3dxmtxProjection = pCamera->GetProjectionMatrix();
		m_d3dViewport = pCamera->GetViewport();
		m_d3dxvLookAtWorld = pCamera->GetLookAtPosition();
		m_d3dxvOffset = pCamera->GetOffset();
		m_fTimeLag = pCamera->GetTimeLag();
		m_pd3dcbCamera = pCamera->GetCameraConstantBuffer();

		if (m_pd3dcbCamera) m_pd3dcbCamera->AddRef();
	}
	/*else
	{
		m_d3dxvPosition = D3DXVECTOR3(PLAYER_INIT_X, PLAYER_INIT_Y, PLAYER_INIT_Z);
		m_d3dxvRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_d3dxvLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		m_fPitch = 0.0f;
		m_fRoll = 0.0f;

		m_fYaw = 0.0f;

		m_fTimeLag = 0.0f;

		m_d3dxvLookAtWorld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvOffset = D3DXVECTOR3(0.0f, 0.f, 0.f);

		m_nMode = 0x00;

		D3DXMatrixIdentity(&m_d3dxmtxView);
		D3DXMatrixIdentity(&m_d3dxmtxProjection);

		m_pd3dcbCamera = NULL;
	}*/
}
CCamera::~CCamera()
{
	if (m_pd3dcbCamera) m_pd3dcbCamera->Release();

}

void CCamera::GenerateViewMatrix()
{
	// ī�޶� ��ȯ ����� �����ϴ� �Լ��̴�.
	// ī�޶��� ��ġ ����, ī�޶� �ٶ󺸴� ����, ī�޶��� ������
	D3DXMatrixLookAtLH(&m_d3dxmtxView, &m_d3dxvPosition, &m_d3dxvLookAtWorld, &m_d3dxvUp);

}
// z���� �������� ī�޶� ��ȯ����� �����Ѵ�.
void CCamera::RegenerateViewMatrix()
{
	// ī�޶��� z�� ���͸� ����ȭ
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
	// ī�޶��� z��� y�࿡ ������ ���͸� x������ ����
	D3DXVec3Cross(&m_d3dxvRight, &m_d3dxvUp, &m_d3dxvLook);
	// ī�޶��� x�� ���͸� ����ȭ�Ѵ�.
	D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
	// ī�޶��� z��� x�࿡ ������ ���͸� y������ �����Ѵ�.
	D3DXVec3Cross(&m_d3dxvUp, &m_d3dxvLook, &m_d3dxvRight);
	// ī�޶��� y�� ���͸� ����ȭ�Ѵ�.
	D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);

	m_d3dxmtxView._11 = m_d3dxvRight.x;
	m_d3dxmtxView._12 = m_d3dxvUp.x;
	m_d3dxmtxView._13 = m_d3dxvLook.x;
	m_d3dxmtxView._21 = m_d3dxvRight.y;
	m_d3dxmtxView._22 = m_d3dxvUp.y;
	m_d3dxmtxView._23 = m_d3dxvLook.y;
	m_d3dxmtxView._31 = m_d3dxvRight.z;
	m_d3dxmtxView._32 = m_d3dxvUp.z;
	m_d3dxmtxView._33 = m_d3dxvLook.z;
	m_d3dxmtxView._41 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvRight);
	m_d3dxmtxView._42 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvUp);
	m_d3dxmtxView._43 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvLook);
}

// ī�޶� Ŭ������ ���̴� ��� ���۸� �����ϱ� ���� �Լ�
void CCamera::CreateShaderVariables(ID3D11Device* pd3dDevice)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_CAMERA);			// �������
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbCamera);

}

void CCamera::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext)
{
	// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� ���̴��� �����ϱ� ���� ����ü�� 
	// ���� ��� ���۸� �����ϰ� ����̽� ���ؽ�Ʈ�� ���Կ� �����ϴ� �Լ��̴�.
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	// ��� ������ �޸� �ּҸ� �����ͼ� ī�޶� ��ȯ ��İ� ���� ��ȯ ����� �����Ѵ�.
	// ���̴����� ����� ��� ���� �ٲ�� �Ϳ� �����϶�.

	HRESULT hResult = S_OK;


	//pd3dDeviceContext->Map(m_pd3dcbCamera, 0, D3D11_MAP_WRITE_DISCARD,
	//	0, &d3dMappedResource);
	//VS_CB_CAMERA *pcbViewProjection =
	//	(VS_CB_CAMERA*)d3dMappedResource.pData;
	//D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxView, &m_d3dxmtxView);
	//D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxProjection, &m_d3dxmtxProjection);

	//pd3dDeviceContext->Unmap(m_pd3dcbCamera, 0);

	//// ��� ���۸� ����(VS_SLOT_VIEWPROJECTION_MATRIX)�� �����Ѵ�.
	//pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_CAMERA, 1,
	//	&m_pd3dcbCamera);

	pd3dDeviceContext->Map(m_pd3dcbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_CAMERA *pcbViewProjection = (VS_CB_CAMERA *)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxView, &m_d3dxmtxView);
	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxProjection, &m_d3dxmtxProjection);
	pd3dDeviceContext->Unmap(m_pd3dcbCamera, 0);

	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_CAMERA, 1, &m_pd3dcbCamera);
}



// �� ��Ʈ�� �����ϴ� ��� �Լ��� ����
void CCamera::SetViewPort(ID3D11DeviceContext* pd3dDeviceContext, DWORD xStart, DWORD yStart,
	DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ)
{
	m_d3dViewport.TopLeftX = float(xStart);
	m_d3dViewport.TopLeftY = float(yStart);
	m_d3dViewport.Width = float(nWidth);
	m_d3dViewport.Height = float(nHeight);
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;

	pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);
}
// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� �����ϴ� ��� �Լ��� �����Ѵ�.

// ī�޶� ��ȯ ����� �����ϴ� �Լ��̴�. ī�޶��� ��ġ ����, ī�޶� �ٶ󺸴� ����,
// ī�޶��� Up ����(���� y-�� ����)�� �Ķ���ͷ� ����ϴ� D3DXMatrixLookAtLH() �Լ��� ����Ѵ�.


// ���� ��ȯ ����� �����ϴ� �Լ��̴�.ī�޶� ��ǥ���� ����� �Ÿ�(ī�޶󿡼� �� �� �ִ� 
// ���� ����� �Ÿ�), ����� �Ÿ�(ī�޶󿡼� �� �� �ִ� ���� �� �Ÿ�), ȭ���� ��Ⱦ��(Aspect Ratio), 
// ī�޶� FOV ������  ��ġ ����, ī�޶� �ٶ󺸴� ����, ī�޶��� Up ����(���� y - �� ����)��
// �Ķ���ͷ� ����ϴ� D3DXMatrixPerspectiveFovLH() �Լ��� ����Ѵ�.
void CCamera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance,
	float fAspectRatio, float fFOVAngle)
{
	D3DXMatrixPerspectiveFovLH(&m_d3dxmtxProjection, (float)D3DXToRadian(fFOVAngle),
		fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}


/////// First Person Camera Class ////////

// 1��Ī ī�޶�� ����� ���� �ǹ�
CFirstPersonCamera::CFirstPersonCamera(CCamera* pCamera) : CCamera(pCamera)
{
	m_nMode = FIRST_PERSON_CAMERA;
	if (pCamera)
	{
		// 1��Ī ī�޶�� �����ϱ� ������ ī�޶� �����̽� �� ī�޶�� ī�޶��� up���͸�
		// ���� ��ǥ�� y���� �ǵ��� �Ѵ�.
		// �̰��� �����̽� �� ī�޶��� ���� y�� ���Ͱ� � �����̵��� 1��Ī ī�޶��� 
		// ���� y�� ���Ͱ� ������ǥ�� y���� �ǵ��� �� , �ȹٷ� ���ִ� ���·� �����Ѵٴ� �ǹ�
		// �׸��� ���� x�຤�Ϳ� ���� z�� ������ y��ǥ�� 0.0f�� �ǵ��� �Ѵ�. 
		// �̰��� ���� x�� ���Ϳ� ���� z�� ���͸� xz������� �����ϴ� ���� �ǹ�. 
		// �� 1��Ī ī�޶��� ���� x�� ���Ϳ� ���� z�� ���ʹ� xz��鿡 �����ϴ�.
	}
}

void CFirstPersonCamera::Rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;
	for (auto player : PLAYER_MANAGER->GetPlayerMap())
	{
		if (player.second->IsConnected())
		{
			if (x != 0.0f)
			{
				// ī�޶��� ���� x���� �������� ȸ���ϴ� ����� ���� �� �����̴� ����.
				D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvRight, (float)D3DXToRadian(x));

				// ī�޶��� ���� x�� y�� z���� ȸ���Ѵ�.
				D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
				D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
				D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
			}
			if (player.second && (y != 0.0f))
			{
				// �÷��̾��� ���� y���� �������� ȸ���ϴ� ����� �����Ѵ�.
				D3DXMatrixRotationAxis(&mtxRotate, &(player.second->GetUpVector()), (float)D3DXToRadian(y));

				// ī�޶��� ���� x�� y�� z���� ȸ���Ѵ�.
				D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
				D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
				D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);

			}
			if (player.second && (z != 0.0f))
			{
				//�÷��̾��� ���� z-���� �������� ȸ���ϴ� ����� �����Ѵ�.
				D3DXMatrixRotationAxis(&mtxRotate, &(player.second->GetLookVector()), (float)D3DXToRadian(z));
				//ī�޶��� ��ġ ���͸� �÷��̾� ��ǥ��� ǥ���Ѵ�(������ ����).
				m_d3dxvPosition -= player.second->GetPosition();
				//������ ���� ���͸� ȸ���Ѵ�.
				D3DXVec3TransformCoord(&m_d3dxvPosition, &m_d3dxvPosition, &mtxRotate);
				//ȸ���� ī�޶��� ��ġ�� ���� ��ǥ��� ǥ���Ѵ�.
				m_d3dxvPosition += player.second->GetPosition();
				//ī�޶��� ���� x-��, y-��, z-���� ȸ���Ѵ�.
				D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
				D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
				D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
			}
		}
	}
}

////// Third Person Camera Class ////////

// 3��Ī ī�޶�� �� 3���� ������ �÷��̾ �ٶ󺻴�. 
// ������ : �÷��̾���� ������ �Ÿ�
// LookAt ���� : �÷��̾��� ������ ������ �ٶ󺸵��� �Ѵ�.
// �������� �÷��̾�� ī�޶� ������ �����Ͽ� ������ ��.

CThirdPersonCamera::CThirdPersonCamera(CCamera* pCamera) : CCamera(pCamera)
{
	m_nMode = THIRD_PERSON_CAMERA;
	if (pCamera)
	{
		// �����̽� �� ī�޶� ���� ���.


	}
}

// 3��Ī ī�޶��� ȸ���� �÷��̾��� ȸ������ �߻�. 
// �÷��̾��� ȸ���� �÷��̾��� ���� y���� �߽����� �ϴ� ȸ���� ����.
// �÷��̾ ȸ���ϸ� ī�޶�� �÷��̾��� y���� �߽����� ȸ���ϰ� �� �� �÷��̾ �ٶ󺸵��� �ؾ���
void CThirdPersonCamera::Update(float fTimeElapsed)
{
	//�÷��̾��� ȸ���� ���� 3��Ī ī�޶� ȸ���ؾ� �Ѵ�.

	for (auto player : PLAYER_MANAGER->GetPlayerMap())
	{
		if (player.second->IsConnected())
		{
			D3DXMATRIX mtxRotate;
			D3DXMatrixIdentity(&mtxRotate);
			D3DXVECTOR3 d3dxvRight = player.second->GetRightVector();
			D3DXVECTOR3 d3dxvUp = player.second->GetUpVector();
			D3DXVECTOR3 d3dxvLook = player.second->GetLookVector();
			//�÷��̾��� ���� x-��, y-��, z-�� ���ͷκ��� ȸ�� ����� �����Ѵ�.
			mtxRotate._11 = d3dxvRight.x;
			mtxRotate._12 = d3dxvRight.y;
			mtxRotate._13 = d3dxvRight.z;

			mtxRotate._21 = d3dxvUp.x;
			mtxRotate._22 = d3dxvUp.y;
			mtxRotate._23 = d3dxvUp.z;

			mtxRotate._31 = d3dxvLook.x;
			mtxRotate._32 = d3dxvLook.y;
			mtxRotate._33 = d3dxvLook.z;

			D3DXVECTOR3 d3dxvOffset;
			D3DXVec3TransformCoord(&d3dxvOffset, &m_d3dxvOffset, &mtxRotate);

			//ȸ���� ī�޶��� ��ġ�� �÷��̾��� ��ġ�� ȸ���� ī�޶� ������ ���͸� ���� ���̴�.
			D3DXVECTOR3 d3dxvPosition = player.second->GetPosition() + d3dxvOffset;

			//������ ī�޶��� ��ġ���� ȸ���� ī�޶��� ��ġ������ �����̴�.
			D3DXVECTOR3 d3dxvDirection = d3dxvPosition - m_d3dxvPosition;

			float fLength = D3DXVec3Length(&d3dxvDirection);
			D3DXVec3Normalize(&d3dxvDirection, &d3dxvDirection);

			/*3��Ī ī�޶��� ����(Lag)�� �÷��̾ ȸ���ϴ��� ī�޶� ���ÿ� ���� ȸ������ �ʰ� �ణ�� ������ �ΰ� ȸ���ϴ� ȿ���� �����ϱ� ���� ���̴�. m_fTimeLag�� 1���� ũ�� fTimeLagScale�� �۾����� ���� ȸ���� ���� �Ͼ ���̴�.*/
			float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;
			
			float fDistance = fLength * fTimeLagScale;
			
			if (fDistance > fLength) fDistance = fLength;
			if (fLength < 0.01f) fDistance = fLength;
			if (fDistance > 0)
			{
				m_d3dxvPosition += d3dxvDirection * fDistance;
				SetLookAt(GetLookAtPosition());
			}
		}
	}
}

// 3��Ī ī�޶� ȸ���� ī�޶��� ��ġ�� ������ �ٲ��.
// ī�޶� �÷��̾ �ٶ󺸵��� �ؾ� �ϹǷ� ���ο� ī�޶��� ��ġ ���͸� ����Ͽ� 
// ī�޶��� ���� ���͸� �ٽ� �����Ѵ�. �̸� ���� ī�޶� ��ȯ ����� ���. 

void CThirdPersonCamera::SetLookAt(D3DXVECTOR3& d3dxvLookAt)
{
	D3DXMATRIX mtxLookAt;

	for (auto player : PLAYER_MANAGER->GetPlayerMap())
	{
		if (player.second->IsConnected())
		{
			D3DXMatrixLookAtLH(&mtxLookAt, &m_d3dxvPosition, &d3dxvLookAt, &player.second->GetUpVector());
			m_d3dxvRight = D3DXVECTOR3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
			m_d3dxvUp = D3DXVECTOR3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
			m_d3dxvLook = D3DXVECTOR3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
		}
	}
}