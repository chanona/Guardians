#include "stdafx.h"
#include "Picking.h"

CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
}

Ray CPicking::CalcPickingRay(int x, int y)
{
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	D3DXMATRIX proj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &proj);

	px = (((2.0f * x) / vp.Width) - 1.0f) / proj(0, 0);
	py = (((-2.0f * y) / vp.Height) + 1.0f) / proj(1, 1);

	Ray ray;
	ray.origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray.direction = D3DXVECTOR3(px, py, 1.0f);

	//transform the ray to world space
	D3DXMATRIX view;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &view);

	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, 0, &view);

	TransformRay(&ray, &viewInverse);

	return ray;
}

void CPicking::TransformRay(Ray* ray, D3DXMATRIX* T)
{
	//transform the ray's origin, w=1
	D3DXVec3TransformCoord(
		&ray->origin,
		&ray->direction,
		T);

	//transform the ray's direction, w=0
	D3DXVec3TransformNormal(
		&ray->direction,
		&ray->direction,
		T);

	//normalize the direction
	D3DXVec3Normalize(&ray->direction, &ray->direction);
}

bool CPicking::PickingTest(Ray* ray, BoundingSphere* sphere)
{
	D3DXVECTOR3 v = ray->origin - sphere->center;
	float b = 2.0f * D3DXVec3Dot(&ray->direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (sphere->radius * sphere->radius);

	//find the discriminant
	float discriminant = (b * b) - (4.0f * c);

	//test for imaginary number
	if (discriminant < 0.0f)
		return false;

	discriminant = sqrtf(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;

	//if a solution is >=0, then we interesected the sphere
	if (s0 >= 0.0f || s1 >= 0.0f)
		return true;

	return false;
}