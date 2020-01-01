/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <string>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

#include "../../Headers/Graphics/spritefont.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

SpriteFont::SpriteFont(const Vector2f& position, bool right, const string& startText)
{
    unsigned int stringLength = startText.size();
    unsigned int hInc = 0;

    if (stringLength != 0)
    {
        if (right)
        {
            for (register unsigned int i = stringLength; i > 0; i--)
            {
                char currentChar = tolower(startText[i - 1]);
                bool writeChar = true;

                IntRect currentRect;

                switch (currentChar)
                {
                    case '1' : currentRect = IntRect(0, 0, 11, 16); break;
                    case '2' : currentRect = IntRect(11, 0, 15, 16); break;
                    case '3' : currentRect = IntRect(26, 0, 15, 16); break;
                    case '4' : currentRect = IntRect(41, 0, 15, 16); break;
                    case '5' : currentRect = IntRect(56, 0, 15, 16); break;
                    case '6' : currentRect = IntRect(71, 0, 15, 16); break;
                    case '7' : currentRect = IntRect(86, 0, 15, 16); break;
                    case '8' : currentRect = IntRect(101, 0, 15, 16); break;
                    case '9' : currentRect = IntRect(116, 0, 15, 16); break;
                    case '0' : currentRect = IntRect(131, 0, 15, 16); break;
                    case 'a' : currentRect = IntRect(146, 0, 15, 16); break;
                    case 'b' : currentRect = IntRect(161, 0, 15, 16); break;
                    case 'c' : currentRect = IntRect(176, 0, 15, 16); break;
                    case 'd' : currentRect = IntRect(191, 0, 15, 16); break;
                    case 'e' : currentRect = IntRect(206, 0, 15, 16); break;
                    case 'f' : currentRect = IntRect(221, 0, 15, 16); break;
                    case 'g' : currentRect = IntRect(236, 0, 15, 16); break;
                    case 'h' : currentRect = IntRect(251, 0, 15, 16); break;
                    case 'i' : currentRect = IntRect(266, 0, 11, 16); break;
                    case 'j' : currentRect = IntRect(277, 0, 15, 16); break;
                    case 'k' : currentRect = IntRect(292, 0, 15, 16); break;
                    case 'l' : currentRect = IntRect(307, 0, 13, 16); break;
                    case 'm' : currentRect = IntRect(320, 0, 15, 16); break;
                    case 'n' : currentRect = IntRect(335, 0, 15, 16); break;
                    case 'o' : currentRect = IntRect(350, 0, 15, 16); break;
                    case 'p' : currentRect = IntRect(365, 0, 15, 16); break;
                    case 'q' : currentRect = IntRect(380, 0, 15, 16); break;
                    case 'r' : currentRect = IntRect(395, 0, 15, 16); break;
                    case 's' : currentRect = IntRect(410, 0, 15, 16); break;
                    case 't' : currentRect = IntRect(425, 0, 15, 16); break;
                    case 'u' : currentRect = IntRect(440, 0, 15, 16); break;
                    case 'v' : currentRect = IntRect(455, 0, 15, 16); break;
                    case 'w' : currentRect = IntRect(470, 0, 15, 16); break;
                    case 'x' : currentRect = IntRect(485, 0, 15, 16); break;
                    case 'y' : currentRect = IntRect(500, 0, 15, 16); break;
                    case 'z' : currentRect = IntRect(515, 0, 15, 16); break;
                    case '.' : currentRect = IntRect(530, 0, 7, 16); break;
                    case '-' : currentRect = IntRect(537, 0, 15, 16); break;
                    case '&' : currentRect = IntRect(552, 0, 15, 16); break;
                    case '@' : currentRect = IntRect(567, 0, 15, 16); break;
                    case '$' : currentRect = IntRect(582, 0, 15, 16); break;
                    case ' ' : currentRect = IntRect(537, 0, 7, 1); break;
                    default  : writeChar = false; break;
                }

                if (writeChar)
                {
                    m_text.emplace_back(Vertex(Vector2f(position.x - hInc, position.y), Color::White, Vector2f(currentRect.left + currentRect.width, currentRect.top)));
                    m_text.emplace_back(Vertex(Vector2f(position.x - hInc - currentRect.width, position.y), Color::White, Vector2f(currentRect.left, currentRect.top)));
                    m_text.emplace_back(Vertex(Vector2f(position.x - hInc - currentRect.width, position.y + 16), Color::White, Vector2f(currentRect.left, currentRect.top + currentRect.height)));
                    m_text.emplace_back(Vertex(Vector2f(position.x - hInc, position.y + 16), Color::White, Vector2f(currentRect.left + currentRect.width, currentRect.top + currentRect.height)));

                    hInc += currentRect.width;
                }
            }
        }
        else
        {
            for (register unsigned int i = 0; i < stringLength; i++)
            {
                char currentChar = tolower(startText[i]);
                bool writeChar = true;

                IntRect currentRect;

                switch (currentChar)
                {
                    case '1' : currentRect = IntRect(0, 0, 11, 16); break;
                    case '2' : currentRect = IntRect(11, 0, 15, 16); break;
                    case '3' : currentRect = IntRect(26, 0, 15, 16); break;
                    case '4' : currentRect = IntRect(41, 0, 15, 16); break;
                    case '5' : currentRect = IntRect(56, 0, 15, 16); break;
                    case '6' : currentRect = IntRect(71, 0, 15, 16); break;
                    case '7' : currentRect = IntRect(86, 0, 15, 16); break;
                    case '8' : currentRect = IntRect(101, 0, 15, 16); break;
                    case '9' : currentRect = IntRect(116, 0, 15, 16); break;
                    case '0' : currentRect = IntRect(131, 0, 15, 16); break;
                    case 'a' : currentRect = IntRect(146, 0, 15, 16); break;
                    case 'b' : currentRect = IntRect(161, 0, 15, 16); break;
                    case 'c' : currentRect = IntRect(176, 0, 15, 16); break;
                    case 'd' : currentRect = IntRect(191, 0, 15, 16); break;
                    case 'e' : currentRect = IntRect(206, 0, 15, 16); break;
                    case 'f' : currentRect = IntRect(221, 0, 15, 16); break;
                    case 'g' : currentRect = IntRect(236, 0, 15, 16); break;
                    case 'h' : currentRect = IntRect(251, 0, 15, 16); break;
                    case 'i' : currentRect = IntRect(266, 0, 11, 16); break;
                    case 'j' : currentRect = IntRect(277, 0, 15, 16); break;
                    case 'k' : currentRect = IntRect(292, 0, 15, 16); break;
                    case 'l' : currentRect = IntRect(307, 0, 13, 16); break;
                    case 'm' : currentRect = IntRect(320, 0, 15, 16); break;
                    case 'n' : currentRect = IntRect(335, 0, 15, 16); break;
                    case 'o' : currentRect = IntRect(350, 0, 15, 16); break;
                    case 'p' : currentRect = IntRect(365, 0, 15, 16); break;
                    case 'q' : currentRect = IntRect(380, 0, 15, 16); break;
                    case 'r' : currentRect = IntRect(395, 0, 15, 16); break;
                    case 's' : currentRect = IntRect(410, 0, 15, 16); break;
                    case 't' : currentRect = IntRect(425, 0, 15, 16); break;
                    case 'u' : currentRect = IntRect(440, 0, 15, 16); break;
                    case 'v' : currentRect = IntRect(455, 0, 15, 16); break;
                    case 'w' : currentRect = IntRect(470, 0, 15, 16); break;
                    case 'x' : currentRect = IntRect(485, 0, 15, 16); break;
                    case 'y' : currentRect = IntRect(500, 0, 15, 16); break;
                    case 'z' : currentRect = IntRect(515, 0, 15, 16); break;
                    case '.' : currentRect = IntRect(530, 0, 7, 16); break;
                    case '-' : currentRect = IntRect(537, 0, 15, 16); break;
                    case '&' : currentRect = IntRect(552, 0, 15, 16); break;
                    case '@' : currentRect = IntRect(567, 0, 15, 16); break;
                    case '$' : currentRect = IntRect(582, 0, 15, 16); break;
                    case ' ' : currentRect = IntRect(537, 0, 7, 1); break;
                    default  : writeChar = false; break;
                }

                if (writeChar)
                {
                    m_text.emplace_back(Vertex(Vector2f(position.x + hInc, position.y), Color::White, Vector2f(currentRect.left, currentRect.top)));
                    m_text.emplace_back(Vertex(Vector2f(position.x + hInc + currentRect.width, position.y), Color::White, Vector2f(currentRect.left + currentRect.width, currentRect.top)));
                    m_text.emplace_back(Vertex(Vector2f(position.x + hInc + currentRect.width, position.y + 16), Color::White, Vector2f(currentRect.left + currentRect.width, currentRect.top + currentRect.height)));
                    m_text.emplace_back(Vertex(Vector2f(position.x + hInc, position.y + 16), Color::White, Vector2f(currentRect.left, currentRect.top + currentRect.height)));

                    hInc += currentRect.width;
                }
            }
        }
    }
}

void SpriteFont::setText(const char* text, unsigned int size, const Vector2f& position, bool right)
{
    unsigned int hInc = 0;

    m_text.clear();

    #ifdef DEBUGMODE
    cout << text << endl;
    #endif // DEBUGMODE

    if (right)
    {
        for (register int i = size - 1; i >= 0; i--)
        {
            char currentChar = text[i];
            bool writeChar = true;

            IntRect currentRect;

            switch (currentChar)
            {
                case '1' : currentRect = IntRect(0, 0, 11, 16); break;
                case '2' : currentRect = IntRect(11, 0, 15, 16); break;
                case '3' : currentRect = IntRect(26, 0, 15, 16); break;
                case '4' : currentRect = IntRect(41, 0, 15, 16); break;
                case '5' : currentRect = IntRect(56, 0, 15, 16); break;
                case '6' : currentRect = IntRect(71, 0, 15, 16); break;
                case '7' : currentRect = IntRect(86, 0, 15, 16); break;
                case '8' : currentRect = IntRect(101, 0, 15, 16); break;
                case '9' : currentRect = IntRect(116, 0, 15, 16); break;
                case '0' : currentRect = IntRect(131, 0, 15, 16); break;
                default  : writeChar = false; break;
            }

            if (writeChar)
            {
                m_text.emplace_back(Vertex(Vector2f(position.x - hInc, position.y), Color::White, Vector2f(currentRect.left + currentRect.width, currentRect.top)));
                m_text.emplace_back(Vertex(Vector2f(position.x - hInc - currentRect.width, position.y), Color::White, Vector2f(currentRect.left, currentRect.top)));
                m_text.emplace_back(Vertex(Vector2f(position.x - hInc - currentRect.width, position.y + 16), Color::White, Vector2f(currentRect.left, currentRect.top + currentRect.height)));
                m_text.emplace_back(Vertex(Vector2f(position.x - hInc, position.y + 16), Color::White, Vector2f(currentRect.left + currentRect.width, currentRect.top + currentRect.height)));

                hInc += currentRect.width;
            }
        }
    }
    else
    {
        for (register unsigned int i = 0; i < size; i++)
        {
            char currentChar = tolower(text[i]);
            bool writeChar = true;

            IntRect currentRect;

            switch (currentChar)
            {
                case '1' : currentRect = IntRect(0, 0, 11, 16); break;
                case '2' : currentRect = IntRect(11, 0, 15, 16); break;
                case '3' : currentRect = IntRect(26, 0, 15, 16); break;
                case '4' : currentRect = IntRect(41, 0, 15, 16); break;
                case '5' : currentRect = IntRect(56, 0, 15, 16); break;
                case '6' : currentRect = IntRect(71, 0, 15, 16); break;
                case '7' : currentRect = IntRect(86, 0, 15, 16); break;
                case '8' : currentRect = IntRect(101, 0, 15, 16); break;
                case '9' : currentRect = IntRect(116, 0, 15, 16); break;
                case '0' : currentRect = IntRect(131, 0, 15, 16); break;
                default  : writeChar = false; break;
            }

            if (writeChar)
            {
                m_text.emplace_back(Vertex(Vector2f(position.x + hInc, position.y), Color::White, Vector2f(currentRect.left, currentRect.top)));
                m_text.emplace_back(Vertex(Vector2f(position.x + hInc + currentRect.width, position.y), Color::White, Vector2f(currentRect.left + currentRect.width, currentRect.top)));
                m_text.emplace_back(Vertex(Vector2f(position.x + hInc + currentRect.width, position.y + 16), Color::White, Vector2f(currentRect.left + currentRect.width, currentRect.top + currentRect.height)));
                m_text.emplace_back(Vertex(Vector2f(position.x + hInc, position.y + 16), Color::White, Vector2f(currentRect.left, currentRect.top + currentRect.height)));

                hInc += currentRect.width;
            }
        }
    }
}

void SpriteFont::setPosition(const Vector2f& position)
{
    unsigned int textLength = m_text.size();

    Vector2f deltaPos = m_text[0].position - position;

    if (textLength != 0)
    {
        for (vector<Vertex>::iterator it = m_text.begin(); it != m_text.end(); it++)
            it->position -= deltaPos;
    }
}

float SpriteFont::getHorizontalSize()
{
    float x_first = m_text.front().position.x;
    float x_last = m_text.back().position.x + 15;

    #ifdef DEBUGMODE
    cout << "Horizontal Size : " << abs(x_last - x_first) << endl;
    #endif // DEBUGMODE

    return abs(x_last - x_first);
}

void SpriteFont::draw(RenderTarget& target, RenderStates) const
{
    target.draw(&m_text.front(), m_text.size(), Quads, hudTxt[4]);
}
