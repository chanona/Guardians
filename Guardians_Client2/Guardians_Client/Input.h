/*!
 * \file Input.h
 * \date 2015/08/18 20:21
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Input_h__
#define Input_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class  CInput
{
public:
	DECLARE_SINGLETON(CInput)
public:
	enum MOUSECLICK {DIM_LBUTTON, DIM_RBUTTON, DIM_MBUTTON, DIM_XBUTTON};
	enum MOUSEMOVE {DIM_X, DIM_Y, DIM_Z};
private:
	explicit CInput(void);
	~CInput(void);
public:
	_byte GetDIKeyState(BYTE KeyFlag);
	_byte GetDIMouseState(MOUSECLICK KeyFlag);
	_long GetDIMouseMove(MOUSEMOVE KeyFlag);
public:
	void Set_InputState(void); // 입력장치의 상태를 받아온다.


public:
	HRESULT Ready_InputDevice(HINSTANCE hInst, HWND hWnd);

private:
	HRESULT Init_KeyBoard(HWND hWnd);
	HRESULT Init_Mouse(HWND hWnd);
private:
	LPDIRECTINPUT8			m_pInput; // 장치객체를 초기화하기위한 사전객체.
	LPDIRECTINPUTDEVICE8	m_pKeyBoard; // 키보드를 대표하는 객체다.
	LPDIRECTINPUTDEVICE8	m_pMouse; // 마우스를 대표하는 객체다.
private:
	_byte			m_byKeyState[256];
	DIMOUSESTATE	m_MouseState;
public:
	void Release(void);
};

END

#endif // Input_h__