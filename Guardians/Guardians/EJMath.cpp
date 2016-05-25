#include "stdafx.h"
#include "EJMath.h"


EJMath::EJMath()
{
}


EJMath::~EJMath()
{
}

bool EJMath::Vector3Equal(D3DXVECTOR3 & v1, D3DXVECTOR3 & v2)
{
	if (!Equals(v1.x, v2.x)) return false;
	if (!Equals(v1.y, v2.y)) return false;
	if (!Equals(v1.z, v2.z)) return false;

	return true;
}

D3DXVECTOR3* EJMath::SmoothStep(D3DXVECTOR3 & V0, D3DXVECTOR3 & V1, float t)
{
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t*t*(3.f - 2.f*t);

	return D3DXVec3Lerp(&V0, &V0, &V1, t);
}
