#pragma once

#include "GameObject.h"

class CDiffusedShader;
class CBow : public CGameObject
{
protected:
	bool m_bAlive;

	//플레이어의 위치 벡터, x-축(Right), y-축(Up), z-축(Look) 벡터이다.
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvRight;
	D3DXVECTOR3 m_d3dxvUp;
	D3DXVECTOR3 m_d3dxvLook;
	// 플레이어가 로컬 x축 y축 z축으로 얼마만큼회전했는가를 나타낸다.
	float m_fPitch;
	float m_fYaw;
	float m_fRoll;

	// 플레이어의 이동 속도를 나타냄.
	D3DXVECTOR3 m_d3dxvVelocity;

	// 플레이어에 작용하는 중력을 나타내는 벡터
	D3DXVECTOR3 m_d3dxvGravity;

	// xz-평면에서 (한 프레임 동안) 플레이어의 이동 속력의 최대값을 나타냄.
	float m_fMaxVelocityXZ;

	// y 축 방향으로 (한 프레임 동안) 플레이어의 이동속력의 최대값을 나타냄
	float m_fMaxVelocityY;
	//플레이어에 작용하는 마찰력을 나타냄
	float m_fFriction;

	// 플레이어의 위치가 바뀔 때마다 호출되는 OnPlayerUpdated() 함수에서 사용하는 데이터
	LPVOID m_pBowUpdatedContext;

	CDiffusedShader*	m_pShader;

public:
	CBow();
	~CBow();

	void Initalize();
	void Clear();

	void SetAlive(const bool alive) { m_bAlive = alive; }
	bool GetAlive() const { return m_bAlive; }

	// 플레이어의 현재 카메라를 설정하고 반환하는 멤버 함수를 선언한다.
	D3DXVECTOR3 GetPosition() { return(m_d3dxvPosition); }
	D3DXVECTOR3 GetLookVector() { return(m_d3dxvLook); }
	D3DXVECTOR3 GetUpVector() { return(m_d3dxvUp); }
	D3DXVECTOR3 GetRightVector() { return(m_d3dxvRight); }

	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(const D3DXVECTOR3& d3dxvGravity) { m_d3dxvGravity = d3dxvGravity; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(const D3DXVECTOR3& d3dxvVelocity) { m_d3dxvVelocity = d3dxvVelocity; }

	// 몬스터의 위치를 d3dxvPosition 위치로 설정 d3dxvPosition 벡터에서 현재 몬스터의 위치
	// 벡터를 빼면 현재 몬스터의 위치에서 d3dxvPosition 방향으로의 방향 벡터가 된다.
	// 현재 몬스터의 위치에서 이 방향 벡터 만큼이동한다.
	void SetPosition(const D3DXVECTOR3& d3dxvPosition)
	{
		Move((d3dxvPosition - m_d3dxvPosition), false);
	}

	const D3DXVECTOR3& GetVelocity() const { return m_d3dxvVelocity; }
	float GetYaw() const { return m_fYaw; }
	float GetPitch() const { return m_fPitch; }
	float GetRoll() const { return m_fRoll; }

	// 몬스터 이동 함수
	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move(const D3DXVECTOR3& d3dxvShift, bool bVelocity = false);
	//void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);

	// 몬스터를 회전 하는 함수
	void Rotate(float x, float y, float z);

	// 몬스터의 위치와 회전 정보를 경과 시간에 따라 갱신하는 함수
	void Update(float fTimeElapsed);

	// 몬스터의 위치와 회전축으로부터 월드 변환 행렬을 생성하는 함수이다.
	void RegenerateWorldMatrix();

	// 몬스터의 위치가 바뀔 때마다 호출되는 함수와 그 함수에서 사용하는 정보를 설정함수
	virtual void OnMonsterUpdated(float fTimeElpased);
	void SetMonsterUpdatedContext(LPVOID pContext) { m_pBowUpdatedContext = pContext; }

	// 플레이어의 카메라가 3인칭 카메라일 때 플레이어 메쉬 렌더링
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext);
};