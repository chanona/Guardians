#pragma once

#include "State.h"

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
		if (m_pGlobalState) m_pGlobalState->Execute(m_pOwner);
		if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
	}

	void ChangeState(CState<EntityType>* pNewState)
	{
		assert(pNewState);

		m_pPreviousState = m_pCurrentState;
		m_pCurrentState->Exit(m_pOwner);

		m_pCurrentState = pNewState;

		m_pCurrentState->Enter(m_pOwner);
	}

	void RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}
private:
	EntityType* m_pOwner;
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

