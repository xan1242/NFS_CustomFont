#pragma once
#ifndef GAME_UG2
#define GAME_UG2
#endif

#define CREATERESOURCEFILE_ADDR 0x0057CEF0
#define RESFILE_BEGINLOADING_ADDR 0x0057BD70
#define SERVICERESOURCELOADING_ADDR 0x0057EF60

#define FERENDEROBJECT_ADDPOLY_ADDR 0x0051BFC0
#define FENGFONT_RENDERSTRING_ADDR 0x0051C110

#define FENGFONT_GETLINEWIDTH_ADDR 0x0050BCE0
#define FENGFONT_GETTEXTHEIGHT_ADDR 0x0050BF40
#define FENGFONT_GETCHARACTERWIDTH_ADDR 0x0050BC10

#define FINDFONT_ADDR 0x0051BD70

#define LOADERFENGFONT_ADDR 0x0052CD00
#define LOADERFENGFONT_VTABLE_ADDR 0x007FA858

#define UNLOADERFENGFONT_ADDR 0x00537290
#define UNLOADERFENGFONT_VTABLE_ADDR 0x007FA8E8

#define LOADGLOBALACHUNKS_CALL_ADDR 0x0057ED8F

#define FENGFONT_HASH_OFFSET 0x18
#define FENGFONT_HEIGHT_OFFSET 0x24

#define ADDPOLY_HOOK_ADDR 0x0051C470

#define RENDERSTR_HOOK_ADDR 0x00537085

#define GETLINEWIDTH_HOOK_ADDR_1 0x004B5198
#define GETLINEWIDTH_HOOK_ADDR_2 0x004BDB3D
#define GETLINEWIDTH_HOOK_ADDR_3 0x0050BECF
#define GETLINEWIDTH_HOOK_ADDR_4 0x0050BF09
#define GETLINEWIDTH_HOOK_ADDR_5 0x004BDBB4
#define GETLINEWIDTH_HOOK_ADDR_6 0x00540BAA

//#define GETLINEWIDTH_HOOK_ADDR_RENDERSTR 0x005A4532


#define GETTEXTHEIGHT_HOOK_ADDR_1 0x0051D0F7
#define GETTEXTHEIGHT_HOOK_ADDR_2 0x0051D230
#define GETTEXTHEIGHT_HOOK_ADDR_3 0x0051D429
#define GETTEXTHEIGHT_HOOK_ADDR_4 0x0051D55E
#define GETTEXTHEIGHT_HOOK_ADDR_5 0x0051D699
#define GETTEXTHEIGHT_HOOK_ADDR_6 0x0051D7A1
#define GETTEXTHEIGHT_HOOK_ADDR_7 0x0051D944

// DialogInterface::CalcFEngDialogPackageName
#define FINDFONT_HOOK_ADDR_1 0x00526CCA
// cFEngRender::RenderString
#define FINDFONT_HOOK_ADDR_2 0x00536F7C

#define GETCHARACTERWIDTH_HOOK_ADDR_1 0x0049F9AA
#define GETCHARACTERWIDTH_HOOK_ADDR_2 0x005201C3

#define FONT_SCALE_VAL_ADDR_1 0x536FC6
#define FONT_SCALE_VAL_ADDR_2 0x799360

void(__thiscall* FEngFont_RenderString)(unsigned int dis, void* Color, wchar_t* pcString, void* obj, void* matrix, void* cached, void* PackageRenderInfo) = (void(__thiscall*)(unsigned int, void*, wchar_t*, void*, void*, void*, void*))FENGFONT_RENDERSTRING_ADDR;

float(__thiscall* FEngFont_GetLineWidth)(unsigned int dis, wchar_t* pcString, unsigned int flags, unsigned int maxWidth, bool* word_warp) = (float(__thiscall*)(unsigned int, wchar_t*, unsigned int, unsigned int, bool*))FENGFONT_GETLINEWIDTH_ADDR;
float(__thiscall* FEngFont_GetCharacterWidth)(unsigned int dis, wchar_t chr, wchar_t prevchr, unsigned int flags) = (float(__thiscall*)(unsigned int, wchar_t, wchar_t, unsigned int))FENGFONT_GETCHARACTERWIDTH_ADDR;
float(__thiscall* FEngFont_GetTextHeight)(unsigned int dis, wchar_t* pcString, int flags) = (float(__thiscall*)(unsigned int, wchar_t*, int))FENGFONT_GETTEXTHEIGHT_ADDR;

void* (__cdecl* FindFont)(unsigned int hash) = (void* (__cdecl*)(unsigned int))FINDFONT_ADDR;

void* (*CreateResourceFile)(char* filename, int ResFileType, int unk1, int unk2, int unk3) = (void* (*)(char*, int, int, int, int))CREATERESOURCEFILE_ADDR;
void(__thiscall* ResourceFile_BeginLoading)(void* ResourceFile, void* callback, void* unk) = (void(__thiscall*)(void*, void*, void*))RESFILE_BEGINLOADING_ADDR;
void(*ServiceResourceLoading)() = (void(*)())SERVICERESOURCELOADING_ADDR;
