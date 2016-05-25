#pragma once

#include "Camera.h"
#include "GameObject.h"
#include "GameTimer.h"
#include "Shader.h"

class CPlayerShader;
class CPlayer : public CGameObject
{
protected:
	//�÷��̾��� ��ġ ����, x-��(Right), y-��(Up), z-��(Look) �����̴�.
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvRight;
	D3DXVECTOR3 m_d3dxvUp;
	D3DXVECTOR3 m_d3dxvLook;

	//D3DXVECTOR3 m_vDirection;

	// �÷��̾ ���� x�� y�� z������ �󸶸�ŭȸ���ߴ°��� ��Ÿ����.
	float m_fPitch;
	float m_fYaw;
	float m_fRoll;

	// �÷��̾��� �̵� �ӵ��� ��Ÿ��.
	BYTE m_direction;
	float m_speed;

	// �÷��̾ �ۿ��ϴ� �߷��� ��Ÿ���� ����
	//D3DXVECTOR3 m_d3dxvGravity;

	// xz-��鿡�� (�� ������ ����) �÷��̾��� �̵� �ӷ��� �ִ밪�� ��Ÿ��.
	//float m_fMaxVelocityXZ;

	// y �� �������� (�� ������ ����) �÷��̾��� �̵��ӷ��� �ִ밪�� ��Ÿ��
	//float m_fMaxVelocityY;
	//�÷��̾ �ۿ��ϴ� �������� ��Ÿ��
	//float m_fFriction;

	// �÷��̾��� ��ġ�� �ٲ� ������ ȣ��Ǵ� OnPlayerUpdated() �Լ����� ����ϴ� ������
	LPVOID m_pPlayerUpdatedContext;
	// ī�޶��� ��ġ�� �ٲ� �븶�� ȣ��Ǵ� OnCameraUpdated() �Լ����� ����ϴ� ������
	LPVOID m_pCameraUpdatedContext;
	// �÷��̾��� ���� ī�޶�.
	CCamera* m_pCamera;
	// 3��Ī ī�޶��� �� �÷��̾ �׸��� ���� ����ϴ� ���̴�
	CPlayerShader *m_pShader;
	
	float m_fHeight;

	// Network Variables
	bool m_bConnected;
	std::queue<D3DXVECTOR3> m_deadReckoningQueue;
public:
	CPlayer();
	~CPlayer();

	void Initalize();

	void SetDirection(const BYTE dir) { m_direction = dir; }
	void SetSpeed(const float speed) { m_speed = speed; }
	void SetConnected(const bool connect) { m_bConnected = connect; }
	bool IsConnected() const { return m_bConnected; }

	// �÷��̾��� ���� ī�޶� �����ϰ� ��ȯ�ϴ� ��� �Լ��� �����Ѵ�.
	BYTE GetDirection() const { return m_direction; }
	float GetSpeed() const { return m_speed; }
	float GetPositionX() const { return m_d3dxvPosition.x; }
	float GetPositionY() const { return m_d3dxvPosition.y; }
	float GetPositionZ() const { return m_d3dxvPosition.z; }
	D3DXVECTOR3 GetPosition() { return(m_d3dxvPosition); }
	D3DXVECTOR3 GetLookVector() { return(m_d3dxvLook); }
	D3DXVECTOR3 GetUpVector() { return(m_d3dxvUp); }
	D3DXVECTOR3 GetRightVector() { return(m_d3dxvRight); }

	float GetHeight() { return m_fHeight; }

	/*void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(const D3DXVECTOR3& d3dxvGravity) { m_d3dxvGravity = d3dxvGravity; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(const D3DXVECTOR3& d3dxvVelocity) { m_d3dxvVelocity = d3dxvVelocity; }
*/
	// �÷��̾��� ��ġ�� d3dxvPosition ��ġ�� ���� d3dxvPosition ���Ϳ��� ���� �÷��̾��� ��ġ
	// ���͸� ���� ���� �÷��̾��� ��ġ���� d3dxvPosition ���������� ���� ���Ͱ� �ȴ�.
	// ���� �÷��̾��� ��ġ���� �� ���� ���� ��ŭ�̵��Ѵ�.
	void SetPosition(const D3DXVECTOR3& d3dxvPosition)
	{
		m_d3dxvPosition = d3dxvPosition;
	}

	//const D3DXVECTOR3& GetVelocity() const { return m_d3dxvVelocity; }
	float GetYaw() const { return m_fYaw; }
	float GetPitch() const { return m_fPitch; }
	float GetRoll() const { return m_fRoll; }

	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	CCamera* GetCamera() { return(m_pCamera); }

	// �÷��̾� �̵� �Լ�
	void Move(BYTE direction);
	void Move(const D3DXVECTOR3& d3dxvShift);
	void OtherMove();
	//void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);

	// �÷��̾ ȸ�� �ϴ� �Լ�
	void Rotate(float x, float y, float z);

	// �÷��̾��� ��ġ�� ȸ�� ������ ��� �ð��� ���� �����ϴ� �Լ�
	void Update(float fTimeElapsed);

	// �÷��̾��� ��ġ�� ȸ�������κ��� ���� ��ȯ ����� �����ϴ� �Լ��̴�.
	void RegenerateWorldMatrix();

	// �÷��̾��� ��ġ�� �ٲ� ������ ȣ��Ǵ� �Լ��� �� �Լ����� ����ϴ� ������ �����Լ�
	virtual void OnPlayerUpdated(float fTimeElpased);
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }

	// ī�޶��� ��ġ�� �ٲ� ������ ȣ��Ǵ� �Լ��� �� �Լ����� ����ϴ� ������ ����
	virtual void OnCameraUpdated(float fTimeElapsed);
	void SetCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }

	// �÷��̾��� ��� ���۸� �����ϰ� �����ϴ� ��� �Լ��� �����Ѵ�.
	void CreateShaderVariables(ID3D11Device* pd3dDevice);
	void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext);

	CCamera* OnChangeCamera(ID3D11Device* pd3dDevice, DWORD nNewCameraMode, DWORD nCurrentCameraMode);
	virtual void ChangeCamera(ID3D11Device* pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed);

	// �÷��̾��� ī�޶� 3��Ī ī�޶��� �� �÷��̾� �޽� ������
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext);
	
	// Network Function
	queue<D3DXVECTOR3>& GetDeadReckoningQueue() { return m_deadReckoningQueue; }

	friend ostream& operator << (ostream& os, CPlayer* pPlayer)
	{
		os << "id : " << pPlayer->GetID() << " " <<  "x : " << pPlayer->GetPositionX() << " " << 
			"y : " << pPlayer->GetPositionY() << " " << "z : " << pPlayer->GetPositionZ() << endl;
		return os;
	}
};