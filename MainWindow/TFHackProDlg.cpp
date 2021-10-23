
// TFHackProDlg.cpp: 实现文件
//

#include "framework.h"
#include "TFHackPro.h"
#include "TFHackProDlg.h"
#include "afxdialogex.h"
#include "tlhelp32.h"
#include "Main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTFHackProDlg 对话框



CTFHackProDlg::CTFHackProDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TFHACKPRO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CTFHackProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTFHackProDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTFHackProDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTFHackProDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTFHackProDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTFHackProDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTFHackProDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CTFHackProDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &CTFHackProDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CTFHackProDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// CTFHackProDlg 消息处理程序

BOOL CTFHackProDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTFHackProDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTFHackProDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTFHackProDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

///////////////////////////////////////////

UINT LockAmmo(LPVOID lpParam)
{
	//AfxMessageBox(L"进入函数");
	DWORD dwPid = GetProcessIdByProcessName(L"SFGame.exe");
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	DWORD dwBaseAddress = GetProcessModuleBaseAddress(dwPid, (WCHAR*)L"SFGame.exe");
	DWORD dwTempAddress = 0;
	DWORD dwAmmo =999;
	DWORD dwTemp = 0;
	DWORD dwRetNum = 0;
	BOOL bFlag = FALSE;
	while (TRUE)
	{
		if (dwBaseAddress == 0){
			return FALSE;
		}
		ReadProcessMemory(hProcess, (LPVOID)((DWORD)(dwBaseAddress + 0x013A84B0)), &dwTempAddress, 4, &dwRetNum);
		if (dwTempAddress == 0){
			return FALSE;
		}
		ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x24), &dwTempAddress, 4, &dwRetNum);
		ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x0), &dwTempAddress, 4, &dwRetNum);
		ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x64), &dwTempAddress, 4, &dwRetNum);
		ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x28), &dwTempAddress, 4, &dwRetNum);
		ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0xC54), &dwTempAddress, 4, &dwRetNum);
		ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0xD0), &dwTempAddress, 4, &dwRetNum);
		ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x358), &dwTemp, 4, &dwRetNum);
		if (dwTemp>0||dwTemp<1000)
		{
			WriteProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x358), &dwAmmo, 4, NULL);
		}
		Sleep(50);
	}
	CloseHandle(hProcess);
	return FALSE;
}

UINT AddAmmo(LPVOID lpParam)
{
	//AfxMessageBox(L"进入函数");
	DWORD dwPid = GetProcessIdByProcessName(L"SFGame.exe");
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	DWORD dwBaseAddress = GetProcessModuleBaseAddress(dwPid, (WCHAR*)L"SFGame.exe");
	DWORD dwTempAddress = 0;
	DWORD dwAmmo = 99999;
	DWORD dwTemp = 0;
	DWORD dwRetNum = 0;
	BOOL bFlag = FALSE;
	if (dwBaseAddress == 0) {
	return FALSE;
	}
	ReadProcessMemory(hProcess, (LPVOID)((DWORD)(dwBaseAddress + 0x013ED5C4)), &dwTempAddress, 4, &dwRetNum);
	if (dwTempAddress == 0) {
	return FALSE;
	}
	ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x228), &dwTempAddress, 4, &dwRetNum);
	ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x40C), &dwTempAddress, 4, &dwRetNum);
	ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x358), &dwTemp, 4, &dwRetNum);
	if (dwTemp > 0 || dwTemp < 1000)
	{
		WriteProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x378), &dwAmmo, 4, NULL);
		WriteProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x358), &dwAmmo, 4, NULL);
		CloseHandle(hProcess);
		return TRUE;
	}
	Sleep(50);
	CloseHandle(hProcess);
	return FALSE;
}

UINT ZeroRecoil(LPVOID lpParam)
{
	DWORD dwPid = GetProcessIdByProcessName(L"SFGame.exe");
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

	//VirtualAllocEx();
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	DWORD dwBaseAddress = GetProcessModuleBaseAddress(dwPid, (WCHAR*)L"SFGame.exe");
	DWORD dwTempAddress = 0;
	BYTE dwTmp1, dwTmp2, dwTmp3, dwTmp4;
	CHAR buffer0[] = { 0x90 ,0x90 ,0x90 ,0x90 ,0x90 ,0x90 ,0x90 ,0x90 };
	CHAR buffer1[] = { 0xF3 ,0x0F ,0x11 ,0x86 ,0xA8 ,0x0F ,0x00 ,0x00 };
	CHAR buffer2[] = { 0xF3 ,0x0F ,0x11 ,0x8E ,0xA8 ,0x0F ,0x00 ,0x00 };
	CHAR buffer3[] = { 0xF3 ,0x0F ,0x11 ,0x96 ,0xA8 ,0x0F ,0x00 ,0x00 };
	
	ReadProcessMemory(hProcess, (LPCVOID)(dwBaseAddress + 0x9756F2), &dwTmp1, 1, NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwBaseAddress + 0x975556), &dwTmp2, 1, NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwBaseAddress + 0x975701), &dwTmp3, 1, NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwBaseAddress + 0x9753FF), &dwTmp4, 1, NULL);

	if (dwTmp1==0xf3 && dwTmp2 == 0xf3 && dwTmp3 == 0xf3 && dwTmp4 == 0xf3)
	{
		WriteProcessMemory(hProcess,(LPVOID)(dwBaseAddress + 0x9756F2),&buffer0,sizeof(buffer1),NULL);
		WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x975556), &buffer0, sizeof(buffer1), NULL);
		WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x975701), &buffer0, sizeof(buffer1), NULL);
		WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x9753FF), &buffer0, sizeof(buffer1), NULL);
	}
	else if (dwTmp1 == 0x90 && dwTmp2 == 0x90 && dwTmp3 == 0x90 && dwTmp4 == 0x90)
	{
		WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x9756F2), &buffer1, sizeof(buffer1), NULL);
		WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x975556), &buffer1, sizeof(buffer1), NULL);
		WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x975701), &buffer2, sizeof(buffer1), NULL);
		WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x9753FF), &buffer3, sizeof(buffer1), NULL);
	}
	else
	{
		WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x9756F2), &buffer1, sizeof(buffer1), NULL);
		WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x975556), &buffer1, sizeof(buffer1), NULL);
		WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x975701), &buffer2, sizeof(buffer1), NULL);
		WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x9753FF), &buffer3, sizeof(buffer1), NULL);
	}

	return true;
}

/// <summary>
/// //////////////////////////////////////
/// </summary>

DWORD WIDTH;
DWORD HEIGHT;

struct Vec3 { float x, y, z; };
struct Vec4 { float x, y, z, w; };
struct Vec2 { float x, y; };


HDC hDC;
HFONT Font;
HPEN hPen;
HBRUSH Brush = CreateSolidBrush(RGB(0, 255, 0));
COLORREF TextCOLOR = RGB(255, 100, 255);
float Matrix[16] = { 0 };

BOOL GetPos(HANDLE hProcess, float pos[3], DWORD& dwHp, DWORD dwBaseAddress, DWORD i)
{
	DWORD dwTempAddress = 0;
	DWORD dwRetNum = 0;
	float dwTemp = 0;
	float tmp[3];

	ReadProcessMemory(hProcess, (LPCVOID)(dwBaseAddress + 0x013ED5C4), &dwTempAddress, 4, &dwRetNum);
	if (dwTempAddress == 0x013ED5C4)	return false;
	//ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x168), &dwTempAddress, 4, &dwRetNum);
	//if (dwTempAddress == 0x168)	return false;
	//ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x4), &dwTempAddress, 4, &dwRetNum);
	//if (dwTempAddress == 0x4)	return false;
	ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x448), &dwTempAddress, 4, &dwRetNum);
	if (dwTempAddress == 0x168)	return false;
	ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x560), &dwTempAddress, 4, &dwRetNum);
	if (dwTempAddress == 0x560)	return false;
	ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x0 + 0x10 * i), &dwTempAddress, 4, &dwRetNum);
	if (dwTempAddress == 0x10 * i)	return false;
	ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x5C - 0x8), &tmp, 12, &dwRetNum);

	for (size_t i = 0; i < 3; i++)
	{
		pos[i] = tmp[i];
	}
	ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x5C + 0x2DC), &dwHp, 12, &dwRetNum);
	return true;
}
DWORD GetpplNum(HANDLE hProcess, DWORD dwBaseAddress)
{
	DWORD dwTempAddress = 0;
	DWORD dwRetNum = 0;
	DWORD dwTemp = 0;
	ReadProcessMemory(hProcess, (LPCVOID)(dwBaseAddress + 0x1381C90), &dwTemp, 4, &dwRetNum);

	return dwTemp;
}

HDC hdcMem;
HBITMAP hbmMem;
HGDIOBJ hOld;


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


void BeginDraw()
{
	hdcMem = CreateCompatibleDC(hDC);
	hbmMem = CreateCompatibleBitmap(hDC, WIDTH, HEIGHT);
	hOld = SelectObject(hdcMem, hbmMem);
}

void EndDraw()
{
	BitBlt(hDC, 0, 0, WIDTH, HEIGHT, hdcMem, 0, 0, PATCOPY);
	SelectObject(hdcMem, hOld);

	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
}

void DrawFilledRect(int x, int y, int w, int h)
{
	//BeginDraw();
	RECT rect = { x, y, x + w, y + h };
	FillRect(hDC, &rect, Brush);
	//EndDraw();

}


void DrawBorderBox(int x, int y, int w, int h, int thickness)
{
	DrawFilledRect(x, y, w, thickness);
	DrawFilledRect(x, y, thickness, h);
	DrawFilledRect((x + w), y, thickness, h);
	DrawFilledRect(x, y + h, w + thickness, thickness);
}


void DrawLine(int targetX, int targetY)
{
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 142));
	// 选中画笔
	SelectObject(hDC, hPen);
	//MoveToEx(hDC, 960, 1080, NULL);
	MoveToEx(hDC, WIDTH / 2, -100, NULL);
	LineTo(hDC, targetX, targetY);
	DeleteObject(hPen);

}

void DrawString(int x, int y, COLORREF color, const char* text)
{
	SetTextAlign(hDC, TA_CENTER | TA_NOUPDATECP); SetBkColor(hDC, RGB(255, 0, 0));
	SetBkMode(hDC, TRANSPARENT); SetTextColor(hDC, color); SelectObject(hDC, Font); TextOutA(hDC, x, y, text, strlen(text)); DeleteObject(Font);
}

BOOL GetMatrix(HANDLE hProcess, DWORD dwBaseAddress)
{
	DWORD dwTempAddress = 0;
	DWORD dwRetNum = 0;
	DWORD dwTemp = 0;
	ReadProcessMemory(hProcess, (LPCVOID)(dwBaseAddress + 0x01393D74), &dwTempAddress, 4, &dwRetNum);
	if (dwTempAddress == 0x01393D74)	return false;
	ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x50), &dwTempAddress, 4, &dwRetNum);
	if (dwTempAddress == 0x50)	return false;
	ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0xF4), &dwTempAddress, 4, &dwRetNum);
	if (dwTempAddress == 0xF4)	return false;
	ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x5C), &dwTempAddress, 4, &dwRetNum);
	if (dwTempAddress == 0x5C)	return false;
	ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x24), &dwTempAddress, 4, &dwRetNum);
	ReadProcessMemory(hProcess, (LPCVOID)(dwTempAddress + 0x6A0), &Matrix, 64, &dwRetNum);

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
		DWORD W = (RectTmp.right - RectTmp.left) * horzScale;
		DWORD H = (RectTmp.bottom - RectTmp.top) * vertScale;
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

UINT ESPBox(LPVOID lpParam)
{
	//SetDebugConsole(L"ESPBox");
	//BeginDraw();

	DWORD dwPid = GetProcessIdByProcessName(L"SFGame.exe");
	DWORD dwBaseAddr = GetProcessModuleBaseAddress(dwPid, L"SFGame.exe");
	if (dwBaseAddr == FALSE)
		return 0;
	DWORD dwTmp;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	HWND hWnd = FindWindow(NULL, L"风暴战区TF");
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GetWindowRealSize, hWnd, 0, 0);
	Sleep(50);

	hDC = GetDC(hWnd);
	Vec2 vScreen = { 0 };
	Vec3 Pos = { 0 };
	float tmp[3];

	DWORD width = 25000;
	DWORD height = width * ((float)WIDTH / HEIGHT) * 2.5;

	Vec3 MyPos = { 0 };
	DWORD dwHp = 0;

	COLORREF CenterCOLOR = RGB(0, 255, 0);;
	//printf("%d\n", GetpplNum(hProcess, dwBaseAddr));

	double dwDis=0;

	while (1)
	{
		DrawString(25, 0, RGB(255, 100, 255), "ROOT");
		DrawString(WIDTH / 2 - 3, HEIGHT / 2 - 27, CenterCOLOR, "·");

		for (size_t i = 0; i < GetpplNum(hProcess, dwBaseAddr)*2; i++)
		//for (size_t i = 0; i < 32; i++)
		{
			if (GetPos(hProcess, tmp, dwHp, dwBaseAddr, i))
			{
				//system("cls");
				//printf("ppl = %d\n", i);
				if (dwHp > 0 && dwHp < 101)
				{
					Pos.x = tmp[0];
					Pos.y = tmp[1];
					Pos.z = tmp[2];

					GetMatrix(hProcess, dwBaseAddr);

					if (WorldToScreen(Pos, vScreen, Matrix, WIDTH, HEIGHT))
					{

						ReadProcessMemory(hProcess, (LPCVOID)(dwBaseAddr + 0x013ED5C8), &dwTmp, 4, NULL);
						ReadProcessMemory(hProcess, (LPCVOID)(dwTmp + 0x54), &MyPos, 12, NULL);

						if (((DWORD)Pos.x != MyPos.x) && ((DWORD)Pos.y != MyPos.y))
						{
							dwDis = sqrt(pow(((double)MyPos.x - Pos.x), 2) + pow(((double)MyPos.y - Pos.y), 2) + pow(((double)MyPos.z - Pos.z), 2));
							if (dwDis > 0)
							{
								char strHp[256] = { 0 };
								sprintf_s(strHp, 256, "%d", dwHp);
								DrawString(vScreen.x, vScreen.y, RGB(255, 0, 0), strHp);
								DrawBorderBox(vScreen.x - (width * 0.5 / dwDis), vScreen.y - (height * 0.5 / dwDis), width / dwDis, height / dwDis, 1);
								if (GetKeyState(VK_INSERT) & 1)
								{
									DrawLine(vScreen.x, vScreen.y);
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}


HANDLE hThread1;
CWinThread* MyThread2;
CWinThread* MyThread3;

BOOL bFlag1 = 0;

void CTFHackProDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//MyThread1 = AfxBeginThread(LockAmmo, NULL);
	if (bFlag1 == 0)
	{
		hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LockAmmo, NULL, 0, 0);
		bFlag1 = 1;
	}
	else if(bFlag1 == 1)
	{
		TerminateThread(hThread1, 0);
		CloseHandle(hThread1);
		bFlag1 = 0;
	}

	UpdateData(FALSE);
}

BOOL g_Button2_Flag = FALSE;

void CTFHackProDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	const WCHAR* dllName = L"SpeedHack.dll";
	const WCHAR *exeFullPath = GetFileFullPath(dllName);
	if (g_Button2_Flag == FALSE)
	{
		BOOL Flag = RemoteThreadDllInject(L"SFGame.exe", exeFullPath);

		if (Flag == TRUE)
		{
			g_Button2_Flag = TRUE;
			AfxMessageBox(L"注入成功\n本功能目前存在BUG\n只可开启一次");
			//MessageBox(L"按下HOME键开启E键瞬移\n小键盘1设置速度为1倍速\n小键盘2设置速度为1.5倍速\n小键盘3设置速度为3倍速\n小键盘4设置速度为15倍速\n", L"加速功能说明", NULL);
			MessageBox(L"小键盘1设置速度为3倍速", L"加速功能说明", NULL);
		}
	}
	else if (g_Button2_Flag == TRUE)
	{
		BOOL Flag = RemoteThreadDllFree(L"SFGame.exe", exeFullPath);

		if (Flag == TRUE)
		{
			g_Button2_Flag = FALSE;
			AfxMessageBox(L"卸载成功");
		}
	}
	UpdateData(FALSE);
}


void CTFHackProDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	MyThread2 = AfxBeginThread(AddAmmo, NULL);
	UpdateData(FALSE);
}


void CTFHackProDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	MyThread3 = AfxBeginThread(ZeroRecoil, NULL);
	UpdateData(FALSE);
}

BOOL bFlag5 = 0;
HANDLE hThread5;
void CTFHackProDlg::OnBnClickedButton5()
{
	UpdateData(TRUE);
	//MyThread4 = AfxBeginThread(ESPBox, NULL);
	if (bFlag5 == 0)
	{
		hThread5 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ESPBox, NULL, 0, 0);
		bFlag5 = 1;
	}
	else
	{
		TerminateThread(hThread5, 0);
		CloseHandle(hThread5);
		bFlag5 = 0;
	}
	UpdateData(FALSE);
}

UINT KeyEmulate(LPVOID lpParam)
{
	while (1)
	{
		if (GetAsyncKeyState(VK_RBUTTON) < 0)
		{
			keybd_event(VK_LBUTTON, MapVirtualKey(VK_LBUTTON, 0), 0, 0);
			Sleep(10);
			keybd_event(VK_LBUTTON, MapVirtualKey(VK_LBUTTON, 0), KEYEVENTF_KEYUP, 0);
		}
		Sleep(10);
	}
	return 0;
}

BOOL bFlag6 = 0;
HANDLE hThread6;

void CTFHackProDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (bFlag6 == 0)
	{
		hThread6 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)KeyEmulate, NULL,0,0);
		bFlag6 = 1;
	}
	else
	{
		TerminateThread(hThread6,0);
		CloseHandle(hThread6);
		bFlag6 = 0;
	}
	UpdateData(FALSE);
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
		//Sleep(1000);
	}
	return 0;
}

BOOL g_bFlag7 = FALSE;
HANDLE hThread7;

void CTFHackProDlg::OnBnClickedButton8()
{
	if (g_bFlag7)
	{
		TerminateThread(hThread7, 0);
		CloseHandle(hThread7);
		g_bFlag7 = FALSE;
	}
	else
	{
		hThread7 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BigJump, NULL, 0, 0);
		g_bFlag7 = TRUE;

	}
}


UINT LocalFly()
{
	//AllocConsole();
	//SetConsoleTitle(L"LocalFly");
	//FILE* stream;
	//freopen_s(&stream, "CON", "w", stdout);
	//printf("ok\n");

	DWORD dwTemp = 0;
	float Accelerate = 0;
	SIZE_T qwRet = 0;
	DWORD dwPid = GetProcessIdByProcessName(L"SFGame.exe");
	HANDLE hProcess = NULL;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess==INVALID_HANDLE_VALUE ||hProcess==NULL)
	{
		AfxMessageBox(L"打开进程失败，请以管理员权限运行");
		return -1;
	}
	else
	{
		AfxMessageBox(L"本功能仅在本地可用\n\n空格一键上天\nX键飞天\nAlt键落地缓冲\n");
	}
	DWORD dwBaseAddr = GetProcessModuleBaseAddress(dwPid, L"SFGame.exe");
	while (true)
	{
		if (GetAsyncKeyState(VK_SPACE))
		{
			ReadProcessMemory(hProcess, (LPCVOID)(dwBaseAddr + 0x013ED5C8), &dwTemp, 4, &qwRet);
			ReadProcessMemory(hProcess, (LPCVOID)(dwTemp + 0x188), &Accelerate, 4, &qwRet);
			Accelerate += 2;
			WriteProcessMemory(hProcess, (LPVOID)(dwTemp + 0x188), &Accelerate, 4, &qwRet);
			//printf("%f\n", Accelerate);
			Sleep(0);
		}
		if (GetAsyncKeyState(0x58))
		{
			ReadProcessMemory(hProcess, (LPCVOID)(dwBaseAddr + 0x013ED5C8), &dwTemp, 4, &qwRet);
			Accelerate = 500;
			WriteProcessMemory(hProcess, (LPVOID)(dwTemp + 0x188), &Accelerate, 4, &qwRet);
			//printf("%f\n", Accelerate);
		}
		if (GetAsyncKeyState(VK_MENU))
		{
			ReadProcessMemory(hProcess, (LPCVOID)(dwBaseAddr + 0x013ED5C8), &dwTemp, 4, &qwRet);
			Accelerate = -100;
			WriteProcessMemory(hProcess, (LPVOID)(dwTemp + 0x188), &Accelerate, 4, &qwRet);
			//printf("%f\n", Accelerate);

		}
	}
	return 0;
}

BOOL g_bFlag8 = FALSE;
HANDLE hThread8;

void CTFHackProDlg::OnBnClickedButton9()
{
	if (g_bFlag8)
	{
		TerminateThread(hThread8, 0);
		CloseHandle(hThread7);
		g_bFlag8 = FALSE;
	}
	else
	{
		hThread8 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LocalFly, NULL, 0, 0);
		g_bFlag8 = TRUE;

	}
}
