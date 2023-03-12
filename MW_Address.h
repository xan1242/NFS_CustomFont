#pragma once
#ifndef GAME_MW
#define GAME_MW
#endif

#define CREATERESOURCEFILE_ADDR 0x0065FD30
#define RESFILE_BEGINLOADING_ADDR 0x006616F0
#define SERVICERESOURCELOADING_ADDR 0x006626B0

#define FERENDEROBJECT_ADDPOLY_ADDR 0x00582660
#define FENGFONT_RENDERSTRING_ADDR 0x0059AC60
#define FENGFONT_PRINTCHARACTER_ADDR 0x00585D60
#define FENGFONT_GETLINEWIDTH_ADDR 0x00586130
#define FENGFONT_GETTEXTHEIGHT_ADDR 0x00591FD0
#define FENGFONT_GETCHARACTERWIDTH_ADDR 0x00586050
#define FENGFONT_GETNEXTWORDWIDTH_ADDR 0x00591F60
#define FINDFONT_ADDR 0x00585B50

#define LOADERFENGFONT_ADDR 0x00591EB0
#define LOADERFENGFONT_VTABLE_ADDR 0x008F7DD8

#define UNLOADERFENGFONT_ADDR 0x0059AC10
#define UNLOADERFENGFONT_VTABLE_ADDR 0x008F7E40

#define LOADGLOBALACHUNKS_CALL_ADDR 0x006660B6

#define FENGFONT_HASH_OFFSET 0x18
#define FENGFONT_HEIGHT_OFFSET 0x24

#define ADDPOLY_HOOK_ADDR 0x00585E76
#define PRINTCHAR_HOOK_ADDR 0x0059B01D
#define RENDERSTR_HOOK_ADDR 0x005A45A1

#define GETLINEWIDTH_HOOK_ADDR_1 0x0052429D
#define GETLINEWIDTH_HOOK_ADDR_2 0x00524314
#define GETLINEWIDTH_HOOK_ADDR_3 0x0058621F
#define GETLINEWIDTH_HOOK_ADDR_4 0x00586259
#define GETLINEWIDTH_HOOK_ADDR_5 0x0058DF00
#define GETLINEWIDTH_HOOK_ADDR_6 0x0058DF78
#define GETLINEWIDTH_HOOK_ADDR_7 0x0058FBBF

#define GETLINEWIDTH_HOOK_ADDR_RENDERSTR 0x005A4532


#define GETTEXTHEIGHT_HOOK_ADDR_1 0x005150A5
#define GETTEXTHEIGHT_HOOK_ADDR_2 0x00515205
#define GETTEXTHEIGHT_HOOK_ADDR_3 0x00515592
#define GETTEXTHEIGHT_HOOK_ADDR_4 0x00524D85
#define GETTEXTHEIGHT_HOOK_ADDR_5 0x00524FA2
#define GETTEXTHEIGHT_HOOK_ADDR_6 0x005250F4

// UIOLGameRoom::ChatBuffer::WordWrap
#define FINDFONT_HOOK_ADDR_1 0x0058F972
// CTextScroller::WordWrapAddLines
#define FINDFONT_HOOK_ADDR_2 0x54919A

#define GETCHARACTERWIDTH_HOOK_ADDR_1 0x0051368A
#define GETCHARACTERWIDTH_HOOK_ADDR_2 0x00589C53

#define GAME_D3D9_DEVICE *(LPDIRECT3DDEVICE9*)0x00982BDC;

void* (__thiscall* FERenderObject_AddPoly)(unsigned int dis, float x0, float y0, float x1, float y1, float z, float s0, float t0, float s1, float t1, unsigned int* colors, int res) = (void* (__thiscall*)(unsigned int, float, float, float, float, float, float, float, float, float, unsigned int*, int))FERENDEROBJECT_ADDPOLY_ADDR;
void(__thiscall* FEngFont_PrintCharacter)(unsigned int dis, void* pGlyph, float fX, float fY, unsigned int* render_colors, void* cached, void* pPoly) = (void(__thiscall*)(unsigned int, void*, float, float, unsigned int*, void*, void*))FENGFONT_PRINTCHARACTER_ADDR;

void(__thiscall* FEngFont_RenderString)(unsigned int dis, void* Color, wchar_t* pcString, void* obj, void* matrix, void* cached) = (void(__thiscall*)(unsigned int, void*, wchar_t*, void*, void*, void*))FENGFONT_RENDERSTRING_ADDR;

float(__thiscall* FEngFont_GetLineWidth)(unsigned int dis, wchar_t* pcString, unsigned int flags, unsigned int maxWidth, bool word_warp) = (float(__thiscall*)(unsigned int, wchar_t*, unsigned int, unsigned int, bool))FENGFONT_GETLINEWIDTH_ADDR;
float(__thiscall* FEngFont_GetCharacterWidth)(unsigned int dis, wchar_t chr, wchar_t prevchr, unsigned int flags) = (float(__thiscall*)(unsigned int, wchar_t, wchar_t, unsigned int))FENGFONT_GETCHARACTERWIDTH_ADDR;
float(__thiscall* FEngFont_GetTextHeight)(unsigned int dis, wchar_t* pcString, int ilLeading, unsigned int flags, unsigned int maxWidth, bool word_warp) = (float(__thiscall*)(unsigned int, wchar_t*, int, unsigned int, unsigned int, bool))FENGFONT_GETTEXTHEIGHT_ADDR;
float(__thiscall* FEngFont_GetNextWordWidth)(unsigned int dis, wchar_t* pcString, unsigned int flags) = (float(__thiscall*)(unsigned int, wchar_t*, unsigned int))FENGFONT_GETNEXTWORDWIDTH_ADDR;

void* (__cdecl* FindFont)(unsigned int hash) = (void* (__cdecl*)(unsigned int))FINDFONT_ADDR;

void* (*CreateResourceFile)(const char* filename, int ResFileType, int unk1, int unk2, int unk3) = (void* (*)(const char*, int, int, int, int))CREATERESOURCEFILE_ADDR;
void(__thiscall* ResourceFile_BeginLoading)(void* ResourceFile, void* callback, void* unk) = (void(__thiscall*)(void*, void*, void*))RESFILE_BEGINLOADING_ADDR;
void(*ServiceResourceLoading)() = (void(*)())SERVICERESOURCELOADING_ADDR;
