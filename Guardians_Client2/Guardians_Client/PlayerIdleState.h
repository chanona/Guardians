#pragma once

#include "State.h"
#include "Player.h"
#include "CSingleton.h"

/*
 * StateMachine�� State �߰��Ҷ�
 * m_pStateMachine->ChangeState(PlayerIdleState::GetInstance());
 */


/*
 - * �����
 - * FSM�� ����� ��ü Ŭ���� �ȿ� CStateMachine<Object Class> *m_pStateMachine; ����
 - * ó�� ��ü�� �ʱ�ȭ �ҽ� m_pStateMachine = new CStateMachine<Object Class>(this);
 - * m_pStateMachine->SetCurrentState() �Լ��� ���� ���� ����
 - * ������Ʈ �Լ��ȿ� m_pStateMachine->Update(); ȣ��
 - * ��ü ���Ž� delete m_pStateMachine;
 - *
 - *
 - */
	
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
		cout << "Idle ���� ��������\n";
	}
};