// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
// Windows ��� ����:
#include <windows.h>
#include <WinSock2.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <cassert>
// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include <list>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
using namespace std;


#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "winmm.lib")