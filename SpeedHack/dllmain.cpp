﻿//example of usage
#include "Windows.h"
#include "speedhack.h"
//#include"stdio.h"

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

DWORD WINAPI MainThread(LPVOID lpParam)
{
	Speedhack::Setup();
	Speedhack::SetSpeed(3);
	//Speedhack::Detach();
	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0x1000, &MainThread, NULL, 0, NULL);
	}

	return TRUE;
}