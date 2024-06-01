/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include "../../Headers/Core/matrix.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif
#include <cstddef>
#include <vector>

using namespace std;

Matrix::Matrix(unsigned int startWidth, unsigned int startHeight, unsigned char value)
{
    for (register unsigned int i = 0; i < startWidth; i++)
    {
        m_matrix.push_back(NULL);
        m_matrix[i] = new vector<unsigned char>;

        for (register unsigned int j = 0; j < startHeight; j++)
            m_matrix[i]->emplace_back(value);
    }
}

Matrix::~Matrix()
{
    unsigned int size = m_matrix.size();

    for (register unsigned int i = 0; i < size; i++)
    {
        m_matrix[i]->clear();
        delete m_matrix[i];
    }

    m_matrix.clear();
}

void Matrix::resize(unsigned int width, unsigned int height, unsigned char value)
{
    if (width < m_matrix.size())
    {
        for (register unsigned int i = m_matrix.size()-1; i >= width; i--)
        {
            m_matrix[i]->clear();
            m_matrix.pop_back();
        }
    }
    else if (width > m_matrix.size())
    {
        for (register unsigned int i = m_matrix.size(); i < width; i++)
        {
            unsigned int currentHeight = m_matrix[0]->size();

            m_matrix.push_back(NULL);
            m_matrix[i] = new vector<unsigned char>;

            for (register unsigned int j = 0; j < currentHeight; j++)
                m_matrix[i]->emplace_back(value);
        }
    }

    if (height < m_matrix[0]->size())
    {
        for (register unsigned int i = 0; i < m_matrix.size(); i++)
        {
            for (register unsigned int j = m_matrix[i]->size()-1; j >= height; j--)
                m_matrix[i]->pop_back();
        }
    }
    else if (height > m_matrix[0]->size())
    {
        for (register unsigned int i = 0; i < m_matrix.size(); i++)
        {
            for (register unsigned int j = m_matrix[i]->size(); j < height; j++)
                m_matrix[i]->emplace_back(value);
        }
    }

    #ifdef DEBUGMODE
    cout << "New Width : " << m_matrix.size() << endl;
    cout << "New Height : " << m_matrix[0]->size() << endl;

    cout << "First Value : " << static_cast<int>((*m_matrix[0])[0]) << endl;
    cout << "Last Value : " << static_cast<int>((*m_matrix[m_matrix.size()-1])[m_matrix[0]->size()-1]) << endl;
    #endif
}

void Matrix::setValue(unsigned int x, unsigned int y, unsigned char value)
{
    (*m_matrix[x])[y] = value;
}

unsigned char Matrix::getValue(unsigned int x, unsigned int y)
{
    return (*m_matrix[x])[y];
}

unsigned char* Matrix::getReference(unsigned int x, unsigned int y)
{
    return &((*m_matrix[x])[y]);
}
