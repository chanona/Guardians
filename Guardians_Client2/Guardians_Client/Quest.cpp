#include "stdafx.h"
#include "Quest.h"
#include "ClientNetEngine.h"
#include "Export_Function.h"

CQuest::CQuest(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CQuest::~CQuest(void)
{
}

HRESULT CQuest::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_UI");
	if (NULL == m_pEffect)
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pMtrlCom = nullptr;

	m_eQuest = QUEST_START;
	m_bQuest = false;

	m_fSizeX[QUEST_START] = 300.f;
	m_fSizeY[QUEST_START] = 500.f;
	m_fX[QUEST_START] = 200.f;
	m_fY[QUEST_START] = 250.f;

	m_fSizeX[QUEST_ING] = 300.f;
	m_fSizeY[QUEST_ING] = 500.f;
	m_fX[QUEST_ING] = 200.f;
	m_fY[QUEST_ING] = 250.f;

	m_fSizeX[QUEST_OK] = 300.f;
	m_fSizeY[QUEST_OK] = 500.f;
	m_fX[QUEST_OK] = 200.f;
	m_fY[QUEST_OK] = 250.f;

	for (int i = 0; i < QUEST_END; ++i)
	{
		m_pVertex[i] = new Engine::VTXTEX[4];
		m_pBufferCom[i]->Get_VtxInfo(m_pVertex[i]);
	}

	return S_OK;
}

HRESULT CQuest::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[QUEST_START] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_QuestStart");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Quest1", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[QUEST_ING] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_QuestIng");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Quest2", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[QUEST_OK] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_QuestOk");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Quest3", pComponent));
		
	// For.Texture Component
	pComponent = m_pTextureCom[QUEST_START] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_QuestStart");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Quest1", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom[QUEST_ING] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_QuestIng");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Quest2", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom[QUEST_OK] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_QuestOk");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Quest3", pComponent));
		
	return S_OK;
}

_int CQuest::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	/*for (int i = 0; i < STATE_END; ++i)
	{
	m_pBufferCom[i]->Set_VtxInfo(m_pVertex[i]);
	}*/

	if (m_bQuest == true)
	{
		if (Engine::GetDIKeyState(DIK_ESCAPE))
		{
			m_bQuest = false;
		}
	}

	// 직교투영행렬을 만들어주는 기능의 함수다.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);

	return 0;
}

void CQuest::Render(void)
{
	if (NULL == m_pEffect)
		return;

	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	D3DXMatrixIdentity(&m_matView[QUEST_START]);
	m_matView[QUEST_START]._11 = m_fSizeX[QUEST_START];
	m_matView[QUEST_START]._22 = m_fSizeY[QUEST_START];
	m_matView[QUEST_START]._33 = 1.f;
	m_matView[QUEST_START]._41 = WINSIZEX * 0.5f - m_fX[QUEST_START];
	m_matView[QUEST_START]._42 = WINSIZEY * 0.5f - m_fY[QUEST_START];

	D3DXMatrixIdentity(&m_matView[QUEST_ING]);
	m_matView[QUEST_ING]._11 = m_fSizeX[QUEST_ING];
	m_matView[QUEST_ING]._22 = m_fSizeY[QUEST_ING];
	m_matView[QUEST_ING]._33 = 1.f;
	m_matView[QUEST_ING]._41 = WINSIZEX * 0.5f - m_fX[QUEST_ING];
	m_matView[QUEST_ING]._42 = WINSIZEY * 0.5f - m_fY[QUEST_ING];

	D3DXMatrixIdentity(&m_matView[QUEST_OK]);
	m_matView[QUEST_OK]._11 = m_fSizeX[QUEST_OK];
	m_matView[QUEST_OK]._22 = m_fSizeY[QUEST_OK];
	m_matView[QUEST_OK]._33 = 1.f;
	m_matView[QUEST_OK]._41 = WINSIZEX * 0.5f - m_fX[QUEST_OK];
	m_matView[QUEST_OK]._42 = WINSIZEY * 0.5f - m_fY[QUEST_OK];
	
	m_pEffect->SetMatrix("g_matView", &m_matView[m_eQuest]);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

	m_pTextureCom[m_eQuest]->Set_ConstantTable(m_pEffect, "g_BaseTexture");

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.

	if (m_bQuest == true)
	{
		m_pEffect->Begin(NULL, 0);

		m_pEffect->BeginPass(0);

		m_pBufferCom[m_eQuest]->Render(NULL);

		m_pEffect->EndPass();

		m_pEffect->End();

		m_pEffect->SetTexture("g_BaseTexture", NULL);
	}
}

CQuest* CQuest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CQuest*		pGameObject = new CQuest(pGraphicDev);

	if (FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("pGameObject Created Failed");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CQuest::Set_ContantTable(void)
{

}

_ulong CQuest::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if (0 == dwRefCnt)
	{
		for (int i = 0; i < QUEST_END; ++i)
		{
			::Safe_Delete_Array(m_pVertex[i]);
		}
		delete this;
	}

	return dwRefCnt;
}

