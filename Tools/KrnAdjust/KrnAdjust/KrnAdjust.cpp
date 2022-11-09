// Font Spacing Adjuster for FFN .krn files
// by Xan/Tenjoin

#include <iostream>
#include <string>

using namespace std;

char line[512];
char lastline[512];

void AdjustTheKern(const char* filename, const char* outfilename, int32_t x, int32_t y, int32_t advance)
{
    FILE* fin = fopen(filename, "rb");
    if (!fin)
        return;

    FILE* fout = fopen(outfilename, "wb");
    if (!fout)
        return;

    int32_t value = 0;

    while (!feof(fin))
    {
        memset(line, 0, 512);
        fgets(line, 512, fin);
        if (strncmp(line, "\txOffset", 8) == 0)
        {
            sscanf(line, "\txOffset %d\n", &value);
            value += x;
            sprintf(line, "\txOffset %d\n", value);
        }
        else if (strncmp(line, "\tyOffset", 8) == 0)
        {
            sscanf(line, "\tyOffset %d\n", &value);
            value += y;
            sprintf(line, "\tyOffset %d\n", value);
        }
        else if (strncmp(line, "\tadvance", 8) == 0)
        {
            sscanf(line, "\tadvance %d\n", &value);
            value += advance;
            sprintf(line, "\tadvance %d\n", value);
        }

        fputs(line, fout);
    }

    fclose(fout);
    fclose(fin);
}

void ScaleTheKern(const char* filename, const char* outfilename, float x, float y, float advance)
{
    FILE* fin = fopen(filename, "rb");
    if (!fin)
        return;

    FILE* fout = fopen(outfilename, "wb");
    if (!fout)
        return;

    int32_t value = 0;
    float fvalue = 0;

    while (!feof(fin))
    {
        memset(line, 0, 512);
        fgets(line, 512, fin);
        if (strncmp(line, "\txOffset", 8) == 0)
        {
            sscanf(line, "\txOffset %d\n", &value);
            fvalue = (float)value;
            if (x)
                fvalue *= x;
            value = (int32_t)(fvalue);
            sprintf(line, "\txOffset %d\n", value);
        }
        else if (strncmp(line, "\tyOffset", 8) == 0)
        {
            sscanf(line, "\tyOffset %d\n", &value);
            fvalue = (float)value;
            if (y)
                fvalue *= y;
            value = (int32_t)(fvalue);
            sprintf(line, "\tyOffset %d\n", value);
        }
        else if (strncmp(line, "\tadvance", 8) == 0)
        {
            sscanf(line, "\tadvance %d\n", &value);
            fvalue = (float)value;
            if (advance)
                fvalue *= advance;
            value = (int32_t)(fvalue);
            sprintf(line, "\tadvance %d\n", value);
        }

        fputs(line, fout);
    }

    fclose(fout);
    fclose(fin);
}

int main(int argc, char* argv[])
{
    printf("FFN Kern Batch Adjuster\n");

    if (argc < 5)
    {
        printf("USAGE (adjust, integer numbers): %s InputFile OutFile Xadj Yadj AdvanceAdj\nUSAGE (scaling, float numbers): %s -s InputFile OutFile Xadj Yadj AdvanceAdj\nInputFile is an .krn file exported from Font Wizard (File > Export Spacing)\n", argv[0], argv[0]);
        return -1;
    }

    if ((argv[1][0] == '-') && (argv[1][1] == 's'))
    {
        printf("Scaling kerning by following parameters:\nX: %.2f\nY: %.2f\nAdvance: %.2f\nOutput file: %s\n", stof(argv[4]), stof(argv[5]), stof(argv[6]), argv[3]);

        ScaleTheKern(argv[2], argv[3], stof(argv[4]), stof(argv[5]), stof(argv[6]));
        return 0;
    }

    printf("Adjusting kerning by following parameters:\nX: %d\nY: %d\nAdvance: %d\nOutput file: %s\n", stoi(argv[3]), stoi(argv[4]), stoi(argv[5]), argv[2]);

    AdjustTheKern(argv[1], argv[2], stoi(argv[3]), stoi(argv[4]), stoi(argv[5]));
    return 0;
}
