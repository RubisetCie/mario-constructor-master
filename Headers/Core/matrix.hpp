/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MATRIX_INCLUDED
#define MATRIX_INCLUDED

#include <vector>

class Matrix
{
    public :

        Matrix(unsigned int startWidth, unsigned int startHeight, unsigned char value = 0);
        ~Matrix();

        void resize(unsigned int width, unsigned int height, unsigned char value = 0);
        void setValue(unsigned int x, unsigned int y, unsigned char value);

        unsigned char getValue(unsigned int x, unsigned int y);
        unsigned char* getReference(unsigned int x, unsigned int y);

    private :

        std::vector<std::vector<unsigned char>*> m_matrix;
};

#endif
