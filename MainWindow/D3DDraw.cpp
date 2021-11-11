#include "D3DDraw.h"

static D3DCOLOR RedColor = D3DCOLOR_ARGB(255, 255, 255, 255);
float 线粗 = 2;


bool InitD3D()
{
	/*
	D3D这玩意比较复杂，如果单纯是想搞点辅助什么的，复制粘贴我的足够了，
	如果想深入学习，可能得另找资料了，下面的这些基本是固定的，想知道是用来干啥的
	可以自行百度，我这个人比较懒。。。
	*/

	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	// 创建D3D设备
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, PluginHwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
		return false;

	if (pLine == NULL)
		D3DXCreateLine(g_pd3dDevice, &pLine);

	//创建D3D字体
	D3DXCreateFontW(g_pd3dDevice, 16, 0, FW_DONTCARE, D3DX_DEFAULT, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Vernada", &Font);
	return true;
}

void CreateTransparentWnd(HWND 游戏窗口句柄, Draw 绘制函数)
{
	if (绘制函数 == NULL || 游戏窗口句柄 == 0) return;

	GameHwnd = 游戏窗口句柄;
	Render = 绘制函数;

	//初始化窗口类
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wClass.hInstance = GetModuleHandle(NULL);
	wClass.lpfnWndProc = (WNDPROC)WinProc;
	wClass.lpszClassName = L" ";
	wClass.lpszMenuName = L" ";
	wClass.style = CS_VREDRAW | CS_HREDRAW;

	//注册窗口
	if (RegisterClassEx(&wClass) == 0)
	{
		MessageBox(NULL, L"创建窗口出错！", L"提示！", 0);
		exit(1);
	}

	//创建窗口
	GetWindowRect(GameHwnd, &窗口矩形);
	窗口宽 = 窗口矩形.right - 窗口矩形.left;
	窗口高 = 窗口矩形.bottom - 窗口矩形.top;
	PluginHwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, L" ", L" ", WS_POPUP, 1, 1, 窗口宽, 窗口高, 0, 0, 0, 0);

	//显示窗口
	SetLayeredWindowAttributes(PluginHwnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
	ShowWindow(PluginHwnd, SW_SHOW);

	InitD3D();
}

void 窗口消息循环()
{
	while (1)
	{
		//使辅助窗口一直盖在游戏窗口上
		if (GameHwnd)
		{
			GetWindowRect(GameHwnd, &窗口矩形);
			窗口宽 = 窗口矩形.right - 窗口矩形.left;
			窗口高 = 窗口矩形.bottom - 窗口矩形.top;
			DWORD dwStyle = GetWindowLong(GameHwnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				窗口矩形.top += 23;
				窗口高 -= 23;
			}
			MoveWindow(PluginHwnd, 窗口矩形.left, 窗口矩形.top, 窗口宽, 窗口高, true);
		}

		//处理窗口消息
		MSG Message;
		ZeroMemory(&Message, sizeof(Message));
		if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Message);
			TranslateMessage(&Message);
		}
		Sleep(1);
	}

	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
	CloseWindow(PluginHwnd);

	::UnregisterClass(wClass.lpszClassName, wClass.hInstance);
}

LRESULT WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
		if (g_pd3dDevice)
			Render();//就是这里调用了我们的画框画线之类的函数
		break;

	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hWnd, &Margin);
		break;

	case WM_DESTROY:
	{
		g_pD3D->Release();
		g_pd3dDevice->Release();
		exit(1);
		return 0;
	}
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void DrawLine(D3DCOLOR Color, float X1, float Y1, float X2, float Y2, float Width)
{
	D3DXVECTOR2 Vertex[2] = { {X1,Y1},{X2,Y2} };
	pLine->SetWidth(Width);
	pLine->Draw(Vertex, 2, Color);
}

void DrawString(float X, float Y, const char* Str, D3DCOLOR Color)
{
	RECT Rect = { (LONG)X,(LONG)Y };
	Font->DrawTextA(NULL, Str, -1, &Rect, DT_CALCRECT, Color);
	Font->DrawTextA(NULL, Str, -1, &Rect, DT_LEFT, Color);
}

void DrawBox(float X, float Y, float W, float H, float Width, D3DCOLOR Color)
{
	D3DXVECTOR2 Vertex[5] = { {X,Y},{X + W,Y},{X + W,Y + H},{X,Y + H},{X,Y} };
	pLine->SetWidth(Width);
	pLine->Draw(Vertex, 5, Color);
}

void BeginDraw()
{
	g_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	g_pd3dDevice->BeginScene();
}

void EndDraw()
{
	g_pd3dDevice->EndScene();
	g_pd3dDevice->Present(0, 0, 0, 0);
}


void 绘制()
{
	BeginDraw();
	DrawLine(D3DCOLOR_ARGB(255, 0, 0, 255), 20, 20, 66, 66, 线粗);
	DrawBox(100, 100, 100, 100, 线粗, D3DCOLOR_ARGB(255, 255, 255, 0));
	DrawString(200, 200, "TFHackPro", D3DCOLOR_ARGB(255, 255, 0, 255));
	EndDraw();
}

void 开始()
{
	GameHwnd = FindWindow(NULL, L"风暴战区TF");
	CreateTransparentWnd(GameHwnd, 绘制);
	窗口消息循环();
}