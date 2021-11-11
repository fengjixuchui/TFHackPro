#pragma once
#include"Windows.h"
#include"stdio.h"
#include"tlhelp32.h"
#include"stdlib.h"
#include"Psapi.h"
#pragma warning(disable:4996)

BOOL GetWndPid(LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD& dwPid);
DWORD GetProcessIdByProcessName(const WCHAR ProcessName[MAX_PATH]);
DWORD GetProcessModuleBaseAddress(DWORD dwPid, const WCHAR ModuleName[MAX_MODULE_NAME32 + 1]);
BOOL RemoteThreadDllInject(const WCHAR*, const WCHAR*);
BOOL FetchProcess();
BOOL FetchProcessImageBase(DWORD dwPid);
const WCHAR* GetFileFullPath(const WCHAR* FileName);

//////////////////////////////////////////

BOOL RemoteThreadDllInject(const WCHAR* ProcessName, const WCHAR* dllpath)
{
	//LPCWSTR dllpath = L"C:\\Users\\Administrator\\Desktop\\TFHack.dll";
	DWORD dwPid = GetProcessIdByProcessName(ProcessName);
	printf("%d\n", dwPid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL)
	{
		printf("OpenProcess()  ERROR_CODE=%x\n", GetLastError());
		return FALSE;
	}
	LPVOID VMAddress = VirtualAllocEx(hProcess, NULL, ((wcslen(dllpath) + 1) * 2), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (VMAddress == NULL)
	{
		printf("VirtualAllocEx()  ERROR_CODE=%x\n", GetLastError());
		CloseHandle(hProcess);
		return FALSE;
	}
	WriteProcessMemory(hProcess, VMAddress, dllpath, ((wcslen(dllpath) + 1) * 2), NULL);
	//FARPROC fAddress = GetProcAddress(GetModuleHandle(L"KERNEL32.dll"), "LoadLibraryW");
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryW, VMAddress, 0, NULL);
	if (hRemoteThread == NULL)
	{
		printf("CreateRemoteThread()  ERROR_CODE=%x\n", GetLastError());
		CloseHandle(hProcess);
		return FALSE;
	}
	WaitForSingleObject(hRemoteThread, 2000);
	VirtualFreeEx(hProcess, VMAddress, NULL, MEM_RELEASE);
	CloseHandle(hProcess);
	CloseHandle(hRemoteThread);
	return TRUE;
}

BOOL RemoteThreadDllFree(const WCHAR* ProcessName, const WCHAR* dllName)
{
	//LPCWSTR dllpath = L"C:\\Users\\Administrator\\Desktop\\TFHack.dll";
	RemoteThreadDllInject(ProcessName, GetFileFullPath(dllName));
	DWORD dwPid = GetProcessIdByProcessName(ProcessName);
	//CString str;
	//str.Format(L"%d", dwPid);
	//MessageBox(0,str,L"PID",0);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL)
	{
		CString str;
		str.Format(L"%u",GetLastError());
		MessageBox(0, L"OpenProcess()"+str,0 ,0);
		return FALSE;
	}
	DWORD dwHandle = (DWORD)GetProcessModuleBaseAddress(dwPid, dllName);

	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)FreeLibrary, (LPVOID)dwHandle, 0, NULL);
	if (hRemoteThread == 0)
	{
		printf("CreateRemoteThread()  ERROR_CODE=%x\n", GetLastError());
		CloseHandle(hProcess);
		return FALSE;
	}
	WaitForSingleObject(hRemoteThread, 2000);
	CloseHandle(hProcess);
	CloseHandle(hRemoteThread);
	return TRUE;
}


DWORD GetProcessModuleBaseAddress(DWORD dwPid, const WCHAR ModuleName[MAX_MODULE_NAME32 + 1])
{
	HANDLE hProcessModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	if (hProcessModuleSnap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot() Fail! ERROR_CODE=%x", GetLastError());
		return FALSE;
	}
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	if (!Module32First(hProcessModuleSnap, &me32))
	{
		CloseHandle(hProcessModuleSnap);
		return 0;
	}
	do {
		//wprintf(L"ModuleName=%s\tBaseAddress=%p\n",me32.szModule,me32.modBaseAddr);
		if (!wcscmp(ModuleName, me32.szModule))
		{
			//wprintf(L"ModuleName=%s\tBaseAddress=%p\n", me32.szModule, me32.modBaseAddr);
			return (DWORD)me32.modBaseAddr;
		}
	} while (Module32Next(hProcessModuleSnap, &me32));
	return 0;
}


BOOL GetWndPid(LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD& dwPid)
{
	HWND hWnd = FindWindow(lpClassName, lpWindowName);
	if (hWnd == NULL)
	{
		return FALSE;
	}
	GetWindowThreadProcessId(hWnd, &dwPid);
	return TRUE;
}


DWORD GetProcessIdByProcessName(const WCHAR ProcessName[MAX_PATH])
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		//printf("CreateToolhelp32Snapshot() Fail! ERROR_CODE=%x\n", GetLastError());
		return FALSE;
	}
	PROCESSENTRY32 pe32;
	ZeroMemory(&pe32, sizeof(pe32));
	pe32.dwSize = sizeof(pe32);
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);
		return FALSE;
	}
	do {
		//printf("%ls",pe32.szExeFile);
		if (!wcscmp(pe32.szExeFile, ProcessName))
		{
			return pe32.th32ProcessID;
		}
	} while (Process32Next(hProcessSnap, &pe32));
	return FALSE;
}

BOOL FetchProcess()
{
	DWORD dwProcessID[0x500] = { 0 };  //开始的预先分配较大的缓冲区，用来存放进程ID
	DWORD dwNeeded = 0;
	BOOL bEnumRes = EnumProcesses(dwProcessID, sizeof(dwProcessID), &dwNeeded);
	UINT uCount = dwNeeded / sizeof(DWORD);//获得枚举到进程的数量
	for (UINT i = 0; i < uCount; i++)
	{
		//只对进程进程枚举，所以申请QUERY权限，具体还得根据应用申请权限
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID[i]);
		if (hProcess != INVALID_HANDLE_VALUE)
		{
			CHAR szProcessName[MAX_PATH] = { 0 };
			DWORD dwNameLen = MAX_PATH;
			BOOL bRet = QueryFullProcessImageNameA(hProcess, 0, szProcessName, &dwNameLen);
			if (bRet)
			{
				printf("ID:%4d\tprocessName(%s)\n", dwProcessID[i], szProcessName);
			}
		}
	}
	return 0;
}

BOOL FetchProcessImageBase(DWORD dwPid)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, dwPid);
	HMODULE hModuleList[] = { 0 };
	DWORD dwRet = 0;
	BOOL bRet = EnumProcessModules(hProcess, hModuleList, sizeof(hModuleList), &dwRet);
	if (!bRet)
	{
		OutputDebugStringA("EnumProcessModules error\n");
		CloseHandle(hProcess);
		return 0;
	}
	printf("base image address 0x%p", hModuleList[0]);
	return 0;
}

const WCHAR* GetFileFullPath(const WCHAR * FileName)
{
	static WCHAR FileFullPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, FileFullPath, MAX_PATH);
	for (int i = wcslen(FileFullPath); i > 0; i--)
	{
		if (FileFullPath[i] == '\\')
		{
			memcpy(&FileFullPath[i + 1], FileName, 38);
			return FileFullPath;
			break;
		}
	}
	return NULL;
}

//FILE* stream;
void SetDebugConsole()
{
	AllocConsole();
	SetConsoleTitle(L"Debug");

	//freopen_s(&stream, "CON", "w", stdout);
	//freopen_s(&stream, "CON", "r", stdin);
	freopen("CON", "w", stdout);
	freopen("CON", "r", stdin);
	//printf("ok\n");
}

void FreeDebugConsole()
{
	fclose(stdin);
	fclose(stdout);
	FreeConsole();
}

DWORD GetMachineSerial()
{
	char     m_Volume[256];//卷标名  
	char     m_FileSysName[256];
	DWORD   m_SerialNum;//序列号  
	DWORD   m_FileNameLength;
	DWORD   m_FileSysFlag;
	::GetVolumeInformationA("c:\\",
		m_Volume,
		256,
		&m_SerialNum,
		&m_FileNameLength,
		&m_FileSysFlag,
		m_FileSysName,
		256);
	m_SerialNum ^= 0xCDCDCDCD;
	//printf("SerialNum = %x\n", m_SerialNum);
	return m_SerialNum;
}


BOOL VerifyKey(DWORD Key)
{
	DWORD Serial = 0;
	DWORD MyKey = 0;
	DWORD Temp = 0;

	Serial = GetMachineSerial();
	Temp = *(BYTE*)((BYTE*)&Serial + 0) + *(BYTE*)((BYTE*)&Serial + 1) + *(BYTE*)((BYTE*)&Serial + 2) + *(BYTE*)((BYTE*)&Serial + 3);
	Temp = ~Temp;
	MyKey = Serial ^ Temp;
	//printf("MyKey = %x\n", MyKey);
	if (Key == MyKey)
	{
		printf("校验成功\n");
		return TRUE;
	}
	else
	{
		printf("校验失败\n");
		return FALSE;
	}
}


bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight)
{
	Vec4 clipCoords;

	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;
	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

	return true;
}


BOOL GetWindowRealSize(HWND hWnd)
{
	while (1)
	{
		// 获取窗口当前显示的监视器
		//HWND hWnd = GetDesktopWindow();//根据需要可以替换成自己程序的句柄 
		HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

		// 获取监视器逻辑宽度与高度
		MONITORINFOEX miex;
		miex.cbSize = sizeof(miex);
		GetMonitorInfo(hMonitor, &miex);
		int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
		int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

		// 获取监视器物理宽度与高度
		DEVMODE dm;
		dm.dmSize = sizeof(dm);
		dm.dmDriverExtra = 0;
		EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
		int cxPhysical = dm.dmPelsWidth;
		int cyPhysical = dm.dmPelsHeight;

		//缩放比例计算
		double horzScale = ((double)cxPhysical / (double)cxLogical);
		double vertScale = ((double)cyPhysical / (double)cyLogical);

		RECT RectTmp;
		GetWindowRect(hWnd, &RectTmp);
		DWORD W = (DWORD)((RectTmp.right - RectTmp.left) * horzScale);
		DWORD H = (DWORD)((RectTmp.bottom - RectTmp.top) * vertScale);
		if (W != WIDTH || H != HEIGHT)
		{
			WIDTH = W;
			HEIGHT = H;
		}

		if (!WIDTH || !HEIGHT)
			return FALSE;
	}
	return TRUE;
}