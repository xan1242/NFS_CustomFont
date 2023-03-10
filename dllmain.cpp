// NFS Custom Font loader & scaler
// MW: Broken word wrapping in certain scenarios... Not the fault of scaling (confirmed via FontWiz)
// Carbon: FMV subtitlers broken (tutorial_canyon) - fault occurs if the font texture page is large
// Pro Street: working, maybe same FMV bug as Carbon, needs testing
// Undercover: works, only on HUD which is using FEng. Apt is used for the rest of UI
// Underground & Underground 2: Fully working, FMV subtitles need checking

#include "stdafx.h"
#include "stdio.h"
#include <stdlib.h>
#include <strsafe.h>
#include <math.h>
#include "NFS_CustomFont.h"
#include "mini/src/mini/ini.h"

#pragma runtime_checks( "", off )

void FixWorkingDirectory()
{
	char ExecutablePath[_MAX_PATH];

	GetModuleFileName(GetModuleHandle(""), ExecutablePath, _MAX_PATH);
	ExecutablePath[strrchr(ExecutablePath, '\\') - ExecutablePath] = 0;
	SetCurrentDirectory(ExecutablePath);
}

#ifndef GAME_UC
void __stdcall LoadResourceFile(char* filename, int ResType, int unk1, void* unk2, void* unk3, int unk4, int unk5)
{
	ResourceFile_BeginLoading(CreateResourceFile(filename, ResType, unk1, unk4, unk5), unk2, unk3);
}
#endif

DWORD GetDirectoryListing(const char* FolderPath)
{
	WIN32_FIND_DATA ffd = { 0 };
	TCHAR  szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	unsigned int NameCounter = 0;

	StringCchCopy(szDir, MAX_PATH, FolderPath);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	if (strlen(FolderPath) > (MAX_PATH - 3))
	{
		//_tprintf(TEXT("Directory path is too long.\n"));
		return -1;
	}

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		//printf("FindFirstFile error\n");
		return dwError;
	}

	// count the files up first
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			FileCount++;
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		//printf("FindFirstFile error\n");
	}
	FindClose(hFind);

	// then create a file list in an array, redo the code
	FileDirectoryListing = (char**)calloc(FileCount, sizeof(char*));

	ffd = { 0 };
	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return dwError;
	}

	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			FileDirectoryListing[NameCounter] = (char*)calloc(strlen(ffd.cFileName) + 1, sizeof(char));
			strcpy(FileDirectoryListing[NameCounter], ffd.cFileName);
			NameCounter++;
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		//printf("FindFirstFile error\n");
	}

	FindClose(hFind);
	return dwError;
}

static injector::hook_back<void(*)()> hb_LoadGlobalAChunks;
void LoadGlobalAChunks_Hook()
{
	// get the appropriate mode per resolution
	LPDIRECT3DDEVICE9 gDevice = GAME_D3D9_DEVICE;
	D3DVIEWPORT9 v = { 0 };

	gDevice->GetViewport(&v);

	if (v.Height > 720)
		FontScaleMode = 1;

	if (v.Height > 1080)
		FontScaleMode = 2;

	sprintf(LoaderFileName, "%s\\%s", CUSTOM_FONT_FOLDER, PathStrs[FontScaleMode]);
	GetDirectoryListing(LoaderFileName);

	for (int i = 0; i < FileCount; i++)
	{
		sprintf(LoaderFileName, "%s\\%s\\%s", CUSTOM_FONT_FOLDER, PathStrs[FontScaleMode], FileDirectoryListing[i]);
#ifndef GAME_UC
		if (bFileExists(LoaderFileName))
			LoadResourceFile(LoaderFileName, 1, 0, NULL, 0, 0, 0);
#else
		if (bFileExists(LoaderFileName))
			LoadResourceFile(LoaderFileName, 1, 0, *(int*)FECATEGORY_ADDR, NULL, 0, 0, 0);
#endif
	}
	ServiceResourceLoading();
	return hb_LoadGlobalAChunks.fun();
}

char fntcheck[32];
char fntcheck2[32];
uint32_t (__cdecl* LoaderFEngFont)(void* bChunk) = (uint32_t(__cdecl*)(void*))LOADERFENGFONT_ADDR;
uint32_t __cdecl LoaderFEngFont_Hook(void* bChunk)
{
	if (*(uint32_t*)bChunk != 0x030201)
		return 0;

	char* fontname = (char*)((uint32_t)bChunk + 8);
	char* extpoint = NULL;

	strcpy(fntcheck2, fontname);
	toupper_char_string(fntcheck2);

	// check if we are overriding the font via an external file and if we are, stop it from loading
	for (int i = 0; i < FileCount; i++)
	{
		// get the filename from the directory list and truncate it
		strcpy(fntcheck, FileDirectoryListing[i]);
		extpoint = strrchr(fntcheck, '.');
		if (extpoint)
			*extpoint = '\0';
		toupper_char_string(fntcheck);
		// compare the filename to the font name
		if (strcmp(fntcheck, fntcheck2) == 0)
		{
			// if there is a match, check if the file in memory that we're trying to load is actually a file from the stock game
			// we check this by checking if there's data beyond the termination point of the font name
			if (*(fontname + strlen(fontname) + 2))
			{
				// if it's a stock file -- return 1 to indicate a successful chunk load and to let the game know that it should move on from it
				return 1;
			}
		}
	}

	return LoaderFEngFont(bChunk);
}

uint32_t(__cdecl* UnloaderFEngFont)(void* bChunk) = (uint32_t(__cdecl*)(void*))UNLOADERFENGFONT_ADDR;
uint32_t __cdecl UnloaderFEngFont_Hook(void* bChunk)
{
	if (*(uint32_t*)bChunk != 0x030201)
		return 0;

	char* fontname = (char*)((uint32_t)bChunk + 8);
	char* extpoint = NULL;

	strcpy(fntcheck2, fontname);
	toupper_char_string(fntcheck2);

	// check if we are overriding the font via an external file and if we are, stop it from unloading
	for (int i = 0; i < FileCount; i++)
	{
		// get the filename from the directory list and truncate it
		strcpy(fntcheck, FileDirectoryListing[i]);
		extpoint = strrchr(fntcheck, '.');
		if (extpoint)
			*extpoint = '\0';
		toupper_char_string(fntcheck);
		// compare the filename to the font name
		if (strcmp(fntcheck, fntcheck2) == 0)
		{
			// if there is a match, check if the file in memory that we're trying to unload is actually a file from the stock game
			// we check this by checking if there's data beyond the termination point of the font name
			if (*(fontname + strlen(fontname) + 2))
			{
				// if it's a stock file -- return 1 to indicate a successful chunk unload and to let the game know that it should move on from it
				return 1;
			}
		}
	}

	return UnloaderFEngFont(bChunk);
}

uint32_t FEngFont_GetFontHash(void* FEngFontObj)
{
	return *(uint32_t*)((int)FEngFontObj + FENGFONT_HASH_OFFSET);
}

float FEngFont_GetFontHeight(void* FEngFontObj)
{
	return *(float*)((int)FEngFontObj + FENGFONT_HEIGHT_OFFSET);
}

float GetFontScalarByHash(uint32_t FontHash)
{
	uint32_t m = FontScaleMode;
	for (int i = 0; i < FontDefineCount[m]; i++)
	{
		if (FontHashes[m][i] == FontHash)
			return FontScalars[m][i];
	}
	return 1.0f;
}

#ifdef GAME_UG
// preserve the registers
uint32_t GetLineWidth_EBX = 0;
uint32_t GetLineWidth_EBP = 0;
uint32_t GetLineWidth_Flags = 0;
float __stdcall FEngFont_GetLineWidth_Hook(uint32_t thethis, wchar_t* pcString, bool* word_wrap)
{
	_asm
	{
		mov GetLineWidth_EBX, ebx
		mov GetLineWidth_EBP, ebp
		mov GetLineWidth_Flags, ecx
	}

	float scalar = GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis));
	float retval = FEngFont_GetLineWidth(thethis, pcString, GetLineWidth_Flags, word_wrap);

	_asm
	{
		mov ebx, GetLineWidth_EBX
		mov ebp, GetLineWidth_EBP
	}

	return retval * scalar;

}
#else
float __stdcall FEngFont_GetCharacterWidth_Hook(wchar_t chr, wchar_t prevchr, unsigned int flags)
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	float retval = FEngFont_GetCharacterWidth(thethis, chr, prevchr, flags);

	return retval * GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis));
}

#if (defined GAME_UG2)
float __stdcall FEngFont_GetLineWidth_Hook(wchar_t* pcString, unsigned int flags, unsigned int maxWidth, bool* word_wrap)
#else
float __stdcall FEngFont_GetLineWidth_Hook(wchar_t* pcString, unsigned int flags, unsigned int maxWidth, bool word_wrap)
#endif
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx
	
	float retval = FEngFont_GetLineWidth(thethis, pcString, flags, maxWidth, word_wrap);
	
	return retval * GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis));
}
#endif // GAME_UG

#if (defined GAME_UG)
// preserve the registers
uint32_t GetTextHeight_EBX = 0;
uint32_t GetTextHeight_ECX = 0;
uint32_t GetTextHeight_ESI = 0;
uint32_t GetTextHeight_EDI = 0;
uint32_t GetTextHeight_EBP = 0;
float __stdcall FEngFont_GetTextHeight_Hook(unsigned int dis, wchar_t* pcString, int flags)
{
	_asm
	{
		mov GetTextHeight_EBX, ebx
		mov GetTextHeight_ECX, ecx
		mov GetTextHeight_ESI, esi
		mov GetTextHeight_EDI, edi
		mov GetTextHeight_EBP, ebp
	}

	float scalar = GetFontScalarByHash(FEngFont_GetFontHash((void*)dis));
	float retval = FEngFont_GetTextHeight(dis, pcString, flags);

	_asm
	{
		mov ebx, GetTextHeight_EBX
		mov ecx, GetTextHeight_ECX
		mov esi, GetTextHeight_ESI
		mov edi, GetTextHeight_EDI
		mov ebp, GetTextHeight_EBP
	}

	return retval * scalar;
}
#elif (defined GAME_UG2)
float __stdcall FEngFont_GetTextHeight_Hook(wchar_t* pcString, int flags)
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	float scalar = GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis));
	float retval = FEngFont_GetTextHeight(thethis, pcString, flags);


	return retval * scalar;
}
#else
float __stdcall FEngFont_GetTextHeight_Hook(wchar_t* pcString, int ilLeading, unsigned int flags, unsigned int maxWidth, bool word_wrap)
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx
	
	float scalar = GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis));
	unsigned int newMaxWidth = (unsigned int)((float)maxWidth / scalar * (1 - scalar));
	//unsigned int newMaxWidth = maxWidth;
	float retval = FEngFont_GetTextHeight(thethis, pcString, ilLeading, flags, newMaxWidth, word_wrap);
	
	
	return retval * scalar;
}
#endif

#if !(defined GAME_MW) && !(defined GAME_UG2) && !(defined GAME_UG)
#ifdef GAME_PS
float __stdcall FEngFont_GetTextHeight_Hook_2(wchar_t* pcString, int ilLeading, unsigned int flags, unsigned int maxWidth, bool word_wrap)
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	float scalar = GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis));
	unsigned int newMaxWidth = (unsigned int)((float)maxWidth / scalar * (1 - scalar));

	float retval = FEngFont_GetTextHeight(thethis, pcString, ilLeading, flags, newMaxWidth, word_wrap);
	float maxheight = FEngFont_GetFontHeight((void*)thethis) * 8.978723404255319f;

	if ((retval) > maxheight)
		return maxheight;

	return retval;
}
#else
float __stdcall FEngFont_GetTextHeight_Hook_2(wchar_t* pcString, int ilLeading, unsigned int flags, unsigned int maxWidth, bool word_wrap)
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	float scalar = GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis));
	unsigned int newMaxWidth = (unsigned int)((float)maxWidth / scalar * (1 - scalar));

	float retval = FEngFont_GetTextHeight(thethis, pcString, ilLeading, flags, newMaxWidth, word_wrap);


	return retval;
}
#endif // GAME_PS
#endif

static float AddPolyScalar = 1.0f;
char feng_str_copy[0x78];

#if (defined GAME_UG)
void* FindFont_Hook_2()
{
	uint32_t hash;
	_asm mov hash, eax
	void* font = FindFont(hash);

	*(float*)FONT_SCALE_VAL_ADDR_1 = GetFontScalarByHash(hash);
	*(float*)FONT_SCALE_VAL_ADDR_2 = GetFontScalarByHash(hash) * 1.6;

	return font;
}

void __stdcall FEngFont_RenderString_Hook(uint32_t thethis, wchar_t* pcString, void* matrix, void* cached)
{
	void* obj;
	_asm mov obj, eax
	void* Color;
	_asm mov Color, ecx

	float scalar = GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis));

	memcpy(&feng_str_copy, obj, 0x78);

	uint32_t* maxWidth = (uint32_t*)&(feng_str_copy[0x74]);
	*maxWidth = (unsigned int)round((float)*maxWidth / GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis)));


	return FEngFont_RenderString(thethis, Color, pcString, feng_str_copy, matrix, cached);
}

#elif (defined GAME_UG2)
void* FindFont_Hook_2(unsigned int hash)
{
	void* font = FindFont(hash);

	*(float*)FONT_SCALE_VAL_ADDR_1 = GetFontScalarByHash(hash);
	*(float*)FONT_SCALE_VAL_ADDR_2 = GetFontScalarByHash(hash) * 1.6;

	return font;
}

void __stdcall FEngFont_RenderString_Hook(void* Color, wchar_t* pcString, void* obj, void* matrix, void* cached, void* PackageRenderInfo)
{
	uint32_t thethis;
	_asm mov thethis, ecx
	float scalar = GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis));

	memcpy(&feng_str_copy, obj, 0x78);

	uint32_t* maxWidth = (uint32_t*)&(feng_str_copy[0x74]);
	*maxWidth = (unsigned int)round((float)*maxWidth / GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis)));


	return FEngFont_RenderString(thethis, Color, pcString, feng_str_copy, matrix, cached, PackageRenderInfo);
}
#elif (defined GAME_MW)
void* __stdcall FERenderObject_AddPoly_Hook(float x0, float y0, float x1, float y1, float z, float s0, float t0, float s1, float t1, unsigned int* colors, int res)
{
	unsigned int TheThis = 0;
	_asm mov TheThis, ecx

	x0 *= AddPolyScalar;
	y0 *= AddPolyScalar;
	x1 *= AddPolyScalar;
	y1 *= AddPolyScalar;

	return FERenderObject_AddPoly(TheThis, x0, y0, x1, y1, z, s0, t0, s1, t1, colors, res);
}

void __stdcall FEngFont_RenderString_Hook(void* Color, wchar_t* pcString, void* obj, void* matrix, void* cached)
{
	uint32_t thethis;
	_asm mov thethis, ecx
	float scalar = GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis));

	memcpy(&feng_str_copy, obj, 0x78);

	uint32_t* maxWidth = (uint32_t*)&(feng_str_copy[0x74]);
	*maxWidth = (unsigned int)round((float)*maxWidth / GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis)));

	return FEngFont_RenderString(thethis, Color, pcString, feng_str_copy, matrix, cached);
}
#else
void* __stdcall FERenderObject_AddPoly_Hook(float x0, float y0, float x1, float y1, float z, float s0, float t0, float s1, float t1, unsigned int* colors)
{
	unsigned int TheThis = 0;
	_asm mov TheThis, ecx

	x0 *= AddPolyScalar;
	y0 *= AddPolyScalar;
	x1 *= AddPolyScalar;
	y1 *= AddPolyScalar;

	return FERenderObject_AddPoly(TheThis, x0, y0, x1, y1, z, s0, t0, s1, t1, colors);
}

void __stdcall FEngFont_RenderString_Hook(void* Color, wchar_t* pcString, void* obj, void* matrix, void* cached)
{
	uint32_t thethis;
	_asm mov thethis, ecx

	float scalar = GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis));

	memcpy(&feng_str_copy, obj, 0x78);

	uint32_t* maxWidth = (uint32_t*)&(feng_str_copy[0x74]);
	*maxWidth = (unsigned int)((float)*maxWidth / GetFontScalarByHash(FEngFont_GetFontHash((void*)thethis)));

	return FEngFont_RenderString(thethis, Color, pcString, feng_str_copy, matrix, cached);
}
#endif

#if (defined GAME_CARBON) || (defined GAME_UC)
void __stdcall FEngFont_PrintCharacter_Hook(void* pGlyph, float fX, float fY, unsigned int* render_colors, void* cached)
{
	unsigned int TheThis = 0;
	_asm mov TheThis, ecx

	// catch the font scalar for AddPoly right before it's called
	AddPolyScalar = GetFontScalarByHash(FEngFont_GetFontHash((void*)TheThis));

	return FEngFont_PrintCharacter(TheThis, pGlyph, fX, fY, render_colors, cached);
}
#elif !(defined GAME_UG2) && !(defined GAME_UG)
void __stdcall FEngFont_PrintCharacter_Hook(void* pGlyph, float fX, float fY, unsigned int* render_colors, void* cached, void* pPoly)
{
	unsigned int TheThis = 0;
	_asm mov TheThis, ecx

	// catch the font scalar for AddPoly right before it's called
	AddPolyScalar = GetFontScalarByHash(FEngFont_GetFontHash((void*)TheThis));

	return FEngFont_PrintCharacter(TheThis, pGlyph, fX, fY, render_colors, cached, pPoly);
}
#endif

#if (defined GAME_PS) || (defined GAME_UC)
void __stdcall FEngFont_PrintCharacterInWorld_Hook(void* pGlyph, float fX, float fY, unsigned int* render_colors, void* cached, ePoly* pPoly)
{
	void* FEngFontObj = NULL;
	_asm mov FEngFontObj, ecx

	float scalar = GetFontScalarByHash(FEngFont_GetFontHash((void*)FEngFontObj));

	if (pPoly)
	{
		FEngFont_PrintCharacterInWorld((unsigned int)FEngFontObj, pGlyph, fX, fY, render_colors, cached, pPoly);

		(*pPoly).Vertices[0].x *= scalar;
		(*pPoly).Vertices[0].y *= scalar;
		(*pPoly).Vertices[0].z *= scalar;

		(*pPoly).Vertices[1].x *= scalar;
		(*pPoly).Vertices[1].y *= scalar;
		(*pPoly).Vertices[1].z *= scalar;

		(*pPoly).Vertices[2].x *= scalar;
		(*pPoly).Vertices[2].y *= scalar;
		(*pPoly).Vertices[2].z *= scalar;

		(*pPoly).Vertices[3].x *= scalar;
		(*pPoly).Vertices[3].y *= scalar;
		(*pPoly).Vertices[3].z *= scalar;
	}
	else if (cached)
	{
#ifdef GAME_PS
		_asm mov ecx, FEngFontObj
		FEngFont_PrintCharacter_Hook(pGlyph, fX, fY, render_colors, cached, pPoly);
#else
		_asm mov ecx, FEngFontObj
		FEngFont_PrintCharacter_Hook(pGlyph, fX, fY, render_colors, cached);
#endif
	}
}
#endif

char feng_font_copy[0x38];
void* FindFont_Hook(unsigned int hash)
{
	void* font = FindFont(hash);
	memcpy(feng_font_copy, font, 0x38);
	float* height = (float*)&(feng_font_copy[FENGFONT_HEIGHT_OFFSET]);
	*height *= GetFontScalarByHash(hash);

	return feng_font_copy;
}

#ifdef GAME_PS
void* FindFont_Hook_2(unsigned int hash)
{
	void* font = FindFont(hash);
	memcpy(feng_font_copy, font, 0x38);
	float* height = (float*)&(feng_font_copy[FENGFONT_HEIGHT_OFFSET]);
	*height *= GetFontScalarByHash(hash);

	return feng_font_copy;
}
#endif

void InitConfig()
{
	mINI::INIFile inifile("NFS_CustomFont.ini");
	mINI::INIStructure ini;
	inifile.read(ini);

	if (ini.has("FontScaleLow"))
	{
		int dc = 0;
		auto const& inisection = ini["FontScaleLow"];
		for (auto const& it : inisection)
		{
			FontHashes[0][dc] = bStringHash(it.first.c_str());
			FontScalars[0][dc] = stof(it.second);
			dc++;
		}
		FontDefineCount[0] = dc;
	}
	if (ini.has("FontScaleMid"))
	{
		int dc = 0;
		auto const& inisection = ini["FontScaleMid"];
		for (auto const& it : inisection)
		{
			FontHashes[1][dc] = bStringHash(it.first.c_str());
			FontScalars[1][dc] = stof(it.second);
			dc++;
		}
		FontDefineCount[1] = dc;
	}
	if (ini.has("FontScaleHigh"))
	{
		int dc = 0;
		auto const& inisection = ini["FontScaleHigh"];
		for (auto const& it : inisection)
		{
			FontHashes[2][dc] = bStringHash(it.first.c_str());
			FontScalars[2][dc] = stof(it.second);
			dc++;
		}
		FontDefineCount[2] = dc;
	}
}

int Init()
{
	injector::MakeCALL(RENDERSTR_HOOK_ADDR, FEngFont_RenderString_Hook, true);

	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_3, FEngFont_GetLineWidth_Hook, true); // GetTextWidth
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_4, FEngFont_GetLineWidth_Hook, true); // GetTextWidth	

	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_1, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_2, FEngFont_GetTextHeight_Hook, true);

#if !(defined GAME_UG2) && !(defined GAME_UG)
	injector::MakeCALL(ADDPOLY_HOOK_ADDR, FERenderObject_AddPoly_Hook, true);
	injector::MakeCALL(PRINTCHAR_HOOK_ADDR, FEngFont_PrintCharacter_Hook, true);
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_RENDERSTR, FEngFont_GetLineWidth_Hook, true); // cFEngRender::RenderString
#endif

#ifdef GAME_UG
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_3, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_4, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_5, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_6, FEngFont_GetTextHeight_Hook, true);

	injector::MakeCALL(FINDFONT_HOOK_ADDR_1, FindFont_Hook_2, true);

	DWORD oldprotect;
	injector::UnprotectMemory(FONT_SCALE_VAL_ADDR_1, sizeof(uint32_t), oldprotect);
	injector::UnprotectMemory(FONT_SCALE_VAL_ADDR_2, sizeof(uint32_t), oldprotect);
#endif
#ifdef GAME_UG2
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_1, FEngFont_GetLineWidth_Hook, true);
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_2, FEngFont_GetLineWidth_Hook, true);
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_5, FEngFont_GetLineWidth_Hook, true);
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_6, FEngFont_GetLineWidth_Hook, true);


	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_3, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_4, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_5, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_6, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_7, FEngFont_GetTextHeight_Hook, true);

	injector::MakeCALL(GETCHARACTERWIDTH_HOOK_ADDR_1, FEngFont_GetCharacterWidth_Hook, true);
	injector::MakeCALL(GETCHARACTERWIDTH_HOOK_ADDR_2, FEngFont_GetCharacterWidth_Hook, true);

	injector::MakeCALL(FINDFONT_HOOK_ADDR_1, FindFont_Hook, true);
	injector::MakeCALL(FINDFONT_HOOK_ADDR_2, FindFont_Hook_2, true);
	DWORD oldprotect;
	injector::UnprotectMemory(FONT_SCALE_VAL_ADDR_1, sizeof(uint32_t), oldprotect);
	injector::UnprotectMemory(FONT_SCALE_VAL_ADDR_2, sizeof(uint32_t), oldprotect);
#endif

#ifdef GAME_MW
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_1, FEngFont_GetLineWidth_Hook, true);
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_2, FEngFont_GetLineWidth_Hook, true);
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_7, FEngFont_GetLineWidth_Hook, true);
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_5, FEngFont_GetLineWidth_Hook, true);
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_6, FEngFont_GetLineWidth_Hook, true);

	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_3, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_4, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_5, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_6, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(FINDFONT_HOOK_ADDR_1, FindFont_Hook, true); // feDialogConfig
	injector::MakeCALL(FINDFONT_HOOK_ADDR_2, FindFont_Hook, true); // SMS message
	injector::MakeCALL(GETCHARACTERWIDTH_HOOK_ADDR_1, FEngFont_GetCharacterWidth_Hook, true);
	injector::MakeCALL(GETCHARACTERWIDTH_HOOK_ADDR_2, FEngFont_GetCharacterWidth_Hook, true); // CTextScroller::WordWrapAddLines
#endif
#ifdef GAME_CARBON
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_5, FEngFont_GetLineWidth_Hook, true);
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_6, FEngFont_GetLineWidth_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_3, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_4, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_5, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_6, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_7, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_8, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_9, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_10, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_11, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_12, FEngFont_GetTextHeight_Hook_2, true);
	injector::MakeCALL(GETCHARACTERWIDTH_HOOK_ADDR_1, FEngFont_GetCharacterWidth_Hook, true);
#endif
#ifdef GAME_PS
	injector::MakeCALL(PRINTCHARINWORLD_HOOK_ADDR, FEngFont_PrintCharacterInWorld_Hook, true);
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_5, FEngFont_GetLineWidth_Hook, true);
	injector::MakeCALL(GETLINEWIDTH_HOOK_ADDR_6, FEngFont_GetLineWidth_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_4, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_5, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_6, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_7, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_8, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_9, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_10, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_12, FEngFont_GetTextHeight_Hook_2, true);
	injector::MakeCALL(GETCHARACTERWIDTH_HOOK_ADDR_1, FEngFont_GetCharacterWidth_Hook, true);
	//injector::MakeCALL(0x005FEDFD, FindFont_Hook_2, true); // feDialogConfig
	//injector::MakeCALL(0x005FEC4E, FindFont_Hook_2, true); // feDialogConfig
#endif
#ifdef GAME_UC
	injector::MakeCALL(PRINTCHARINWORLD_HOOK_ADDR, FEngFont_PrintCharacterInWorld_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_4, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_5, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_6, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_7, FEngFont_GetTextHeight_Hook, true);
	injector::MakeCALL(GETTEXTHEIGHT_HOOK_ADDR_8, FEngFont_GetTextHeight_Hook, true);
#endif

	hb_LoadGlobalAChunks.fun = injector::MakeCALL(LOADGLOBALACHUNKS_CALL_ADDR, LoadGlobalAChunks_Hook, true).get();

	LoaderFEngFont = (uint32_t(__cdecl*)(void*))*(uint32_t*)LOADERFENGFONT_VTABLE_ADDR;
	*(uint32_t*)LOADERFENGFONT_VTABLE_ADDR = (uint32_t)&LoaderFEngFont_Hook;

	UnloaderFEngFont = (uint32_t(__cdecl*)(void*)) * (uint32_t*)UNLOADERFENGFONT_VTABLE_ADDR;
	*(uint32_t*)UNLOADERFENGFONT_VTABLE_ADDR = (uint32_t)&UnloaderFEngFont_Hook;

	return 0;
}


BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		//freopen("CON", "w", stdout);
		//freopen("CON", "w", stderr);

		InitConfig();
		FixWorkingDirectory();
		Init();
	}
	return TRUE;
}


