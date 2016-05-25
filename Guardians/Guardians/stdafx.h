// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

#define FRAME_BUFFER_WIDTH	800		
#define FRAME_BUFFER_HEIGHT	600

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <WinSock2.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <D3DX9math.h>
#include <D3Dcompiler.h>				// ���̴� ������ �Լ��� ���
#include <D3DX10Math.h>					// Direct3D ���� �Լ��� ����ϱ� ���� �������
#include <d3d9types.h>
//#include <D3DX10math.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
using namespace DirectX;

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <functional>
#include <vector>
#include <list>
#include <assert.h>
using namespace std;
// C ��Ÿ�� ��� �����Դϴ�.

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <math.h>

#include <Mmsystem.h>

//

#include "stdio.h"
// ī�޶� ��ȯ��İ� ���� ��ȯ ��� �׸��� ���� ��ȯ ����� ���� ��� ���۸� ������ ���� ��ȣ
#define VS_SLOT_CAMERA						0x00
#define VS_SLOT_WORLD_MATRIX				0x01
#define VS_SLOT_BONE_TRANSFORMS				0x02

#define PS_SLOT_LIGHT						0x00
#define PS_SLOT_MATERIAL					0x01

#define PS_SLOT_TEXTURE						0x00
#define PS_SLOT_SAMPLER_STATE				0x00

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

#include "Enum.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)       ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)