#pragma once

#include "GameObject.h"

class CDiffusedShader;
class CMonster : public CGameObject
{
protected:
	bool m_bAlive;

	//�÷��̾��� ��ġ ����, x-��(Right), y-��(Up), z-��(Look) �����̴�.
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvRight;
	D3DXVECTOR3 m_d3dxvUp;
	D3DXVECTOR3 m_d3dxvLook;
	// �÷��̾ ���� x�� y�� z������ �󸶸�ŭȸ���ߴ°��� ��Ÿ����.
	float m_fPitch;
	float m_fYaw;
	float m_fRoll;

	// �÷��̾��� �̵� �ӵ��� ��Ÿ��.
	D3DXVECTOR3 m_d3dxvVelocity;

	// �÷��̾ �ۿ��ϴ� �߷��� ��Ÿ���� ����
	D3DXVECTOR3 m_d3dxvGravity;

	// xz-��鿡�� (�� ������ ����) �÷��̾��� �̵� �ӷ��� �ִ밪�� ��Ÿ��.
	float m_fMaxVelocityXZ;

	// y �� �������� (�� ������ ����) �÷��̾��� �̵��ӷ��� �ִ밪�� ��Ÿ��
	float m_fMaxVelocityY;
	//�÷��̾ �ۿ��ϴ� �������� ��Ÿ��
	float m_fFriction;

	// �÷��̾��� ��ġ�� �ٲ� ������ ȣ��Ǵ� OnPlayerUpdated() �Լ����� ����ϴ� ������
	LPVOID m_pMonsterUpdatedContext;

	CDiffusedShader*	m_pShader;

	float m_fHeight;

	// Netowrk Variables
	std::queue<D3DXVECTOR3> m_deadReckoningQueue;
public:
	CMonster();
	~CMonster();

	void Initalize();
	void Clear();

	void SetAlive(const bool alive) { m_bAlive = alive; }
	bool GetAlive() const { return m_bAlive; }
	float GetHeight() { return m_fHeight; }

	// �÷��̾��� ���� ī�޶� �����ϰ� ��ȯ�ϴ� ��� �Լ��� �����Ѵ�.
	D3DXVECTOR3 GetPosition() { return(m_d3dxvPosition); }
	D3DXVECTOR3 GetLookVector() { return(m_d3dxvLook); }
	D3DXVECTOR3 GetUpVector() { return(m_d3dxvUp); }
	D3DXVECTOR3 GetRightVector() { return(m_d3dxvRight); }

	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(const D3DXVECTOR3& d3dxvGravity) { m_d3dxvGravity = d3dxvGravity; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(const D3DXVECTOR3& d3dxvVelocity) { m_d3dxvVelocity = d3dxvVelocity; }

	// ������ ��ġ�� d3dxvPosition ��ġ�� ���� d3dxvPosition ���Ϳ��� ���� ������ ��ġ
	// ���͸� ���� ���� ������ ��ġ���� d3dxvPosition ���������� ���� ���Ͱ� �ȴ�.
	// ���� ������ ��ġ���� �� ���� ���� ��ŭ�̵��Ѵ�.
	void SetPosition(const D3DXVECTOR3& d3dxvPosition)
	{
		m_d3dxvPosition = d3dxvPosition;
		//Move((d3dxvPosition - m_d3dxvPosition), false);
	}

	const D3DXVECTOR3& GetVelocity() const { return m_d3dxvVelocity; }
	float GetYaw() const { return m_fYaw; }
	float GetPitch() const { return m_fPitch; }
	float GetRoll() const { return m_fRoll; }

	// ���� �̵� �Լ�
	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move();

	//void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);

	// ���͸� ȸ�� �ϴ� �Լ�
	void Rotate(float x, float y, float z);

	// ������ ��ġ�� ȸ�� ������ ��� �ð��� ���� �����ϴ� �Լ�
	void Update(float fTimeElapsed);

	// ������ ��ġ�� ȸ�������κ��� ���� ��ȯ ����� �����ϴ� �Լ��̴�.
	void RegenerateWorldMatrix();

	// ������ ��ġ�� �ٲ� ������ ȣ��Ǵ� �Լ��� �� �Լ����� ����ϴ� ������ �����Լ�
	virtual void OnMonsterUpdated(float fTimeElpased);
	void SetMonsterUpdatedContext(LPVOID pContext) { m_pMonsterUpdatedContext = pContext; }

	// �÷��̾��� ī�޶� 3��Ī ī�޶��� �� �÷��̾� �޽� ������
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext);



	// Network Functions
	queue<D3DXVECTOR3>& GetDeadReckoningQueue() { return m_deadReckoningQueue; }

	// ������ x,z���� �޾ƿ´�.
	void SetServerPos(float fX, float fZ);
	void SetDir(float fX, float fZ) 
	{
		m_d3dxvLook.x = fX;
		m_d3dxvLook.z = fZ;
	}
};