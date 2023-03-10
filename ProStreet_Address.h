#pragma once

#ifndef GAME_PS
#define GAME_PS
#endif

#define CREATERESOURCEFILE_ADDR 0x006D6DE0
#define RESFILE_BEGINLOADING_ADDR 0x006D9430
#define SERVICERESOURCELOADING_ADDR 0x006D9E70

#define FERENDEROBJECT_ADDPOLY_ADDR 0x005A0100
#define FENGFONT_RENDERSTRING_ADDR 0x005B6830
#define FENGFONT_PRINTCHARACTER_ADDR 0x005A1720
#define FENGFONT_PRINTCHARACTERINWORLD_ADDR 0x005A1970
#define FENGFONT_GETLINEWIDTH_ADDR 0x005A1C60
#define FENGFONT_GETTEXTHEIGHT_ADDR 0x005AD710
#define FENGFONT_GETCHARACTERWIDTH_ADDR 0x005A1BA0
#define FENGFONT_GETNEXTWORDWIDTH_ADDR 0x005AD6A0
#define FINDFONT_ADDR 0x005A1530

#define LOADERFENGFONT_ADDR 0x005AD530
#define LOADERFENGFONT_VTABLE_ADDR 0x00A60D30

#define UNLOADERFENGFONT_ADDR 0x005B67E0
#define UNLOADERFENGFONT_VTABLE_ADDR 0x00A60D88

#define LOADGLOBALACHUNKS_CALL_ADDR 0x006DABD4

#define FENGFONT_HASH_OFFSET 0x20
#define FENGFONT_HEIGHT_OFFSET 0x2C

#define ADDPOLY_HOOK_ADDR 0x005A1881
#define PRINTCHAR_HOOK_ADDR 0x005B6C25
#define PRINTCHARINWORLD_HOOK_ADDR 0x005B6E8E
#define RENDERSTR_HOOK_ADDR 0x005D213C

//#define GETLINEWIDTH_HOOK_ADDR_1 0x0052429D
//#define GETLINEWIDTH_HOOK_ADDR_2 0x00524314
#define GETLINEWIDTH_HOOK_ADDR_3 0x005A1D60
#define GETLINEWIDTH_HOOK_ADDR_4 0x005A1D99
#define GETLINEWIDTH_HOOK_ADDR_5 0x005E2BFC
#define GETLINEWIDTH_HOOK_ADDR_6 0x005E2C6C
//#define GETLINEWIDTH_HOOK_ADDR_7 0x0058FBBF

#define GETLINEWIDTH_HOOK_ADDR_RENDERSTR 0x005D2068


#define GETTEXTHEIGHT_HOOK_ADDR_1 0x5AD8BE
#define GETTEXTHEIGHT_HOOK_ADDR_2 0x5B4A0B
//#define GETTEXTHEIGHT_HOOK_ADDR_3 0x5B6D6F // World RenderString!!!
#define GETTEXTHEIGHT_HOOK_ADDR_4 0x5BD78A
#define GETTEXTHEIGHT_HOOK_ADDR_5 0x5BDA09
#define GETTEXTHEIGHT_HOOK_ADDR_6 0x5BDB9B
#define GETTEXTHEIGHT_HOOK_ADDR_7 0x5BDDDC
#define GETTEXTHEIGHT_HOOK_ADDR_8 0x5BDEF2
#define GETTEXTHEIGHT_HOOK_ADDR_9 0x5BE0C4
#define GETTEXTHEIGHT_HOOK_ADDR_10 0x5BE12B
#define GETTEXTHEIGHT_HOOK_ADDR_12 0x5FEC76

//#define FINDFONT_HOOK_ADDR_1 0x0058F972
//#define FINDFONT_HOOK_ADDR_2 0x54919A

#define GETCHARACTERWIDTH_HOOK_ADDR_1 0x005B76E2

#define GAME_D3D9_DEVICE **(LPDIRECT3DDEVICE9**)0xAC6ED4;
//LPDIRECT3DDEVICE9 gDevice = **(LPDIRECT3DDEVICE9**)dword_AC6ED4;

struct bVector3
{
	float x;
	float y;
	float z;
	float pad;
};

class ePoly
{
public:
	struct bVector3 Vertices[4];
	float UVs[2][4];
	float UVsMask[2][4];
	unsigned char Colours[4][4];
	unsigned char flags;
	unsigned char Flailer;
};

void* (__thiscall* FERenderObject_AddPoly)(unsigned int dis, float x0, float y0, float x1, float y1, float z, float s0, float t0, float s1, float t1, unsigned int* colors) = (void* (__thiscall*)(unsigned int, float, float, float, float, float, float, float, float, float, unsigned int*))FERENDEROBJECT_ADDPOLY_ADDR;

void(__thiscall* FEngFont_PrintCharacter)(unsigned int dis, void* pGlyph, float fX, float fY, unsigned int* render_colors, void* cached, void* pPoly) = (void(__thiscall*)(unsigned int, void*, float, float, unsigned int*, void*, void*))FENGFONT_PRINTCHARACTER_ADDR;
void(__thiscall* FEngFont_PrintCharacterInWorld)(unsigned int dis, void* pGlyph, float fX, float fY, unsigned int* render_colors, void* cached, void* pPoly) = (void(__thiscall*)(unsigned int, void*, float, float, unsigned int*, void*, void*))FENGFONT_PRINTCHARACTERINWORLD_ADDR;

void(__thiscall* FEngFont_RenderString)(unsigned int dis, void* Color, wchar_t* pcString, void* obj, void* matrix, void* cached) = (void(__thiscall*)(unsigned int, void*, wchar_t*, void*, void*, void*))FENGFONT_RENDERSTRING_ADDR;
float(__thiscall* FEngFont_GetLineWidth)(unsigned int dis, wchar_t* pcString, unsigned int flags, unsigned int maxWidth, bool word_warp) = (float(__thiscall*)(unsigned int, wchar_t*, unsigned int, unsigned int, bool))FENGFONT_GETLINEWIDTH_ADDR;
float(__thiscall* FEngFont_GetCharacterWidth)(unsigned int dis, wchar_t chr, wchar_t prevchr, unsigned int flags) = (float(__thiscall*)(unsigned int, wchar_t, wchar_t, unsigned int))FENGFONT_GETCHARACTERWIDTH_ADDR;
float(__thiscall* FEngFont_GetTextHeight)(unsigned int dis, wchar_t* pcString, int ilLeading, unsigned int flags, unsigned int maxWidth, bool word_warp) = (float(__thiscall*)(unsigned int, wchar_t*, int, unsigned int, unsigned int, bool))FENGFONT_GETTEXTHEIGHT_ADDR;
float(__thiscall* FEngFont_GetNextWordWidth)(unsigned int dis, wchar_t* pcString, unsigned int flags) = (float(__thiscall*)(unsigned int, wchar_t*, unsigned int))FENGFONT_GETNEXTWORDWIDTH_ADDR;

void* (__cdecl* FindFont)(unsigned int hash) = (void* (__cdecl*)(unsigned int))FINDFONT_ADDR;

void* (*CreateResourceFile)(char* filename, int ResFileType, int unk1, int unk2, int unk3) = (void* (*)(char*, int, int, int, int))CREATERESOURCEFILE_ADDR;
void(__thiscall* ResourceFile_BeginLoading)(void* ResourceFile, void* callback, void* unk) = (void(__thiscall*)(void*, void*, void*))RESFILE_BEGINLOADING_ADDR;
void(*ServiceResourceLoading)() = (void(*)())SERVICERESOURCELOADING_ADDR;
