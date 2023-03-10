#pragma once
#include "..\includes\injector\injector.hpp"
#include "..\includes\injector\utility.hpp"
#include <d3d9.h>

#ifdef GAME_MW
#include "MW_Address.h"
#endif

#ifdef GAME_CARBON
#include "Carbon_Address.h"
#endif

#ifdef GAME_PS
#include "ProStreet_Address.h"
#endif

#ifdef GAME_UC
#include "Undercover_Address.h"
#endif

#ifdef GAME_UG2
#include "UG2_Address.h"
#endif

#ifdef GAME_UG
#include "UG_Address.h"
#endif

#define CUSTOM_FONT_FOLDER "CustomFonts"
#define FONT_SCALE_SETTINGS_FILENAME "NFS_CustomFont.ini"
#define MAX_FONT_DEFINES 64
#define MAX_FONT_SCALES 3

float FontScalars[MAX_FONT_SCALES][MAX_FONT_DEFINES];
uint32_t FontHashes[MAX_FONT_SCALES][MAX_FONT_DEFINES];
static uint32_t FontDefineCount[MAX_FONT_SCALES];
static uint32_t FontScaleMode = 0;
static const char* PathStrs[3] = {"Low", "Mid", "High"};

// file loader list
char** FileDirectoryListing;
uint32_t FileCount = 0;
char LoaderFileName[512];

#ifndef GAME_UC
void __stdcall LoadResourceFile(char* filename, int ResType, int unk1, void* unk2, void* unk3, int unk4, int unk5);
#endif

bool bFileExists(char* Filename)
{
	FILE* fin = fopen(Filename, "rb");
	if (!fin)
		return false;
	fclose(fin);
	return true;
}

void toupper_char_string(char* in)
{
	char* s = in;
	while (*s)
	{
		*s = toupper((unsigned char)*s);
		s++;
	}
}

int bStringHash(const char* str)
{
	if (str == NULL)
		return 0;

	char* _str = ((char*)str);
	int result = -1;

	while (*_str != 0)
	{
		result = result * 0x21 + (unsigned int)(toupper(*_str));
		_str = _str + 1;
	}

	return result;
}
