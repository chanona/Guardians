#pragma once

#include "State.h"
#include "Player.h"
#include "CSingleton.h"

/*
 * StateMachine에 State 추가할때
 * m_pStateMachine->ChangeState(PlayerIdleState::GetInstance());
 */


/*
 - * 사용방법
 - * FSM을 사용할 객체 클래스 안에 CStateMachine<Object Class> *m_pStateMachine; 선언
 - * 처음 객체를 초기화 할시 m_pStateMachine = new CStateMachine<Object Class>(this);
 - * m_pStateMachine->SetCurrentState() 함수를 통해 상태 설정
 - * 업데이트 함수안에 m_pStateMachine->Update(); 호출
 - * 객체 제거시 delete m_pStateMachine;
 - *
 - *
 - */
	
class PlayerIdleState : public CState<CPlayer>, public CSingleton<PlayerIdleState>
{
public:
	PlayerIdleState() {}

	void Enter(CPlayer *pPlayer) override
	{
		cout << "Idle 상태 들어감\n";
	}

	void Execute(CPlayer *pPlayer) override
	{
		cout << "Idle 상태 실행중\n";
	}

	void Exit(CPlayer *pPlayer) override
	{
		cout << "Idle 상태 빠져나옴\n";
	}
};