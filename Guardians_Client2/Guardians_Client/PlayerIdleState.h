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