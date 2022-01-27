#include "D3DDraw.h"

static D3DCOLOR RedColor = D3DCOLOR_ARGB(255, 255, 255, 255);
float �ߴ� = 2;


bool InitD3D()
{
	/*
	D3D������Ƚϸ��ӣ�������������㸨��ʲô�ģ�����ճ���ҵ��㹻�ˣ�
	���������ѧϰ�����ܵ����������ˣ��������Щ�����ǹ̶��ģ���֪����������ɶ��
	�������аٶȣ�������˱Ƚ���������
	*/

	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	// ����D3D�豸
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

	//����D3D����
	D3DXCreateFontW(g_pd3dDevice, 16, 0, FW_DONTCARE, D3DX_DEFAULT, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Vernada", &Font);
	return true;
}

void CreateTransparentWnd(HWND ��Ϸ���ھ��, Draw ���ƺ���)
{
	if (���ƺ��� == NULL || ��Ϸ���ھ�� == 0) return;

	GameHwnd = ��Ϸ���ھ��;
	Render = ���ƺ���;

	//��ʼ��������
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

	//ע�ᴰ��
	if (RegisterClassEx(&wClass) == 0)
	{
		MessageBox(NULL, L"�������ڳ���", L"��ʾ��", 0);
		exit(1);
	}

	//��������
	GetWindowRect(GameHwnd, &���ھ���);
	���ڿ� = ���ھ���.right - ���ھ���.left;
	���ڸ� = ���ھ���.bottom - ���ھ���.top;
	PluginHwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, L" ", L" ", WS_POPUP, 1, 1, ���ڿ�, ���ڸ�, 0, 0, 0, 0);

	//��ʾ����
	SetLayeredWindowAttributes(PluginHwnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
	ShowWindow(PluginHwnd, SW_SHOW);

	InitD3D();
}

void ������Ϣѭ��()
{
	while (1)
	{
		//ʹ��������һֱ������Ϸ������
		if (GameHwnd)
		{
			GetWindowRect(GameHwnd, &���ھ���);
			���ڿ� = ���ھ���.right - ���ھ���.left;
			���ڸ� = ���ھ���.bottom - ���ھ���.top;
			DWORD dwStyle = GetWindowLong(GameHwnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				���ھ���.top += 23;
				���ڸ� -= 23;
			}
			MoveWindow(PluginHwnd, ���ھ���.left, ���ھ���.top, ���ڿ�, ���ڸ�, true);
		}

		//��������Ϣ
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
			Render();//����������������ǵĻ�����֮��ĺ���
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


void ����()
{
	BeginDraw();
	DrawLine(D3DCOLOR_ARGB(255, 0, 0, 255), 20, 20, 66, 66, �ߴ�);
	DrawBox(100, 100, 100, 100, �ߴ�, D3DCOLOR_ARGB(255, 255, 255, 0));
	DrawString(200, 200, "TFHackPro", D3DCOLOR_ARGB(255, 255, 0, 255));
	EndDraw();
}

void ��ʼ()
{
	GameHwnd = FindWindow(NULL, L"�籩ս��TF");
	CreateTransparentWnd(GameHwnd, ����);
	������Ϣѭ��();
}