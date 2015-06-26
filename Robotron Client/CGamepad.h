#pragma once
#pragma comment( lib, "Xinput9_1_0.lib" )

#include <iostream>
#include <Windows.h>
#include <Xinput.h>

class CGamepad
{
public:

	CGamepad(); 
	~CGamepad();
	
	CGamepad(float dzX, float dzY) : deadzoneX(dzX), deadzoneY(dzY) {}
	
	int CGamepad::GetPort(){ return cId + 1; }

	XINPUT_GAMEPAD *CGamepad::GetState(){ return &state.Gamepad; }

	bool CheckConnection();
	
	bool Refresh();
	
	bool IsPressed(WORD);


private:
	int cId;
	XINPUT_STATE state;

	float deadzoneX;
	float deadzoneY;

public:
	

	
};




