// NFS String Hash calculator console app
// by Xan/Tenjoin

#include "stdafx.h"
#include <ctype.h>
#include <string.h>

int bStringHash(char *a1)
{
	char *v1; // edx@1
	char v2; // cl@1
	int result; // eax@1

	v1 = a1;
	v2 = *a1;
	for (result = -1; v2; ++v1)
	{
		result = v2 + 33 * result;
		v2 = v1[1];
	}
	return result;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("ERROR: Too few arguments.\nUSAGE: %s [arguments] string (use quotation marks if using spaces)\nARGUMENTS:\n-s = output to stdout\n-n = generate a newline\n-ls = lowercase hashing, output to stdout\n-ln = lowercase hashing, generate a newline\n", argv[0]);
		return -1;
	}

	if ((argv[1][0] == '-') && (argv[1][1] == 'l'))
	{
		if ((argv[1][2] == 's'))
			return bStringHash(argv[2]);
		if ((argv[1][2] == 'n'))
			printf("%.8X\n", bStringHash(argv[2]));
		else
			printf("%.8X", bStringHash(argv[2]));
		return 0;
	}

	if ((argv[1][0] == '-'))
	{
		for (int i = 0; i < strlen(argv[2]); ++i)
			argv[2][i] = toupper(argv[2][i]);

		if ((argv[1][1] == 's'))
			return bStringHash(argv[2]);
		if ((argv[1][1] == 'n'))
			printf("%.8X\n", bStringHash(argv[2]));
		else
			printf("%.8X", bStringHash(argv[2]));
		return 0;
	}

	for (int i = 0; i < strlen(argv[1]); ++i)
		argv[1][i] = toupper(argv[1][i]);
	
	printf("%.8X", bStringHash(argv[1]));
    return 0;
}
