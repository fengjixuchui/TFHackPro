#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dwmapi.h>

#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")

/*
D3D��ص�һЩ����
���������static������ȫ�ֱ����Ļ���ǧ�����ͷ�ļ���������cpp�ļ�������
����ͷ�ļ�extern ������ ������
*/
static MARGINS Margin;
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
static ID3DXLine* pLine = 0;
static ID3DXFont* Font;


static HWND PluginHwnd, GameHwnd;
static RECT ���ھ���;

static int ���ڿ�, ���ڸ�;

//ע�ᴰ����Ҫ�õ��Ĵ�����
static WNDCLASSEX wClass;


//�����Σ�����֮��ĵ����������������
typedef void(*Draw)();
static Draw Render;


//������Ϣ������
LRESULT WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

bool InitD3D();

void CreateTransparentWnd(HWND ��Ϸ���ھ��, Draw ���ƺ���);

void ������Ϣѭ��();

void DrawLine(D3DCOLOR Color, float X1, float Y1, float X2, float Y2, float Width);

void DrawString(float X, float Y, const char* Str, D3DCOLOR Color);

void DrawBox(float X, float Y, float W, float H, float Width, D3DCOLOR Color);

void BeginDraw();

void EndDraw();

void ����();

void ��ʼ();