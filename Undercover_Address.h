#pragma once

#ifndef GAME_UC
#define GAME_UC
#endif

//#define CREATERESOURCEFILE_ADDR 0x006AC350
//#define RESFILE_BEGINLOADING_ADDR 0x006AE570
#define LOADRESFILE_ADDR 0x006AE5F0
#define GLOBALCATEGORY_ADDR 0x00D3BD98
#define FECATEGORY_ADDR 0x00D3BD9C

#define FERENDEROBJECT_ADDPOLY_ADDR 0x0054F7E0
#define FENGFONT_RENDERSTRING_ADDR 0x0057D8C0
#define FENGFONT_PRINTCHARACTER_ADDR 0x00550520
#define FENGFONT_PRINTCHARACTERINWORLD_ADDR 0x005506C0
#define FENGFONT_GETLINEWIDTH_ADDR 0x00562B20
#define FENGFONT_GETTEXTHEIGHT_ADDR 0x005715F0
#define FENGFONT_GETCHARACTERWIDTH_ADDR 0x00562A40
#define FENGFONT_GETNEXTWORDWIDTH_ADDR 0x00571580
#define FINDFONT_ADDR 0x00562830

#define LOADERFENGFONT_ADDR 0x005714E0
#define LOADERFENGFONT_VTABLE_ADDR 0x00D3BCE0

#define UNLOADERFENGFONT_ADDR 0x0057D870
#define UNLOADERFENGFONT_VTABLE_ADDR 0x00D3BD38

#define LOADGLOBALACHUNKS_CALL_ADDR 0x006AF038

#define FENGFONT_HASH_OFFSET 0x20
#define FENGFONT_HEIGHT_OFFSET 0x2C

#define ADDPOLY_HOOK_ADDR 0x005506A4
#define PRINTCHAR_HOOK_ADDR 0x0057DD9F
#define PRINTCHARINWORLD_HOOK_ADDR 0x0057E003
#define RENDERSTR_HOOK_ADDR 0x005A418C

//#define GETLINEWIDTH_HOOK_ADDR_1 0x0052429D
//#define GETLINEWIDTH_HOOK_ADDR_2 0x00524314
#define GETLINEWIDTH_HOOK_ADDR_3 0x00562C1F 
#define GETLINEWIDTH_HOOK_ADDR_4 0x00562C59
//#define GETLINEWIDTH_HOOK_ADDR_5 0x005E2BFC
//#define GETLINEWIDTH_HOOK_ADDR_6 0x005E2C6C
//#define GETLINEWIDTH_HOOK_ADDR_7 0x0058FBBF

#define GETLINEWIDTH_HOOK_ADDR_RENDERSTR 0x005A408C


#define GETTEXTHEIGHT_HOOK_ADDR_1 0x0057D06B
#define GETTEXTHEIGHT_HOOK_ADDR_2 0x005954AF
#define GETTEXTHEIGHT_HOOK_ADDR_4 0x00595770
#define GETTEXTHEIGHT_HOOK_ADDR_5 0x005959CC
#define GETTEXTHEIGHT_HOOK_ADDR_6 0x00595AE2
#define GETTEXTHEIGHT_HOOK_ADDR_7 0x00595CEA
#define GETTEXTHEIGHT_HOOK_ADDR_8 0x00595D51


//#define GETCHARACTERWIDTH_HOOK_ADDR_1 0x005B76E2

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

void(__thiscall* FEngFont_PrintCharacter)(unsigned int dis, void* pGlyph, float fX, float fY, unsigned int* render_colors, void* cached) = (void(__thiscall*)(unsigned int, void*, float, float, unsigned int*, void*))FENGFONT_PRINTCHARACTER_ADDR;
void(__thiscall* FEngFont_PrintCharacterInWorld)(unsigned int dis, void* pGlyph, float fX, float fY, unsigned int* render_colors, void* cached, void* pPoly) = (void(__thiscall*)(unsigned int, void*, float, float, unsigned int*, void*, void*))FENGFONT_PRINTCHARACTERINWORLD_ADDR;
void(__thiscall* FEngFont_RenderString)(unsigned int dis, void* Color, wchar_t* pcString, void* obj, void* matrix, void* cached) = (void(__thiscall*)(unsigned int, void*, wchar_t*, void*, void*, void*))FENGFONT_RENDERSTRING_ADDR;
float(__thiscall* FEngFont_GetLineWidth)(unsigned int dis, wchar_t* pcString, unsigned int flags, unsigned int maxWidth, bool word_warp) = (float(__thiscall*)(unsigned int, wchar_t*, unsigned int, unsigned int, bool))FENGFONT_GETLINEWIDTH_ADDR;
float(__thiscall* FEngFont_GetCharacterWidth)(unsigned int dis, wchar_t chr, wchar_t prevchr, unsigned int flags) = (float(__thiscall*)(unsigned int, wchar_t, wchar_t, unsigned int))FENGFONT_GETCHARACTERWIDTH_ADDR;
float(__thiscall* FEngFont_GetTextHeight)(unsigned int dis, wchar_t* pcString, int ilLeading, unsigned int flags, unsigned int maxWidth, bool word_warp) = (float(__thiscall*)(unsigned int, wchar_t*, int, unsigned int, unsigned int, bool))FENGFONT_GETTEXTHEIGHT_ADDR;
float(__thiscall* FEngFont_GetNextWordWidth)(unsigned int dis, wchar_t* pcString, unsigned int flags) = (float(__thiscall*)(unsigned int, wchar_t*, unsigned int))FENGFONT_GETNEXTWORDWIDTH_ADDR;

void* (__cdecl* FindFont)(unsigned int hash) = (void* (__cdecl*)(unsigned int))FINDFONT_ADDR;

void* (*LoadResourceFile)(char* filename, int ResType, int flags, int mem_category, void* callback, void* callback_param, int file_offset, int file_size) = (void* (*)(char*, int, int, int, void*, void*, int, int))LOADRESFILE_ADDR;
void ServiceResourceLoading()
{
	return;
}
