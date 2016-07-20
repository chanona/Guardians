#pragma once

#include "State.h"


/*
 * 사용방법
 * FSM을 사용할 객체 클래스 안에 CStateMachine<Object Class> *m_pStateMachine; 선언
 * 처음 객체를 초기화 할시 m_pStateMachine = new CStateMachine<Object Class>(this);
 * m_pStateMachine->SetCurrentState() 함수를 통해 상태 설정
 * 업데이트 함수안에 m_pStateMachine->Update(); 호출
 * 객체 제거시 delete m_pStateMachine;
 *
 *
 */

template <class EntityType>
class CStateMachine
{
public:
	CStateMachine() {}
	CStateMachine(EntityType *pOwner) : m_pOwner(pOwner),
		m_pCurrentState(nullptr),
		m_pPreviousState(nullptr),
		m_pGlobalState(nullptr)
	{}
	~CStateMachine() {}

	void Update() const
	{
		if (m_pGlobalState) m_pGlobalState->Execute(m_pOnwer);
		if (m_pCurrentState) m_pCurrentState->Execute(m_pOnwer);
	}

	void ChangeState(State<EntityType>* pNewState)
	{
		assert(pNewState);

		m_pPreviousState = m_pCurrentState;
		m_pCurrentState->Exit(m_pOnwer);

		m_pCurrentState = pNewState;

		m_pCurrentState->Enter(m_pOnwer);
	}

	void RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}
private:
	EntityType* m_pOnwer;
	CState<EntityType> *m_pCurrentState;
	CState<EntityType> *m_pPreviousState;
	CState<EntityType> *m_pGlobalState;

public:
	// Setter, Getter
	void SetCurrentState(CState<EntityType> *s) { m_pCurrentState = s; }
	void SetPreviousState(CState<EntityType> *s) { m_pPreviousState = s; }
	void SetGlobalState(CState<EntityType> *s) { m_pGlobalState = s; }

	CState<EntityType>* GetCurrentState() { return m_pCurrentState; }
	CState<EntityType *> GetPreviousState() { return m_pPreviousState; }
	CState<EntityType>* GetGlobalState() { return m_pGlobalState; }
};

