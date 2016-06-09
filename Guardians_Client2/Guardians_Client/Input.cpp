#include "stdafx.h"
#include "Input.h"

USING(Engine)
IMPLEMENT_SINGLETON(Engine::CInput)

Engine::CInput::CInput(void)
{

}

Engine::CInput::~CInput(void)
{

}

_byte Engine::CInput::GetDIKeyState(BYTE KeyFlag)
{
	return m_byKeyState[KeyFlag];
}

_byte Engine::CInput::GetDIMouseState(MOUSECLICK KeyFlag)
{
	return m_MouseState.rgbButtons[KeyFlag];
}

_long Engine::CInput::GetDIMouseMove(MOUSEMOVE KeyFlag)
{
	return *(((_long*)&m_MouseState) + KeyFlag);
}

void Engine::CInput::Set_InputState(void)
{
	/*ZeroMemory(m_byKeyState, 0, sizeof(_byte) * 256);
	ZeroMemory(m_MouseState, 0, sizeof(DIMOUSESTATE));*/

	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);
}

HRESULT Engine::CInput::Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	HRESULT		hr = NULL;

	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);
	FAILED_CHECK_MSG(hr, L"다이렉트 인풋 생성 실패");

	hr = Init_KeyBoard(hWnd);
	FAILED_CHECK_MSG(hr, L"키보드 생성 실패");

	hr = Init_Mouse(hWnd);
	FAILED_CHECK_MSG(hr, L"키보드 생성 실패");
	return S_OK;
}

HRESULT Engine::CInput::Init_KeyBoard(HWND hWnd)
{
	HRESULT hr = NULL;

	hr = m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL);

	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	FAILED_CHECK(hr);

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	FAILED_CHECK(hr);

	m_pKeyBoard->Acquire();
	return S_OK;
}

HRESULT Engine::CInput::Init_Mouse(HWND hWnd)
{
	HRESULT hr = NULL;
	hr = m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	FAILED_CHECK(hr);

	m_pMouse->SetDataFormat(&c_dfDIMouse);
	FAILED_CHECK(hr);

	m_pMouse->Acquire();
	return S_OK;
}

void Engine::CInput::Release(void)
{
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pInput);

	delete this;
}