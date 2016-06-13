#include "stdafx.h"
#include "PlayerState.h"
#include "Export_Function.h"

CPlayerState::CPlayerState(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CPlayerState::~CPlayerState(void)
{
}

HRESULT CPlayerState::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_UI");
	if (NULL == m_pEffect)
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pMtrlCom = nullptr;

	m_fSizeX[STATE_STATE] = 300.f;
	m_fSizeY[STATE_STATE] = 150.f;
	m_fX[STATE_STATE] = 650.f;
	m_fY[STATE_STATE] = 80.f;

	m_fSizeX[STATE_FACE] = 150.f;
	m_fSizeY[STATE_FACE] = 150.f;
	m_fX[STATE_FACE] = 725.f;
	m_fY[STATE_FACE] = 80.f;

	m_fSizeX[STATE_HP] = 130.f;
	m_fSizeY[STATE_HP] = 50.f;
	m_fX[STATE_HP] = 618.f;
	m_fY[STATE_HP] = 83.f;

	m_fSizeX[STATE_MP] = 150.f;
	m_fSizeY[STATE_MP] = 50.f;
	m_fX[STATE_MP] = 626.f;
	m_fY[STATE_MP] = 100.f;

	m_fHp = 100.f;
	m_fMp = 100.f;
	m_fMaxHp = 100.f;
	m_fMaxMp = 100.f;

	fPlayerHp = 100.f;
	fPlayerMp = 100.f;

	for (int i = 0; i < STATE_END; ++i)
	{
		m_pVertex[i] = new Engine::VTXTEX[4];
		m_pBufferCom[i]->Get_VtxInfo(m_pVertex[i]);
	}

	return S_OK;
}

HRESULT CPlayerState::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[STATE_FACE] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerFace");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_PlayerFace", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[STATE_HP] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerHp");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_PlayerHp", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[STATE_MP] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerMp");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_PlayerMp", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[STATE_STATE] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerState");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_PlayerState", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom[STATE_FACE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerFace");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_PlayerFace", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom[STATE_HP] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerHp");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_PlayerHp", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom[STATE_MP] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerMp");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_PlayerMp", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom[STATE_STATE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerState");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_PlayerState", pComponent));

	return S_OK;
}

_int CPlayerState::Update(const _float& fTimeDelta)
{
	if (Engine::GetDIKeyState(DIK_1) & 0x80)
	{
		m_fHp -= fTimeDelta * 10;

		int i = 0;
	}

	//// HP
	//if (m_fHp != fPlayerHp)
	//{
	//	m_fHp += (fPlayerHp - m_fHp) * fTimeDelta * 10;

	//	if (abs(m_fHp - fPlayerHp) < 1.f)
	//		m_fHp = fPlayerHp;
	//}

	//if (m_fMp != fPlayerMp)
	//{
	//	m_fMp += (fPlayerMp - m_fMp) * fTimeDelta * 10;

	//	if (abs(m_fMp - fPlayerMp) < 1.f)
	//		m_fMp = fPlayerMp;
	//}

	//HP
	m_pVertex[STATE_HP][2].vTexUV.y = 0.5f;
	m_pVertex[STATE_HP][3].vTexUV.y = 0.5f;

	m_pVertex[STATE_HP][1].vPos.x = -0.5f + (m_fHp / m_fMaxHp);
	m_pVertex[STATE_HP][2].vPos.x = -0.5f + (m_fHp / m_fMaxHp);

	m_pVertex[STATE_HP][1].vTexUV.x = (m_fHp / m_fMaxHp);
	m_pVertex[STATE_HP][2].vTexUV.x = (m_fHp / m_fMaxHp);

	m_pBufferCom[STATE_HP]->Set_VtxInfo(m_pVertex[STATE_HP]);

	//MP
	m_pVertex[STATE_MP][2].vTexUV.y = 0.5f;
	m_pVertex[STATE_MP][3].vTexUV.y = 0.5f;

	m_pVertex[STATE_MP][1].vPos.x = -0.5f + (m_fMp / m_fMaxMp);
	m_pVertex[STATE_MP][2].vPos.x = -0.5f + (m_fMp / m_fMaxMp);

	m_pVertex[STATE_MP][1].vTexUV.x = (m_fMp / m_fMaxMp);
	m_pVertex[STATE_MP][2].vTexUV.x = (m_fMp / m_fMaxMp);

	m_pBufferCom[STATE_MP]->Set_VtxInfo(m_pVertex[STATE_MP]);

	Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	/*for (int i = 0; i < STATE_END; ++i)
	{
		m_pBufferCom[i]->Set_VtxInfo(m_pVertex[i]);
	}*/
	
	// 직교투영행렬을 만들어주는 기능의 함수다.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);

	return 0;
}

void CPlayerState::Render(void)
{
	if (NULL == m_pEffect)
		return;
	
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	D3DXMatrixIdentity(&m_matView[STATE_STATE]);
	m_matView[STATE_STATE]._11 = m_fSizeX[STATE_STATE];
	m_matView[STATE_STATE]._22 = m_fSizeY[STATE_STATE];
	m_matView[STATE_STATE]._33 = 1.f;
	m_matView[STATE_STATE]._41 = WINSIZEX * 0.5f - m_fX[STATE_STATE];
	m_matView[STATE_STATE]._42 = WINSIZEY * 0.5f - m_fY[STATE_STATE];

	D3DXMatrixIdentity(&m_matView[STATE_FACE]);
	m_matView[STATE_FACE]._11 = m_fSizeX[STATE_FACE];
	m_matView[STATE_FACE]._22 = m_fSizeY[STATE_FACE];
	m_matView[STATE_FACE]._33 = 1.f;
	m_matView[STATE_FACE]._41 = WINSIZEX * 0.5f - m_fX[STATE_FACE];
	m_matView[STATE_FACE]._42 = WINSIZEY * 0.5f - m_fY[STATE_FACE];

	D3DXMatrixIdentity(&m_matView[STATE_HP]);
	m_matView[STATE_HP]._11 = m_fSizeX[STATE_HP];
	m_matView[STATE_HP]._22 = m_fSizeY[STATE_HP];
	m_matView[STATE_HP]._33 = 1.f;
	m_matView[STATE_HP]._41 = WINSIZEX * 0.5f - m_fX[STATE_HP];
	m_matView[STATE_HP]._42 = WINSIZEY * 0.5f - m_fY[STATE_HP];

	D3DXMatrixIdentity(&m_matView[STATE_MP]);
	m_matView[STATE_MP]._11 = m_fSizeX[STATE_MP];
	m_matView[STATE_MP]._22 = m_fSizeY[STATE_MP];
	m_matView[STATE_MP]._33 = 1.f;
	m_matView[STATE_MP]._41 = WINSIZEX * 0.5f - m_fX[STATE_MP];
	m_matView[STATE_MP]._42 = WINSIZEY * 0.5f - m_fY[STATE_MP];

	for (int i = 0; i < STATE_END; ++i)
	{
		m_pEffect->SetMatrix("g_matView", &m_matView[i]);
		m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

		m_pTextureCom[i]->Set_ConstantTable(m_pEffect, "g_BaseTexture");

		// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.

		m_pEffect->Begin(NULL, 0);

		m_pEffect->BeginPass(0);

		m_pBufferCom[i]->Render(NULL);

		m_pEffect->EndPass();

		m_pEffect->End();

		m_pEffect->SetTexture("g_BaseTexture", NULL);
	}
}

CPlayerState* CPlayerState::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerState*		pGameObject = new CPlayerState(pGraphicDev);

	if (FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("pGameObject Created Failed");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CPlayerState::Set_ContantTable(void)
{
	
}

_ulong CPlayerState::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if (0 == dwRefCnt)
	{
		for (int i = 0; i < STATE_END; ++i)
		{
			::Safe_Release(m_pBufferCom[i]);
			::Safe_Delete_Array(m_pVertex[i]);
		}
		delete this;
	}

	return dwRefCnt;
}

