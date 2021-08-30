#include "Windows.h"
#include "speedhack.h"
#include "psapi.h"
#include "stdio.h"

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
	//Speedhack::SetSpeed(3);

	//AllocConsole();
	//SetConsoleTitle(L"TFHackDLL");
	//FILE* stream;
	//freopen_s(&stream,"CON","w",stdout);
	//printf("ok\n");

	BOOL 加速 = FALSE;
	BOOL 三倍速 = FALSE;

	while (TRUE)
	{
		//切换加速状态
		//if (GetAsyncKeyState(VK_NUMPAD1) != 0)
		if((GetKeyState(VK_NUMPAD1) & 0x8000) != 0)
		{
			if (加速)
			{
				加速 = FALSE;
				Sleep(500);
			}
			else
			{
				加速 = TRUE;
				Sleep(500);
			}
		}
		if (加速 != 三倍速)
		{
			if (加速 == TRUE)
			{
				//Speedhack::Setup();
				Speedhack::SetSpeed(3);
				//printf("Speedhack::SetSpeed(3)\n");
			}
			else
			{
				Speedhack::SetSpeed(1);
				//Speedhack::Detach();
				//printf("Speedhack::SetSpeed(1)\n");
			}
			三倍速 = 加速;
		}
	}
	//Speedhack::SetSpeed(3);
	//Speedhack::Detach();
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