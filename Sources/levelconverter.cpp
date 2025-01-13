#include <fstream>
#include <string>

#include <cstdio>

using namespace std;

ifstream oldfile;
ofstream newfile;

// Old Tileset:
static const int oldtileset[13][15] =
{
    {0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14},
    {15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29},
    {30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44},
    {45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59},
    {60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74},
    {75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89},
    {90,  91,  92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102, 103, 104},
    {105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119},
    {120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134},
    {135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149},
    {150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164},
    {165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179},
    {180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194}
};

// New Tileset:
static const int newtileset[12][18] =
{
    {0,  1,  2,  -1,  3,  4,  5, -1,  6,  7,  8, -1, 9, 10, 11, 12, 13, 14},
    {15, 16, 17, -1, 18, 19, 20, -1, 21, 22, 23, -1, 24, 25, 26, 27, 28, 29},
    {30, 31, 32, -1, 33, 34, 35, -1, 68, 39, 40, 41, 36, 37, 38, 42, 43, 44},
    {105,106,107, -1, 45, 46, 47, -1, 67, 54, 55, 56, 51, 52, 53, 57, 58, 59},
    {120,121,122, -1, 60, 61, 62, -1, 66, 78, 79, 80,102,103,104, 69, 70, 71},
    {75, 76, 77, 129,130,131, 48, 49, 50, 93, 94, 95,117,118,119, 72, 73, 74},
    {90, 91, 92, 144,145,146, 63, 64, 65,108,109,110, 132,133,134, 87, 88, 89},
    {135,136,137,138,139,140,141,142,143,162,163,164, 147,148,149, -1, -1, -1},
    {150,151,152,153,154,155,156,157,158,159, 84, 85, 86, 81, 82, 83, -1, -1},
    {165,166,167,168,169,170,171,172,173,161, 99,100,101, 96, 97, 98, 174, 175},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, 160,114,115,116,111,112,113, 186, 187},
    {180,181,182,183,184,185,186,184, -1, -1,126,127,128,123,124,125, -1, -1}
};

typedef struct TileData
{
    float x;
    float y;

    char tilex;
    char tiley;
} TileData;

void closefiles()
{
    if (oldfile.is_open())
        oldfile.close();

    if (newfile.is_open())
        newfile.close();
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        string filename(argv[1]);

        atexit(closefiles);

        // File readance :
        oldfile.open(filename.c_str(), ios::binary);

        size_t lastdot(filename.find_last_of('.'));

        char CMLid[4];
        char c = 'a';

        if (!oldfile.good())
            return 1;

        // CML or CMW validity check :
        oldfile.read(CMLid, 4);

        if (CMLid[0] != 'C' || CMLid[1] != 'M' || CMLid[2] != 'L' || CMLid[3] != 1)
            return 1;

        if (lastdot != string::npos)
            filename = filename.substr(0, lastdot);

        filename += "_converted.cml";

        newfile.open(filename.c_str(), ios::binary);

        if (!newfile.good())
            return 1;

        // New file base data place :
        CMLid[3]++;
        newfile.write("CML", 3);
        newfile.write(&CMLid[3], 1);

        // Advance 5 null characters :
        for (register unsigned int i = 0; i < 5; i++)
        {
            c = 'a';

            while (c != '\0')
            {
                c = oldfile.get();
                newfile.put(c);
            }
        }

        // Advance 47 characters :
        for (register unsigned int i = 0; i < 47; i++)
        {
            c = oldfile.get();
            newfile.put(c);
        }

        // Read undetermined number of character :
        {
            unsigned char nodeSize;

            oldfile.read(reinterpret_cast<char*>(&nodeSize), 1);
            newfile.write(reinterpret_cast<char*>(&nodeSize), 1);

            if (nodeSize != 0)
            {
                for (register unsigned char i = 0; i < nodeSize; i++)
                {
                    for (register unsigned int u = 0; u < 8; u++)
                    {
                        c = oldfile.get();
                        newfile.put(c);
                    }
                }
            }
        }

        // Advance 9 characters :
        for (register unsigned int i = 0; i < 9; i++)
        {
            c = oldfile.get();
            newfile.put(c);
        }

        // Read undetermined number of character :
        {
            unsigned char spotSize;

            oldfile.read(reinterpret_cast<char*>(&spotSize), 1);
            newfile.write(reinterpret_cast<char*>(&spotSize), 1);

            if (spotSize != 0)
            {
                for (register unsigned char i = 0; i < spotSize; i++)
                {
                    for (register unsigned int u = 0; u < 20; u++)
                    {
                        c = oldfile.get();
                        newfile.put(c);
                    }
                }
            }
        }

        // Advance 5 characters :
        for (register unsigned int i = 0; i < 5; i++)
        {
            c = oldfile.get();
            newfile.put(c);
        }

        // Read undetermined number of character :
        {
            unsigned char spotSize;

            oldfile.read(reinterpret_cast<char*>(&spotSize), 1);
            newfile.write(reinterpret_cast<char*>(&spotSize), 1);

            if (spotSize != 0)
            {
                for (register unsigned char i = 0; i < spotSize; i++)
                {
                    for (register unsigned int u = 0; u < 20; u++)
                    {
                        c = oldfile.get();
                        newfile.put(c);
                    }
                }
            }
        }

        // Advance 25 characters :
        for (register unsigned int i = 0; i < 25; i++)
        {
            c = oldfile.get();
            newfile.put(c);
        }

        c = 'a';

        // Get to the second null character :
        while (c != '\0')
        {
            c = oldfile.get();
            newfile.put(c);
        }

        c = 'a';

        while (c != '\0')
        {
            c = oldfile.get();
            newfile.put(c);
        }

        // Switching tile place :
        for (register unsigned int counter = 0; counter < 6; counter++)
        {
            unsigned int listSize;

            oldfile.read(reinterpret_cast<char*>(&listSize), 4);
            newfile.write(reinterpret_cast<char*>(&listSize), 4);

            if (listSize != 0)
            {
                for (register unsigned int i = 0; i < listSize; i++)
                {
                    TileData currentData;
                    int tileid, tilex = -1, tiley;

                    oldfile.read(reinterpret_cast<char*>(&currentData), sizeof(TileData));

                    if (currentData.tilex >= 15)
                    {
                        currentData.tilex += 3;
                        goto LB_WRITENEW;
                    }

                    tileid = oldtileset[currentData.tiley][currentData.tilex];

                    for (register unsigned int jy = 0; jy < 12; jy++)
                    {
                        for (register unsigned int jx = 0; jx < 18; jx++)
                        {
                            if (newtileset[jy][jx] == tileid)
                            {
                                tilex = jx;
                                tiley = jy;

                                goto LB_EXITLOOP;
                            }
                        }
                    }

                    LB_EXITLOOP:

                    if (tilex == -1)
                    {
                        tilex = 3;
                        tiley = 2;
                    }

                    currentData.tilex = tilex;
                    currentData.tiley = tiley;

                    LB_WRITENEW:

                    newfile.write(reinterpret_cast<char*>(&currentData), sizeof(TileData));
                }
            }
        }

        // Writing the rest of the level :
        while (!oldfile.eof())
        {
            c = oldfile.get();
            newfile.put(c);
        }
    }

    return 0;
}
