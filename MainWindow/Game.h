#pragma once
#include "Data.h"
#include "Helper.h"

void Verify()
{
	DWORD Key = 0;
	BOOL bFlag = 0;
	HWND hDebug = 0;

	SetDebugConsole();

	hDebug = FindWindow(L"ConsoleWindowClass", L"Debug");
	printf("你的机器码:%x\n", GetMachineSerial());
	while (true)
	{
		printf("请输入序列号:");
		scanf_s("%x", &Key);
		bFlag = VerifyKey(Key);
		if (bFlag)
		{
			FreeDebugConsole();
			CloseWindow(hDebug);
			//DWORD Error = GetLastError();
			break;
		}
		else
		{
			system("pause");
			ExitProcess(0);
		}
		Sleep(30);
	}
}

void InitTFHackPro()
{
	//Verify();
	GamePid = GetProcessIdByProcessName(L"SFGame.exe");
	hGameProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GamePid);
	SFGameBaseAddress = GetProcessModuleBaseAddress(GamePid, L"SFGame.exe");
}

BOOL InfiniteAmmo(BOOL Key)
{
	BYTE OldBytes[6] = { 0x8B,0x81,0x58,0x03,0x00,0x00 };
	BYTE NewBytes[6] = { 0xB8,0x01,0x00,0x00,0x00,0x90 };
	DWORD oldProtect = 0;
	DWORD oldProtect2 = 0;
	DWORD HookAddress = SFGameBaseAddress + InfiniteAmmoOffset;
	DWORD tmp1 = 0, tmp2 = 0;

	VirtualProtectEx(hGameProcess,(LPVOID)HookAddress, 6, PAGE_EXECUTE_READWRITE, &oldProtect);

	if (Key)
	{
		WriteProcessMemory(hGameProcess, (LPVOID)HookAddress, NewBytes, 6, NULL);
	}
	else
	{
		WriteProcessMemory(hGameProcess, (LPVOID)HookAddress, OldBytes, 6, NULL);
	}
	VirtualProtectEx(hGameProcess, (LPVOID)HookAddress, 6, oldProtect, &oldProtect2);
	return TRUE;
}

BOOL ZeroRecoil(BOOL Key)
{
	BYTE buffer0_8[] = { 0x90, 0x90 ,0x90 ,0x90 ,0x90 ,0x90, 0x90, 0x90 };
	BYTE buffer0_6[] = { 0x90, 0x90 ,0x90 ,0x90 ,0x90 ,0x90 };
	BYTE buffer0_2[] = { 0x90, 0x90 };
	BYTE buffer1[] = { 0xF3, 0x0F ,0x11 ,0x96 ,0xA8 ,0x0F, 0x00, 0x00 };
	BYTE buffer2[] = { 0x76, 0x1B };
	BYTE buffer3[] = { 0xF3, 0x0F ,0x11 ,0x86 ,0xA8 ,0x0F, 0x00, 0x00 };
	BYTE buffer4[] = { 0xF3, 0x0F ,0x11 ,0x86 ,0xA8 ,0x0F, 0x00, 0x00 };
	BYTE buffer5[] = { 0x72, 0x16 };
	BYTE buffer6[] = { 0xF3, 0x0F ,0x11 ,0x8E ,0xA8 ,0x0F, 0x00, 0x00 };
	BYTE buffer7[] = { 0xF3, 0x0F ,0x11 ,0x8E ,0xEC ,0x0F, 0x00, 0x00 };
	BYTE buffer8[] = { 0x0F, 0x84, 0x68, 0x02, 0x00, 0x00 };
	BYTE buffer9[] = { 0x0F, 0x85, 0xCE, 0x03, 0x00, 0x00 };

	if (Key)
	{
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x974FEF), &buffer0_8, sizeof(buffer0_8), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x97502E), &buffer0_2, sizeof(buffer0_2), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x975146), &buffer0_8, sizeof(buffer0_8), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x9752E2), &buffer0_8, sizeof(buffer0_8), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x9752EF), &buffer0_2, sizeof(buffer0_2), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x9752F1), &buffer0_8, sizeof(buffer0_8), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x9752F9), &buffer0_8, sizeof(buffer0_8), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x974FD0), &buffer0_6, sizeof(buffer0_6), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x974FD9), &buffer0_6, sizeof(buffer0_6), NULL);
	}
	else
	{
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x974FEF), &buffer1, sizeof(buffer0_8), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x97502E), &buffer2, sizeof(buffer0_2), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x975146), &buffer3, sizeof(buffer0_8), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x9752E2), &buffer4, sizeof(buffer0_8), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x9752EF), &buffer5, sizeof(buffer0_2), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x9752F1), &buffer6, sizeof(buffer0_8), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x9752F9), &buffer7, sizeof(buffer0_8), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x974FD0), &buffer8, sizeof(buffer0_6), NULL);
		WriteProcessMemory(hGameProcess, (LPVOID)(SFGameBaseAddress + 0x974FD9), &buffer9, sizeof(buffer0_6), NULL);
	}

	return TRUE;
}

UINT BigJump(LPVOID lpParam)
{
	while (TRUE)
	{
		if (GetAsyncKeyState(0x43) < 0)
		{
			keybd_event(VK_LCONTROL, MapVirtualKey(VK_LCONTROL, 0), 0, 0);
			Sleep(1);
			keybd_event(VK_SPACE, MapVirtualKey(VK_SPACE, 0), 0, 0);
			Sleep(400);
			keybd_event(VK_LCONTROL, MapVirtualKey(VK_LCONTROL, 0), KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SPACE, MapVirtualKey(VK_SPACE, 0), KEYEVENTF_KEYUP, 0);
			Sleep(1000);
			keybd_event(VK_LCONTROL, MapVirtualKey(VK_LCONTROL, 0), KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SPACE, MapVirtualKey(VK_SPACE, 0), KEYEVENTF_KEYUP, 0);
		}
		Sleep(1);
	}
	return 0;
}

UINT RightButtonSimulate(LPVOID lpParam)
{
	while (1)
	{
		if (GetAsyncKeyState(VK_RBUTTON) < 0)
		{
			keybd_event(VK_LBUTTON, MapVirtualKey(VK_LBUTTON, 0), 0, 0);
			Sleep(10);
			keybd_event(VK_LBUTTON, MapVirtualKey(VK_LBUTTON, 0), KEYEVENTF_KEYUP, 0);
		}
		Sleep(1);
	}
	return 0;
}

UINT ChangeShootSpeed(LPVOID lpParam)
{
	DWORD dwTemp = 0;
	float speed = (float)0.01;

	while (1)
	{
		if (GetAsyncKeyState(VK_F4) < 0)
		{
			ReadProcessMemory(hGameProcess, (LPCVOID)(SFGameBaseAddress + 0x013EE5C4), &dwTemp, 4, NULL);
			ReadProcessMemory(hGameProcess, (LPCVOID)(dwTemp + 0x448), &dwTemp, 4, NULL);
			ReadProcessMemory(hGameProcess, (LPCVOID)(dwTemp + 0xC60), &dwTemp, 4, NULL);
			ReadProcessMemory(hGameProcess, (LPCVOID)(dwTemp + 0xD0), &dwTemp, 4, NULL);
			ReadProcessMemory(hGameProcess, (LPCVOID)(dwTemp + 0x298), &dwTemp, 4, NULL);
			WriteProcessMemory(hGameProcess, (LPVOID)(dwTemp + 0x0), &speed, 4, NULL);
		}
		Sleep(1);
	}
	return 0;
}

/*
BOOL AimBot(LPARAM lparam)
{
	SetDebugConsole(L"Test");

	DWORD dwPid = GetProcessIdByProcessName(L"SFGame.exe");
	DWORD dwBaseAddr = GetProcessModuleBaseAddress(dwPid, L"SFGame.exe");
	if (dwBaseAddr == FALSE)
		return 0;
	DWORD dwTmp = 0;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	Sleep(50);

	DWORD dwHp = 0;
	char buffer[4] = { 0 };
	Vec3 Pos = { 0 };
	Vec3 MyPos = { 0 };
	Vec3 RePos = { 0 };
	int dwDis = 0;

	double Pitch = 0;
	double Yaw = 0;

	int GamePitch = 0;
	int GameYaw = 0;

	int TempPitch = 0;
	int TempYaw = 0;
	int tmp = 0;

	double dis = 0;

	BOOL bFlag = FALSE;

	while (TRUE)
	{
		if (GetKeyState(VK_MENU) & 0x8000 || GetKeyState(VK_LBUTTON) & 0x8000 || GetKeyState(VK_RBUTTON) & 0x8000)
		{
			for (size_t i = 0; i < GetpplNum(hProcess, dwBaseAddr) * 2; i++)
			{
				GetPos(hProcess, &Pos, &dwHp, dwBaseAddr, i);
				if (dwHp > 0 && dwHp < 1000)
				{
					//Get My Position
					ReadProcessMemory(hProcess, (LPCVOID)(dwBaseAddr + 0x013ED5C8), &buffer, 4, NULL);	//dwTemp优化掉了
					ReadProcessMemory(hProcess, (LPCVOID)(*(DWORD*)buffer + 0x54), &MyPos, 12, NULL);	//

					RePos.x = Pos.x - MyPos.x;
					RePos.y = Pos.y - MyPos.y;
					RePos.z = Pos.z - MyPos.z;
					//if (RePos.x != 0 & RePos.y != 0 & RePos.z != 0)
					//	continue;
					dis = sqrt((double)RePos.x * RePos.x + (double)RePos.y * RePos.y + (double)RePos.z * RePos.z);
					if (dis < 0.01 || dis > 2000)	continue;

					Pitch = atan2(RePos.y, RePos.x) * 180.f / PIE;
					double tmp1 = pow(RePos.x, 2) + pow(RePos.y, 2);
					Yaw = -atan2(-RePos.z, sqrt(tmp1)) * 180.f / PIE;

					//printf("%f\t%f\n", Pitch, Yaw);

					ReadProcessMemory(hProcess, (LPCVOID)(dwBaseAddr + 0x013ED5C4), &dwTmp, 4, NULL);
					ReadProcessMemory(hProcess, (LPVOID)(dwTmp + 0x64), &TempPitch, 4, NULL);
					ReadProcessMemory(hProcess, (LPVOID)(dwTmp + 0x60), &TempYaw, 4, NULL);

					GamePitch = (int)(Pitch * 65536 / 360) - 10;
					GameYaw = (int)(Yaw * 65536 / 360) - 10;

					GamePitch %= 65536;
					GameYaw %= 65536;
					if (GamePitch < 0)
						GamePitch += 65536;
					if (GameYaw < 0)
						GameYaw += 65536;
					//
					TempPitch %= 65536;
					TempYaw %= 65536;
					if (TempPitch < 0)
						TempPitch += 65536;
					if (TempYaw < 0)
						TempYaw += 65536;

					tmp = TempPitch - GamePitch;
					if (abs(tmp) > 1250)
						continue;
					tmp = TempYaw - GameYaw;
					if (abs(tmp) > 1250)	//2731
						continue;

					//printf("%d\n", TempPitch);
					printf("%d\t%d\n", GamePitch, GameYaw);

					if (GamePitch != 0 && GameYaw != 0)
					{
						WriteProcessMemory(hProcess, (LPVOID)(dwTmp + 0x64), &GamePitch, 4, NULL);
						WriteProcessMemory(hProcess, (LPVOID)(dwTmp + 0x60), &GameYaw, 4, NULL);
					}
				}
			}
		}
	}
	CloseHandle(hProcess);
	return 0;
}
*/