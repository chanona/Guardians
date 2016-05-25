#pragma once

#define FIRST_PERSON_CAMERA			0x01
#define THIRD_PERSON_CAMERA			0x02

#define ASPECT_RATIO	(float(FRAME_BUFFER_WIDTH)/float(FRAME_BUFFER_HEIGHT))

// ��� ���۸� ���� �����͸� ǥ���ϱ� ���� ����ü�� ������ ���� ����.
// �� ����ü�� ī�޶� ��ȯ ��İ� ���� ��ȯ ����� ��Ÿ����
struct VS_CB_CAMERA
{
	D3DXMATRIX m_d3dxmtxView;
	D3DXMATRIX m_d3dxmtxProjection;
};

// �÷��̾ ��Ÿ���� Ŭ����
class CPlayer;

class CCamera
{
public:

	CCamera(CCamera *pCamera);
	//CCamera(){}
	~CCamera();

protected:
	// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� ��Ÿ���� ��� ������ �����Ѵ�.
	D3DXMATRIX m_d3dxmtxView;
	D3DXMATRIX m_d3dxmtxProjection;

	// ī�޶��� ��ġ ����
	D3DXVECTOR3 m_d3dxvPosition;

	// ī�޶��� ���� �� x y z
	D3DXVECTOR3 m_d3dxvRight;	// x
	D3DXVECTOR3	m_d3dxvUp;		// y
	D3DXVECTOR3 m_d3dxvLook;	// z

	// ī�޶��� ȸ������ 
	float m_fPitch;
	float m_fRoll;
	float m_fYaw;

	// ī�޶��� ����(1��Ī ī�޶�, 3��Ī ī�޶�) �̴�
	DWORD m_nMode;

	// ī�޶� �ٶ󺸴� ���� ��Ÿ���� ����
	D3DXVECTOR3 m_d3dxvLookAtWorld;

	// �÷��̾�� ī�޶��� �������� ��Ÿ���� �����̴� �ַ� 3��Ī ī�޶󿡼� ���ȴ�.
	D3DXVECTOR3 m_d3dxvOffset;

	// �÷��̾ ȸ���� �� �󸶸�ŭ�� �ð��� ������Ű�� ī�޶� ȸ����ų���ΰ��� ��Ÿ����.
	float m_fTimeLag;

	// ��-��Ʈ�� ��Ÿ���� ��� ������ �����Ѵ�.
	D3D11_VIEWPORT m_d3dViewport;

	// ī�޶� ��ȯ��İ� ���� ��ȯ ����� ���� ��� ���� �������̽� �����͸� �����Ѵ�.
	ID3D11Buffer* m_pd3dcbCamera;

public:
	// ī�޶� ��ȯ����� �����Ѵ�.
	D3DXMATRIX GetViewMatrix() { return m_d3dxmtxView; }

	// zī�޶� ������ ȸ���� �ϰ� �Ǹ� ������ �Ǽ������� ����Ȯ�� ������ ī�޶��� ���� x��
	// y�� z���� ���� �������� ���� �� �ִ�. ī�޶��� ���� x�� y�� z���� ���� �����ϵ��� ������ش�
	D3DXMATRIX GetProjectionMatrix() { return(m_d3dxmtxProjection); }

public:

	void SetMode(DWORD nMode) { m_nMode = nMode; }
	DWORD GetMode() { return m_nMode; }

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance,
		float fAspectRatio, float fFOVAngle);

	// ��� ���۸� �����ϰ� ������ �����ϴ� ��� �Լ��� �����Ѵ�.
	void CreateShaderVariables(ID3D11Device* pd3dDevice);
	void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext);

	// �� ��Ʈ�� �����ϴ� ��� �Լ��� ����
	void SetViewPort(ID3D11DeviceContext* pd3dDeviceContext, DWORD xStart, DWORD yStart,
		DWORD nWidth, DWORD nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	D3D11_VIEWPORT GetViewport() { return m_d3dViewport; }

	
	
	ID3D11Buffer* GetCameraConstantBuffer() { return m_pd3dcbCamera; }

	void SetPosition(D3DXVECTOR3 d3dxvPosition)	 { m_d3dxvPosition = d3dxvPosition; }
	D3DXVECTOR3& GetPosition() { return m_d3dxvPosition; }

	void SetLookAtPosition(D3DXVECTOR3 d3dxvLookAtWorld) { m_d3dxvLookAtWorld = d3dxvLookAtWorld; }
	D3DXVECTOR3& GetLookAtPosition() { return m_d3dxvLookAtWorld; }

	D3DXVECTOR3& GetRightVector() { return m_d3dxvRight; }
	D3DXVECTOR3& GetUpVector()	  { return m_d3dxvUp; }
	D3DXVECTOR3& GetLookVector()  { return m_d3dxvLook; }

	float& GetPitch() { return m_fPitch; }
	float& GetRoll()  { return m_fRoll; }
	float& GetYaw()   { return m_fYaw; }

	void SetOffSet(D3DXVECTOR3 d3dxvOffset)
	{
		m_d3dxvOffset = d3dxvOffset;
		m_d3dxvPosition += d3dxvOffset;
	}
	D3DXVECTOR3& GetOffset() { return m_d3dxvOffset; }

	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	float GetTimeLag() { return m_fTimeLag; }

	//ī�޶� d3dxvShift��ŭ�̵��ϴ� ���� �Լ��̴�.
	virtual void Move(const D3DXVECTOR3& d3dxvShift) { m_d3dxvPosition += d3dxvShift;  }

	// zī�޶� x�� y�� z������ ȸ���ϴ� �����Լ��̴�.
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) {}
	
	// ī�޶��� �̵� ȸ���� ���� ī�޶��� ������ �����ϴ� �����Լ��̴�
	virtual void Update(float fTimeElapsed) {}
	
	// 3��Ī ī�޶󿡼� ī�޶� �ٶ󺸴� ������ �����ϴ� �����Լ��̴�.
	// �Ϲ������� �÷��̾ �ٶ󺸵��� �����Ѵ�.
	virtual void SetLookAt(D3DXVECTOR3& vLookAt) {} 
	
	void GenerateViewMatrix();
	void RegenerateViewMatrix(); 

};


class CFirstPersonCamera : public CCamera
{
public:
	CFirstPersonCamera(CCamera* pCamera);
	
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);

};

class CThirdPersonCamera : public CCamera
{
public:
	CThirdPersonCamera(CCamera* pCamera);
	
	virtual void Update(float fTimeScale);
	virtual void SetLookAt(D3DXVECTOR3& vLookAt);
	
};
