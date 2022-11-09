// NFSRealFont - EA RealFont Shape Ripper & FEngFont Generator
// by Xan/Tenjoin

#include "stdafx.h"
#include "FSHhead.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct RealFontOld_Font
{
	char mSignature[4];
	unsigned int mSize;
	unsigned short mVersion;
	unsigned short mNum;
	int mFlags;
	char mCenterX;
	char mCenterY;
	unsigned char mAscent;
	unsigned char mDescent;
	int mGlyphTbl;
	int mKernTbl;
	int mShape;
	int mStates[24];
}CurrentFont;

unsigned char FSH_header[] = FSH_HEADER;
unsigned char FSH_trailer[] = FSH_TRAILER;
struct stat st = { 0 };
char* OutputFileName;
char* OutputFileName2;

int ParseFontInfo(const char* InFilename)
{
	FILE* fin = fopen(InFilename, "rb");

	if (fin == NULL)
	{
		printf("ERROR: Can't open file for reading: %s\n", InFilename);
		perror("ERROR");
		return -1;
	}

	fread(&CurrentFont, sizeof(RealFontOld_Font), 1, fin);

	fclose(fin);

	return 0;
}

int RipShape(const char* InFilename, const char* OutFilename, const char* OutFilename2)
{
	FILE* fin = fopen(InFilename, "rb");
	if (fin == NULL)
	{
		printf("ERROR: Can't open file for reading: %s\n", InFilename);
		perror("ERROR");
		return -1;
	}

	FILE* fout = fopen(OutFilename, "wb");
	if (fout == NULL)
	{
		printf("ERROR: Can't open file for writing: %s\n", OutFilename);
		perror("ERROR");
		return -1;
	}

	FILE* fout2 = fopen(OutFilename2, "wb");
	if (fout2 == NULL)
	{
		printf("ERROR: Can't open file for writing: %s\n", OutFilename2);
		perror("ERROR");
		return -1;
	}

	void* FileBuffer = NULL;
	stat(InFilename, &st);

	FileBuffer = malloc(st.st_size);

	fread(FileBuffer, CurrentFont.mShape, 1, fin);
	fwrite(FileBuffer, CurrentFont.mShape, 1, fout2);
	fclose(fout2);

	fread(FileBuffer, st.st_size - CurrentFont.mShape, 1, fin);

	fwrite(FSH_header, sizeof(FSH_header), 1, fout);
	fwrite(FileBuffer, st.st_size - CurrentFont.mShape, 1, fout);
	fwrite(FSH_trailer, sizeof(FSH_trailer), 1, fout);

	
	fclose(fout);
	fclose(fin);

	return 0;
}

// FENgFont chunk structure is as follows:
// 0x030201 = chunk ID
// @0x4 4 bytes size
// @0x8 font name (48 chars?)
// @0x108 - texture name (also 48 chars?)
// @0x208 - RealFont glyph/kern map
// The format is so simple, I will refrain from building data structures and generate everything on the fly...

int GenerateFEngFontChunk(const char* InFilename, char* FEngName, const char* OutFilename)
{
	struct stat st = {0};
	unsigned int ChunkSize = 0;

	if (stat(InFilename, &st))
	{
		printf("ERROR: Can't find %s during size calculation!\n", InFilename);
		return -1;
	}

	ChunkSize = st.st_size + 0x200;
	void* FileOutBuffer = calloc(ChunkSize + 8, sizeof(char));
	char* namestr = (char*)((int)FileOutBuffer + 8);
	char* namestr2 = (char*)((int)FileOutBuffer + 0x108);

	*(int*)FileOutBuffer = 0x030201;
	*(int*)((int)FileOutBuffer + 4) = ChunkSize;
	sprintf(namestr, "%s", FEngName);
	sprintf(namestr2, "%s", FEngName);


	FILE* fin = fopen(InFilename, "rb");
	if (fin == NULL)
	{
		printf("ERROR: Can't open file for reading: %s\n", InFilename);
		perror("ERROR");
		return -1;
	}

	fread((void*)((int)FileOutBuffer + 0x208), st.st_size, 1, fin);
	fclose(fin);

	FILE* fout = fopen(OutFilename, "wb");
	if (fout == NULL)
	{
		printf("ERROR: Can't open file for writing: %s\n", OutFilename);
		perror("ERROR");
		return -1;
	}

	fwrite(FileOutBuffer, ChunkSize + 8, 1, fout);
	fclose(fout);
	
	return 0;
}


int main(int argc, char *argv[])
{
	printf("EA RealFont Shape Ripper\n");

	if (argc < 2)
	{
		printf("USAGE: %s InFont [OutShape.fsh CutFont.ffn]\nUSAGE (FEng Font Generate Mode): %s -g CutFontName FEngFontName [OutChunkName]\n", argv[0], argv[0]);
		return -1;
	}

	if (argv[1][0] == '-' && argv[1][1] == 'g') // FEng Font Generator mode
	{
		if (argc == 4)
		{
			OutputFileName = (char*)calloc(strlen(argv[3]) + 8, sizeof(char));
			strcpy(OutputFileName, argv[3]);
			strcpy(&OutputFileName[strlen(OutputFileName)], ".bin");
		}
		else
		{
			OutputFileName = argv[4];
		}
		printf("Generating FEngFont %s from %s to %s\n", argv[3], argv[2], OutputFileName);
		GenerateFEngFontChunk(argv[2], argv[3], OutputFileName);
		return 0;
	}

	if (argc == 2)
	{
		char* PatchPoint;
		OutputFileName = (char*)calloc(strlen(argv[1]) + 8, sizeof(char));
		OutputFileName2 = (char*)calloc(strlen(argv[1]) + 8, sizeof(char));
		strcpy(OutputFileName, argv[1]);
		strcpy(OutputFileName2, argv[1]);
		PatchPoint = strrchr(OutputFileName, '.');
		strcpy(PatchPoint, ".fsh");
		PatchPoint = strrchr(OutputFileName2, '.');
		strcpy(PatchPoint, "_cut.ffn");
	}
	else
	{
		OutputFileName = argv[2];
		OutputFileName2 = argv[3];
	}

	printf("Parsing RealFont...\n");
	ParseFontInfo(argv[1]);
	printf("Exporting shape from 0x%x to %s\n", CurrentFont.mShape, OutputFileName);
	RipShape(argv[1], OutputFileName, OutputFileName2);

    return 0;
}

