#pragma once

#pragma once
#ifndef GAME_CARBON
#define GAME_CARBON
#endif

#define CREATERESOURCEFILE_ADDR 0x006B32C0
#define RESFILE_BEGINLOADING_ADDR 0x006B5910
#define SERVICERESOURCELOADING_ADDR 0x006B68D0

#define FERENDEROBJECT_ADDPOLY_ADDR 0x00584700
#define FENGFONT_RENDERSTRING_ADDR 0x00599CC0
#define FENGFONT_PRINTCHARACTER_ADDR 0x00585D10
#define FENGFONT_GETLINEWIDTH_ADDR 0x00586150
#define FENGFONT_GETTEXTHEIGHT_ADDR 0x00591FA0
#define FENGFONT_GETCHARACTERWIDTH_ADDR 0x00586070
#define FENGFONT_GETNEXTWORDWIDTH_ADDR 0x00591F30
#define FINDFONT_ADDR 0x00585B30

#define LOADERFENGFONT_ADDR 0x00591E50
#define LOADERFENGFONT_VTABLE_ADDR 0x00A62B78

#define UNLOADERFENGFONT_ADDR 0x00599C70
#define UNLOADERFENGFONT_VTABLE_ADDR 0x00A62BD8

#define LOADGLOBALACHUNKS_CALL_ADDR 0x006B772C

#define FENGFONT_HASH_OFFSET 0x20
#define FENGFONT_HEIGHT_OFFSET 0x2C

#define ADDPOLY_HOOK_ADDR 0x00585E30
#define PRINTCHAR_HOOK_ADDR 0x0059A078
#define RENDERSTR_HOOK_ADDR 0x005A199A

//#define GETLINEWIDTH_HOOK_ADDR_1 0x0052429D
//#define GETLINEWIDTH_HOOK_ADDR_2 0x00524314
#define GETLINEWIDTH_HOOK_ADDR_3 0x0058624F
#define GETLINEWIDTH_HOOK_ADDR_4 0x00586289
#define GETLINEWIDTH_HOOK_ADDR_5 0x005AFFEC
#define GETLINEWIDTH_HOOK_ADDR_6 0x005B005C
//#define GETLINEWIDTH_HOOK_ADDR_7 0x0058FBBF

#define GETLINEWIDTH_HOOK_ADDR_RENDERSTR 0x005A192B


#define GETTEXTHEIGHT_HOOK_ADDR_1 0x59215E
#define GETTEXTHEIGHT_HOOK_ADDR_2 0x5956A6
#define GETTEXTHEIGHT_HOOK_ADDR_3 0x59741D
#define GETTEXTHEIGHT_HOOK_ADDR_4 0x59762E
#define GETTEXTHEIGHT_HOOK_ADDR_5 0x5977AC
#define GETTEXTHEIGHT_HOOK_ADDR_6 0x5979C2
#define GETTEXTHEIGHT_HOOK_ADDR_7 0x597B12
#define GETTEXTHEIGHT_HOOK_ADDR_8 0x597C40
#define GETTEXTHEIGHT_HOOK_ADDR_9 0x597DB2
#define GETTEXTHEIGHT_HOOK_ADDR_10 0x597E0E
#define GETTEXTHEIGHT_HOOK_ADDR_11 0x5A1095
#define GETTEXTHEIGHT_HOOK_ADDR_12 0x5C54C9

//#define FINDFONT_HOOK_ADDR_1 0x0058F972
//#define FINDFONT_HOOK_ADDR_2 0x54919A

#define GETCHARACTERWIDTH_HOOK_ADDR_1 0x005931F3

#define FESTRING_SIZE_ADDR_1 0x005FA4CA
#define FESTRING_SIZE_ADDR_2 0x005FF480

#define GAME_D3D9_DEVICE *(LPDIRECT3DDEVICE9*)0x00982BDC;

void* (__thiscall* FERenderObject_AddPoly)(unsigned int dis, float x0, float y0, float x1, float y1, float z, float s0, float t0, float s1, float t1, unsigned int* colors) = (void* (__thiscall*)(unsigned int, float, float, float, float, float, float, float, float, float, unsigned int*))FERENDEROBJECT_ADDPOLY_ADDR;
void(__thiscall* FEngFont_PrintCharacter)(unsigned int dis, void* pGlyph, float fX, float fY, unsigned int* render_colors, void* cached) = (void(__thiscall*)(unsigned int, void*, float, float, unsigned int*, void*))FENGFONT_PRINTCHARACTER_ADDR;
void(__thiscall* FEngFont_RenderString)(unsigned int dis, void* Color, wchar_t* pcString, void* obj, void* matrix, void* cached) = (void(__thiscall*)(unsigned int, void*, wchar_t*, void*, void*, void*))FENGFONT_RENDERSTRING_ADDR;

float(__thiscall* FEngFont_GetLineWidth)(unsigned int dis, wchar_t* pcString, unsigned int flags, unsigned int maxWidth, bool word_warp) = (float(__thiscall*)(unsigned int, wchar_t*, unsigned int, unsigned int, bool))FENGFONT_GETLINEWIDTH_ADDR;
float(__thiscall* FEngFont_GetCharacterWidth)(unsigned int dis, wchar_t chr, wchar_t prevchr, unsigned int flags) = (float(__thiscall*)(unsigned int, wchar_t, wchar_t, unsigned int))FENGFONT_GETCHARACTERWIDTH_ADDR;
float(__thiscall* FEngFont_GetTextHeight)(unsigned int dis, wchar_t* pcString, int ilLeading, unsigned int flags, unsigned int maxWidth, bool word_warp) = (float(__thiscall*)(unsigned int, wchar_t*, int, unsigned int, unsigned int, bool))FENGFONT_GETTEXTHEIGHT_ADDR;
float(__thiscall* FEngFont_GetNextWordWidth)(unsigned int dis, wchar_t* pcString, unsigned int flags) = (float(__thiscall*)(unsigned int, wchar_t*, unsigned int))FENGFONT_GETNEXTWORDWIDTH_ADDR;

void* (__cdecl* FindFont)(unsigned int hash) = (void* (__cdecl*)(unsigned int))FINDFONT_ADDR;

void* (*CreateResourceFile)(char* filename, int ResFileType, int unk1, int unk2, int unk3) = (void* (*)(char*, int, int, int, int))CREATERESOURCEFILE_ADDR;
void(__thiscall* ResourceFile_BeginLoading)(void* ResourceFile, void* callback, void* unk) = (void(__thiscall*)(void*, void*, void*))RESFILE_BEGINLOADING_ADDR;
void(*ServiceResourceLoading)() = (void(*)())SERVICERESOURCELOADING_ADDR;
