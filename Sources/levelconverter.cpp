#include <fstream>
#include <string>

using namespace std;

ifstream oldfile;
ofstream newfile;

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        string filename(argv[1]);

        // File readance :
        oldfile.open(filename.c_str(), ios::binary);

        size_t lastdot(filename.find_last_of('.'));

        char CMLid[4];
        char nullsuite[5] {0, 0, 0, 0, 0};
        char c;

        if (!oldfile.good())
            return 1;

        // CML or CMW validity check :
        oldfile.read(CMLid, 4);

        if (CMLid[0] != 'C' || CMLid[1] != 'M' || (CMLid[2] != 'L' && CMLid[2] != 'W'))
            return 1;

        if (lastdot != string::npos)
            filename = filename.substr(0, lastdot);

        if (CMLid[2] == 'L')
            filename += "_clean.cml";
        else
            filename += "_clean.cmw";

        newfile.open(filename.c_str(), ios::binary);

        if (!newfile.good())
            return 1;

        // Old file positioning :
        for (register unsigned char i = 0; i < 5; i++)
        {
            do
                c = oldfile.get();
            while (c != '\0');
        }

        // New file base data place :
        newfile.write("CML", 3);
        newfile.write(&CMLid[3], 1);

        newfile.write(nullsuite, 5);

        while (!oldfile.eof())
        {
            c = oldfile.get();
            newfile.put(c);
        }
    }

    return 0;
}

void closefiles()
{
    if (oldfile.is_open())
        oldfile.close();

    if (newfile.is_open())
        newfile.close();
}
