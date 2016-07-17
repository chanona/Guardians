#ifndef Defines_h__
#define Defines_h__

#include "stdafx.h"
using namespace std;

#include "Typedef.h"
#include "Contant.h"
#include "Macro.h"
#include "Function.h"
#include "Enum.h"

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;
extern int g_iNum;
extern int g_iGoal;

//structs
struct Ray
{
	D3DXVECTOR3 origin;
	D3DXVECTOR3 direction;
};

struct BoundingSphere
{
	//BoundingSphere();

	D3DXVECTOR3 center;
	float radius;
};

#endif // Defines_h__
