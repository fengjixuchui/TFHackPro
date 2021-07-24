#include "Windows.h"
#include "speedhack.h"
#include "psapi.h"
//#include"stdio.h"

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

BOOL CheckDLL()
{
	HANDLE hProcess = GetCurrentProcess();
	if (hProcess == 0)
		return 0;
	HMODULE hModuleList[] = { 0 };
	DWORD dwRet = 0;
	BOOL bRet = EnumProcessModules(hProcess, hModuleList, sizeof(hModuleList), &dwRet);
	WCHAR pFileName[MAX_PATH] = { 0 };
	
	for (size_t i = 0; i < (sizeof(hModuleList)/sizeof(HMODULE)); i++)
	{
		GetModuleFileName(hModuleList[i], pFileName, MAX_PATH);
		MessageBox(0, pFileName, 0, 0);
	}
}

DWORD WINAPI MainThread(LPVOID lpParam)
{
	Speedhack::Setup();
	while (1)
	{
		if (GetKeyState(VK_HOME & 1))
		{
			if (GetKeyState(0x45) < 0)
			{
				Speedhack::SetSpeed(40);
				Sleep(10);
			}
			Speedhack::SetSpeed(1);
			Sleep(100);
		}
		else if (GetKeyState(VK_NUMPAD2 & 1))
		{
			Speedhack::SetSpeed(1.5);
			Sleep(10);
		}
		else if (GetKeyState(VK_NUMPAD3 & 1))
		{
			Speedhack::SetSpeed(3);
			Sleep(10);
		}
		else if (GetKeyState(VK_NUMPAD4 & 1))
		{
			Speedhack::SetSpeed(15);
			Sleep(10);
		}
	}
	//Speedhack::SetSpeed(3);
	Speedhack::Detach();
	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		HANDLE hMainThread = CreateThread(0, 0x1000, &MainThread, NULL, 0, NULL);
	}
	return TRUE;
}