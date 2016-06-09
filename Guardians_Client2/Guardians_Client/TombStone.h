#ifndef TombStone_h__
#define TombStone_h__

#include "Defines.h"
#include "LandObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
}

class CTombStone	: public CLandObject
{
private:
	explicit CTombStone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTombStone(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CTombStone* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:		
	Engine::CStaticMesh*		m_pMeshCom;
private:
	_bool						m_bColl;
private:
	LPD3DXEFFECT				m_pEffect;

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);


};


#endif // TombStone_h__
