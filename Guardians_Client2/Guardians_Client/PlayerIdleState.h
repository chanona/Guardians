#pragma once

#include "State.h"
#include "Player.h"
#include "CSingleton.h"

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
		cout << "Idle 상태 빠져나옴\n"
	}
};