#pragma once

template <class EntityType>
class CState
{
public:
	CState() {}
	virtual ~CState() {}

	virtual void Enter(EntityType *) = 0;
	virtual void Execute(EntityType *) = 0;
	virtual void Exit(EntityType *) = 0;
};

