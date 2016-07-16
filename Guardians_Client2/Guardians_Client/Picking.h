#pragma once

#ifndef PICKING_H
#define PICKING_H

#include "Defines.h"

class CPicking
{
public:
	CPicking(LPDIRECT3DDEVICE9 device);
	Ray CalcPickingRay(int x, int y);
	bool PickingTest(Ray* ray, BoundingSphere* sphere);

private:
	void TransformRay(Ray* ray, D3DXMATRIX* T);

	LPDIRECT3DDEVICE9 m_pGraphicDev;

};

#endif