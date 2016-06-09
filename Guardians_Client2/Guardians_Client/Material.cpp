#include "stdafx.h"
#include "Material.h"

USING(Engine)

Engine::CMaterial::CMaterial(void)
{

}

Engine::CMaterial::~CMaterial(void)
{

}

HRESULT Engine::CMaterial::Initialize(const D3DMATERIAL9& Material)
{
	memcpy(&m_Material, &Material, sizeof(D3DMATERIAL9));

	return S_OK;
}

CMaterial* Engine::CMaterial::Create(const D3DMATERIAL9& Material)
{
	CMaterial*		pMaterial = new CMaterial;

	if(FAILED(pMaterial->Initialize(Material)))
	{
		MSG_BOX("Material Create Failed");
		Engine::Safe_Release(pMaterial);
	}
	return pMaterial;	
}

_ulong Engine::CMaterial::Release()
{
	_ulong dwRefCnt = Engine::CComponent::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}
	return dwRefCnt;
}