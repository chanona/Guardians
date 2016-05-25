#pragma once

#define EJ_MATH EJMath::GetInstance()

const float EPSILON = 0.001f;

class EJMath
{
public:
	EJMath();
	~EJMath();
	
	static EJMath* GetInstance()
	{
		static EJMath instance;
		return &instance;
	}

	// 동등 함수
	bool Vector3Equal(D3DXVECTOR3& v1, D3DXVECTOR3& v2);
	bool Equals(float lhs, float rhs)
	{
		// 만약 lhs == rhs라면 두 수의 차이는 0이어야 한다.
		return fabs(lhs - rhs) < EPSILON ? true : false;
	}
	// 
	D3DXVECTOR3* SmoothStep(D3DXVECTOR3& V0, D3DXVECTOR3& V1, float t);
private :

};

