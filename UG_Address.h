#pragma once
#ifndef GAME_UG
#define GAME_UG
#endif

#define CREATERESOURCEFILE_ADDR 0x004482F0
#define RESFILE_BEGINLOADING_ADDR 0x00448110
#define SERVICERESOURCELOADING_ADDR 0x004483C0

#define FENGFONT_RENDERSTRING_ADDR 0x004F48B0

#define FENGFONT_GETLINEWIDTH_ADDR 0x004F4D00
#define FENGFONT_GETTEXTHEIGHT_ADDR 0x004F4F10

#define FINDFONT_ADDR 0x004F43D0

#define LOADERFENGFONT_ADDR 0x004F4420
#define LOADERFENGFONT_VTABLE_ADDR 0x007007F4

#define UNLOADERFENGFONT_ADDR 0x004F4470
#define UNLOADERFENGFONT_VTABLE_ADDR 0x007008B4

#define LOADGLOBALACHUNKS_CALL_ADDR 0x00447280

#define FENGFONT_HASH_OFFSET 0x18
#define FENGFONT_HEIGHT_OFFSET 0x24

#define RENDERSTR_HOOK_ADDR 0x004F27BF

#define GETLINEWIDTH_HOOK_ADDR_3 0x004F4EAA
#define GETLINEWIDTH_HOOK_ADDR_4 0x004F4EDC


#define GETTEXTHEIGHT_HOOK_ADDR_1 0x004F6D90
#define GETTEXTHEIGHT_HOOK_ADDR_2 0x004F6EC9
#define GETTEXTHEIGHT_HOOK_ADDR_3 0x004F709C
#define GETTEXTHEIGHT_HOOK_ADDR_4 0x004F71A8
#define GETTEXTHEIGHT_HOOK_ADDR_5 0x004F72BC
#define GETTEXTHEIGHT_HOOK_ADDR_6 0x004F73B2

// cFEngRender::RenderString
#define FINDFONT_HOOK_ADDR_1 0x004F2603

#define FONT_SCALE_VAL_ADDR_1 0x4F2645
#define FONT_SCALE_VAL_ADDR_2 0x6B675C

#define GAME_D3D9_DEVICE *(LPDIRECT3DDEVICE9*)0x073636C;

//void(__thiscall* FEngFont_RenderString)(unsigned int dis, void* Color, wchar_t* pcString, void* obj, void* matrix, void* cached, void* PackageRenderInfo) = (void(__thiscall*)(unsigned int, void*, wchar_t*, void*, void*, void*, void*))FENGFONT_RENDERSTRING_ADDR;

//float(__thiscall* FEngFont_GetLineWidth)(unsigned int dis, wchar_t* pcString, unsigned int flags, unsigned int maxWidth, bool* word_warp) = (float(__thiscall*)(unsigned int, wchar_t*, unsigned int, unsigned int, bool*))FENGFONT_GETLINEWIDTH_ADDR;
float(__stdcall* FEngFont_GetTextHeight_Func)(unsigned int dis, wchar_t* pcString, int flags) = (float(__stdcall*)(unsigned int, wchar_t*, int))FENGFONT_GETTEXTHEIGHT_ADDR;

//void* (__cdecl* FindFont)(unsigned int hash) = (void* (__cdecl*)(unsigned int))FINDFONT_ADDR;

void* (*CreateResourceFile)(char* filename, int ResFileType, int unk1, int unk2, int unk3) = (void* (*)(char*, int, int, int, int))CREATERESOURCEFILE_ADDR;
void(*ServiceResourceLoading)() = (void(*)())SERVICERESOURCELOADING_ADDR;

// Underground 1 special - FUNCTION WRAPPING
// This is done due to various crazy compiler settings used by EA at the time...

#pragma runtime_checks( "", off )
uint32_t ResourceFile_BeginLoading_Func_Addr = RESFILE_BEGINLOADING_ADDR;
void __stdcall ResourceFile_BeginLoading(void* ResourceFile, void* unk1, void* unk2)
{
	_asm
	{
		mov edx, ResourceFile
		mov ecx, unk2
		mov eax, unk1
		call ResourceFile_BeginLoading_Func_Addr
	}
}

uint32_t FEngFont_RenderString_Func_Addr = FENGFONT_RENDERSTRING_ADDR;
// FEngFont::RenderString - arg1 = this, ecx = Color, eax = FEString, arg2 = pcString, arg3 = matrix, arg4 = cached
void __stdcall FEngFont_RenderString(unsigned int dis, void* Color, wchar_t* pcString, void* obj, void* matrix, void* cached)
{
	_asm
	{
		push cached
		push matrix
		mov eax, obj
		push pcString
		mov ecx, Color
		push dis
		call FEngFont_RenderString_Func_Addr
	}
}


uint32_t FEngFont_GetLineWidth_Func_Addr = FENGFONT_GETLINEWIDTH_ADDR;
uint32_t GetLineWidth_EBX2 = 0;
uint32_t GetLineWidth_EBP2 = 0;
// FEngFont::GetLineWidth - arg1 = this, arg2 = str, arg3 = wordwrap, ecx = flags
float __stdcall FEngFont_GetLineWidth(unsigned int dis, wchar_t* pcString, unsigned int flags, bool* word_wrap)
{
	float result;
	_asm
	{
		mov GetLineWidth_EBX2, ebx
		mov GetLineWidth_EBP2, ebp
		push word_wrap
		mov ecx, flags
		push pcString
		push dis
		call FEngFont_GetLineWidth_Func_Addr
		fstp dword ptr [result]
		mov ebx, GetLineWidth_EBX2
		mov ebp, GetLineWidth_EBP2
	}
	return result;
}


uint32_t FEngFont_GetTextHeight_Func_Addr = FENGFONT_GETTEXTHEIGHT_ADDR;
uint32_t GetTextHeight_EBX2 = 0;
uint32_t GetTextHeight_ECX2 = 0;
uint32_t GetTextHeight_ESI2 = 0;
uint32_t GetTextHeight_EDI2 = 0;
uint32_t GetTextHeight_EBP2 = 0;
float __stdcall FEngFont_GetTextHeight(unsigned int dis, wchar_t* pcString, int flags)
{
	_asm
	{
		mov GetTextHeight_EBX2, ebx
		mov GetTextHeight_ECX2, ecx
		mov GetTextHeight_ESI2, esi
		mov GetTextHeight_EDI2, edi
		mov GetTextHeight_EBP2, ebp
	}

	float retval = FEngFont_GetTextHeight_Func(dis, pcString, flags);

	_asm
	{
		mov ebx, GetTextHeight_EBX2
		mov ecx, GetTextHeight_ECX2
		mov esi, GetTextHeight_ESI2
		mov edi, GetTextHeight_EDI2
		mov ebp, GetTextHeight_EBP2
	}

	return retval;
}

uint32_t FindFont_Func_Addr = FINDFONT_ADDR;
// FindFont - eax = hash
void* __stdcall FindFont(unsigned int hash)
{
	void* result;
	_asm
	{
		mov eax, hash
		call FindFont_Func_Addr
		mov result, eax
	}

	return result;
}