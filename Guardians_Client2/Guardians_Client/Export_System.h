/*!
 * \file Export_System.h
 * \date 2015/04/02 19:19
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
#ifndef Export_System_h__
#define Export_System_h__

#include "TimeMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"
#include "GraphicDev.h"
#include "Input.h"

BEGIN(Engine)

//For.Getter------------------------------------------------------------------------------------------------------------
inline LPDIRECT3DDEVICE9 Get_GraphicDev(void);
inline _float Get_TimeDelta(const TCHAR*);
inline _bool Get_Activate(const TCHAR* pFrameTag, const _float& fTimeDelta);
inline _byte GetDIKeyState(BYTE KeyFlag);
inline _byte GetDIMouseState(CInput::MOUSECLICK KeyFlag);
inline _long GetDIMouseMove(CInput::MOUSEMOVE KeyFlag);

//For.Setter------------------------------------------------------------------------------------------------------------
inline void Set_TimeDelta(const TCHAR*);
inline void Set_InputState(void); 

//For.Generic------------------------------------------------------------------------------------------------------------
inline HRESULT Ready_GraphicDev(Engine::CGraphicDev::WINMODE Mode, HWND hWnd, const _ushort& wSizeX, const _ushort& wSizeY);
inline HRESULT Ready_InputDevice(HINSTANCE hInst, HWND hWnd);
inline HRESULT Ready_Timer(const TCHAR* pTimerTag);
inline HRESULT Ready_Frame(const TCHAR* pFrameTag, const _float& fFps);
inline HRESULT Ready_Font(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFontKey, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);

inline void Render_Font(const TCHAR* pFontKey, const D3DXVECTOR3& vPos, const TCHAR* pString, D3DXCOLOR Color);


//For.Release------------------------------------------------------------------------------------------------------------
inline void Release_System(void);

#include "Export_System.inl"

END

#endif // Export_System_h__