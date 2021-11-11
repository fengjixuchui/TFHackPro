#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dwmapi.h>

#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")

/*
D3D相关的一些东西
如果不想用static变量用全局变量的话，千万别再头文件声明，在cpp文件声明后
再在头文件extern 类型名 变量名
*/
static MARGINS Margin;
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
static ID3DXLine* pLine = 0;
static ID3DXFont* Font;


static HWND PluginHwnd, GameHwnd;
static RECT 窗口矩形;

static int 窗口宽, 窗口高;

//注册窗口需要用到的窗口类
static WNDCLASSEX wClass;


//画矩形，文字之类的单独放在这个函数里
typedef void(*Draw)();
static Draw Render;


//窗口消息处理函数
LRESULT WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

bool InitD3D();

void CreateTransparentWnd(HWND 游戏窗口句柄, Draw 绘制函数);

void 窗口消息循环();

void DrawLine(D3DCOLOR Color, float X1, float Y1, float X2, float Y2, float Width);

void DrawString(float X, float Y, const char* Str, D3DCOLOR Color);

void DrawBox(float X, float Y, float W, float H, float Width, D3DCOLOR Color);

void BeginDraw();

void EndDraw();

void 绘制();

void 开始();