#include "stdafx.h"
#include "Player.h"
#include "Npc.h"
#include "Export_Function.h"
#include "Arrow.h"
#include "ClientNetEngine.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
: CLandObject(pGraphicDev)
, m_pEffect(NULL)
, m_fTimeDelta(0.f)
, m_bPush(false)
, m_iAniIdx(PLAYER_STAND)
, m_pMouseCol(nullptr)
, m_bMove(false)
, m_pMonster(NULL)
, m_bConnected(false)
, m_iQuestMonCnt(0)
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Initialize(void)
{
	if(FAILED(CLandObject::Initialize()))
		return E_FAIL;

	m_pEffect = Engine::Get_EffectHandle(L"Shader_Meshes");
	if(NULL == m_pEffect)
		return E_FAIL;	

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pMeshCom->Set_AnimationSet(m_iAniIdx);

	m_pTransCom->m_vScale = _vec3(0.01f, 0.01f, 0.01f);

	m_pTransCom->m_vPosition = _vec3(320.0f, 0.f, 320.0f);

	return S_OK;
}

HRESULT CPlayer::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Player");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

		
	return S_OK;
}

_int CPlayer::Update(const _float& fTimeDelta)
{
	if (m_bConnected == false) return -1;

	m_fTimeDelta = fTimeDelta;

	if(m_id == NETWORK_ENGINE->GetID()) Check_KeyState(fTimeDelta);

	if (m_bMove)
	{
		if(m_pMonster)
			MoveToMonster(fTimeDelta);
		else
			Move(fTimeDelta);
	}

	if (m_pMonster)
	{
		Engine::CComponent*	pMeshCom = m_pMonster->Get_Component(L"Com_Mesh");

		if (((Engine::CDynamicMesh*)pMeshCom)->Get_AnimationSet() == SALA_DIE)
		{
			m_pMonster = NULL;
			m_pMeshCom->Set_AnimationSet(PLAYER_STAND);
		}
	}
		
	if ((m_pMeshCom->Check_EndPeriod()) && !m_bMove)
	{
		if (m_pMeshCom->Get_AnimationSet() == PLAYER_ATTACK)
		{
			//화살 생성
			CGameObject* pArrow = CArrow::Create(m_pGraphicDev);
			((CArrow*)pArrow)->Set_Position(m_pTransCom->m_vPosition + D3DXVECTOR3(0.f, 2.f, 0.f));
			((CArrow*)pArrow)->Set_Monster(m_pMonster);
			Engine::Add_Object(L"GameLogic", L"Arrow", pArrow);
		}
		else
			m_pMeshCom->Set_AnimationSet(PLAYER_STAND);
	}	

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	CLandObject::Update(fTimeDelta);

	return 0;
}

void CPlayer::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ContantTable();

	m_pMeshCom->Move_Frame(m_fTimeDelta);

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
#ifdef _DEBUG
	_matrix			matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	_matrix			matTrans;
	D3DXMatrixIdentity(&matTrans);
	memcpy(&matTrans.m[3][0], &m_pTransCom->m_matWorld.m[3][0], sizeof(_vec3));

	_matrix			matWorld;
	matWorld = matScale * matTrans;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
#endif	

	m_pMeshCom->Render_MeshForShader(m_pEffect, true);
}

void CPlayer::Clear()
{
}

void CPlayer::MoveToMonster(const _float& fTimeDelta)
{
	Engine::CComponent* pTransCom = m_pMonster->Get_Component(L"Com_Transform");
	
	D3DXVECTOR3 vPos = ((Engine::CTransform*)pTransCom)->m_vPosition;

	D3DXVECTOR3		vDir = vPos - m_pTransCom->m_vPosition;

	float		fDistance = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransCom->m_vPosition += vDir * 3.f * fTimeDelta;

	// 몬스터와의 거리가 가까울때 공격 모션으로 상태변화
	if (fDistance < 10.f)
	{
		m_pMeshCom->Set_AnimationSet(PLAYER_ATTACK);
		m_bMove = false;
	}
}

void CPlayer::Move(const _float& fTimeDelta)
{
	_vec3		vDir = m_vDestPos - m_pTransCom->m_vPosition;

	_float		fDistance = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	_float fCos = D3DXVec3Dot(&vDir, &D3DXVECTOR3(0.f, 0.f, -1.f));

	_float fAngle = acosf(fCos);
	if (vDir.x > 0.f)
		fAngle = 2 * D3DX_PI - fAngle;

	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = fAngle;

	m_pTransCom->m_vPosition += vDir * 3.f * fTimeDelta;

	if (fDistance < 1.f)
	{
		m_pMeshCom->Set_AnimationSet(0);
		m_bMove = false;
	}	
}

void CPlayer::SetPush(int iIndex)
{
	/*if (true == m_bPush)
		return;

	m_bPush = true;

	m_pMeshCom->Set_AnimationSet(iIndex);

	if (m_iAniIdx > PLAYER_END)
		m_iAniIdx = PLAYER_STAND;*/
}

void CPlayer::SetAngle(float fAngle, Engine::CTransform::ANGLE eAngle)
{
	m_pTransCom->m_fAngle[eAngle] = fAngle;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*		pGameObject = new CPlayer(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CPlayer Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CPlayer::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);	

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)
		return;

	m_pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	m_pEffect->SetVector("g_vLightDiffuse", &_vec4((_float*)&pLightInfo->Diffuse));
	m_pEffect->SetVector("g_vLightAmbient", &_vec4((_float*)&pLightInfo->Ambient));	
}

void CPlayer::Check_KeyState(const _float& fTimeDelta)
{
	if (GetActiveWindow() != g_hWnd) return;

	bool bInput = false;

	if (Engine::GetDIKeyState(DIK_W) & 0x80)
	{
		_vec3			vDirection(0.f, 0.f, 0.f);

		memcpy(&vDirection, &m_pTransCom->m_matWorld.m[2][0], sizeof(_vec3));
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransCom->m_vPosition += vDirection * -1.f * 3.0f * fTimeDelta;

		m_pMeshCom->Set_AnimationSet(PLAYER_WALK);
		////SetPush(PLAYER_WALK);
		m_bMove = false;
		bInput = true;
		m_pMonster = NULL;

		cs_packet_move_forward pkt;
		pkt.size = sizeof(pkt);
		pkt.type = CS_FORWARD;
		pkt.x = m_pTransCom->m_vPosition.x;
		pkt.y = m_pTransCom->m_vPosition.y;
		pkt.z = m_pTransCom->m_vPosition.z;
		pkt.radian = m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y];
		NETWORK_ENGINE->SendPacket((char *)&pkt);
	}

	if (Engine::GetDIKeyState(DIK_S) & 0x80)
	{
		_vec3			vDirection(0.f, 0.f, 0.f);

		memcpy(&vDirection, &m_pTransCom->m_matWorld.m[2][0], sizeof(_vec3));
		D3DXVec3Normalize(&vDirection, &vDirection);
		m_pTransCom->m_vPosition -= vDirection * -1.f * 3.0f * fTimeDelta;

		m_pMeshCom->Set_AnimationSet(PLAYER_WALK);
		m_bMove = false;
		bInput = true;
		m_pMonster = NULL;

		cs_packet_move_backward pkt;
		pkt.size = sizeof(pkt);
		pkt.type = CS_BACKWARD;
		pkt.x = m_pTransCom->m_vPosition.x;
		pkt.y = m_pTransCom->m_vPosition.y;
		pkt.z = m_pTransCom->m_vPosition.z;
		pkt.radian = m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y];
		NETWORK_ENGINE->SendPacket((char *)&pkt);
	}

	if (!bInput && !m_bMove && !m_pMonster)
	{
		m_pMeshCom->Set_AnimationSet(PLAYER_STAND);
	}

	if (Engine::GetDIKeyState(DIK_A) & 0x80)
	{
		if (m_pMeshCom->Get_AnimationSet() == PLAYER_ATTACK)
		{
			m_pMeshCom->Set_AnimationSet(PLAYER_STAND);
		}

		m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] -= D3DXToRadian(90.0f) * fTimeDelta * 0.5f;
		cs_packet_move_left pkt;
		pkt.size = sizeof(pkt);
		pkt.type = CS_LEFT;
		pkt.radian = m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y];

		pkt.x = m_pTransCom->m_vPosition.x;
		pkt.y = m_pTransCom->m_vPosition.y;
		pkt.z = m_pTransCom->m_vPosition.z;

		NETWORK_ENGINE->SendPacket((char *)&pkt);
	}

	if (Engine::GetDIKeyState(DIK_D) & 0x80)
	{
		if (m_pMeshCom->Get_AnimationSet() == PLAYER_ATTACK)
		{
			m_pMeshCom->Set_AnimationSet(PLAYER_STAND);
		}

		m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] += D3DXToRadian(90.0f) * fTimeDelta * 0.5f;

		cs_packet_move_right pkt;
		pkt.size = sizeof(pkt);
		pkt.type = CS_RIGHT;
		pkt.radian = m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y];
		pkt.x = m_pTransCom->m_vPosition.x;
		pkt.y = m_pTransCom->m_vPosition.y;
		pkt.z = m_pTransCom->m_vPosition.z;
		NETWORK_ENGINE->SendPacket((char *)&pkt);
	}

	if (Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON))
	{
		m_pMonster = NULL;
		m_bMove = true;
		m_pMouseCol->PickTerrain(&m_vDestPos, m_pVertex);
		list<Engine::CGameObject*>* pTest = Engine::Find_ObjectList(L"GameLogic", L"Monster");
		auto iter = pTest->begin();
		auto iter_end = pTest->end();

		m_pMeshCom->Set_AnimationSet(PLAYER_WALK);

		for (; iter != iter_end; ++iter)
		{
			Engine::CComponent* pTransCom = (*iter)->Get_Component(L"Com_Transform");
			D3DXVECTOR3 vPos = ((Engine::CTransform*)pTransCom)->m_vPosition;
			D3DXVECTOR3 vDir = m_vDestPos - vPos;
			
			// 피킹한 지점과 몬스터의 거리 계산
			if (D3DXVec3Length(&vDir) < 4.f)
			{
				// 몬스터 Lock
				m_pMonster = (*iter);
			}
		}
	}

	if (Engine::GetDIMouseState(Engine::CInput::DIM_RBUTTON))
	{
		m_pNpc = NULL;
		m_bNpc = true;
		
		list<Engine::CGameObject*>* pNpc = Engine::Find_ObjectList(L"GameLogic", L"Npc");
		auto Npciter = pNpc->begin();
		auto Npciter_end = pNpc->end();
		
		for (; Npciter != Npciter_end; ++Npciter)
		{
			Engine::CComponent* pTransCom = (*Npciter)->Get_Component(L"Com_Transform");
			D3DXVECTOR3 vPos = ((Engine::CTransform*)pTransCom)->m_vPosition;
			D3DXVECTOR3 vDir = m_pTransCom->m_vPosition - vPos;

			if (D3DXVec3Length(&vDir) < 4.f)
			{
				// NPC Lock
				m_pNpc = (*Npciter);

				m_pQuest->Set_bQuest(true);
			}
		}

		if (g_iNum == 0)
		{
		}
		else if (g_iNum > 0 && g_iNum < 10)
		{
			m_pQuest->Set_bQuest(true);
			m_pQuest->SetQuestType(CQuest::QUEST_ING);
		}
		else if (g_iNum >= 10)
		{
			m_pQuest->Set_bQuest(true);
			m_pQuest->SetQuestType(CQuest::QUEST_OK);
		}
	}

	if (m_pQuest->Get_bQuest() == true)
	{
		if (Engine::GetDIKeyState(DIK_ESCAPE))
		{
			m_pQuest->Set_bQuest(false);
			m_pQuest->SetQuestType(CQuest::QUEST_START);
		}
		if (Engine::GetDIKeyState(DIK_RETURN))
		{
			m_pQuest->Set_bQuest(false);
			m_pQuest->SetQuestType(CQuest::QUEST_ING);
		}
	}
	//else m_bPush = false;
}

void CPlayer::Set_MouseCol(CMouseCol* pMouse)
{
	m_pMouseCol = pMouse;
}

_ulong CPlayer::Release(void)
{
	_ulong dwRefCnt = CLandObject::Release();

	if(0 == dwRefCnt)
	{	
		delete this;
	}

	return dwRefCnt;
}
