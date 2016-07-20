#pragma once

#include "State.h"
#include "Player.h"
#include "CSingleton.h"

/*
 * StateMachine�� State �߰��Ҷ�
 * m_pStateMachine->ChangeState(PlayerIdleState::GetInstance());
 */

class PlayerIdleState : public CState<CPlayer>, public CSingleton<PlayerIdleState>
{
public:
	PlayerIdleState() {}

	void Enter(CPlayer *pPlayer) override
	{
		cout << "Idle ���� ��\n";
	}

	void Execute(CPlayer *pPlayer) override
	{
		cout << "Idle ���� ������\n";
	}

	void Exit(CPlayer *pPlayer) override
	{
		cout << "Idle ���� ��������\n"
	}
};