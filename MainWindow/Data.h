#pragma once
#include "Windows.h"

DWORD WIDTH;
DWORD HEIGHT;

//struct Vec3 { float x, y, z; };
struct Vec3
{
	float x = 0;
	float y = 0;
	float z = 0;
};
struct Vec4 { float x, y, z, w; };
struct Vec2 { float x, y; };


DWORD SFGameBaseAddress = 0;
HANDLE hGameProcess = 0;
DWORD GamePid = 0;

DWORD InfiniteAmmoOffset = 0x9612F0;

DWORD AntiRecoliOffset_1 = 0x974FEF;
DWORD AntiRecoliOffset_2 = 0x975146;
DWORD AntiRecoliOffset_3 = 0x9752E2;
DWORD AntiRecoliOffset_4 = 0x9752EF;
DWORD AntiRecoliOffset_5 = 0x9752F1;
DWORD AntiRecoliOffset_6 = 0x9752F9;