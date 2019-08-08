/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

#ifdef DEBUGMODE
#include <iostream>
#endif

#include "../../Headers/Graphics/dragger.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Dragger::Dragger(Texture* mover, Texture* fonttex, unsigned int number)
{
    m_mover.setTexture(*mover);

    m_textid.setTexture(*fonttex);
    m_textid.setTextureRect(IntRect(number * 15, 16, 15, 16));

    m_screenproc.setSize(Vector2f(640, 480));
    m_screenproc.setOutlineColor(Color(128, 0, 0));
    m_screenproc.setOutlineThickness(4);
    m_screenproc.setFillColor(Color::Transparent);

    m_grabnode = false;
}

void Dragger::setPosition(Vector2f position)
{
    m_mover.setPosition(position);
    m_textid.setPosition(position + Vector2f(8, 8));
    m_screenproc.setPosition(position - Vector2f(304, 224));
}

void Dragger::update()
{
    extern bool toolbarVisible;

    Vector2f nodePos(m_mover.getPosition());

    m_mover.setColor(Color(20, 20, 20, 255));

    if (toolbarVisible)
    {
        if (!((mpos_absolute.x > 572 && mpos_absolute.y > 448) || (mpos_absolute.x < 75 && mpos_absolute.y < 478 && mpos_absolute.y > 414)))
        {
            if (FloatRect(nodePos.x, nodePos.y, 32, 32).contains(mpos_relative))
            {
                m_mover.setColor(Color(255, 255, 128, 255));

                if (mousePressed == 1 || middlPressed == 1)
                    m_grabnode = true;
            }
        }
        else
            m_grabnode = false;
    }
    else
    {
        if (FloatRect(nodePos.x, nodePos.y, 32, 32).contains(mpos_relative))
        {
            m_mover.setColor(Color(255, 255, 128, 255));

            if (mousePressed == 1 || middlPressed == 1)
                m_grabnode = true;
        }
    }

    if (mousePressed == 0 && middlPressed == 0)
        m_grabnode = false;

    if (m_grabnode)
    {
        extern bool sectionb;

        extern Vector2i roomScale;
        extern Vector2i roomScaleb;

        Vector2f moveablePos = mpos_relative;

        if (Keyboard::isKeyPressed(Keyboard::LShift))
        {
            moveablePos.x = roundf(moveablePos.x / 16) * 16;
            moveablePos.y = roundf(moveablePos.y / 16) * 16;
        }

        if (moveablePos.x < 320)
            moveablePos.x = 320;

        if (moveablePos.y < 240)
            moveablePos.y = 240;

        if (sectionb)
        {
            if (moveablePos.x > (roomScaleb.x * 640) - 320)
                moveablePos.x = (roomScaleb.x * 640) - 320;

            if (moveablePos.y > (roomScaleb.y * 480) - 240)
                moveablePos.y = (roomScaleb.y * 480) - 240;
        }
        else
        {
            if (moveablePos.x > (roomScale.x * 640) - 320)
                moveablePos.x = (roomScale.x * 640) - 320;

            if (moveablePos.y > (roomScale.y * 480) - 240)
                moveablePos.y = (roomScale.y * 480) - 240;
        }

        m_mover.setColor(Color::White);
        m_mover.setPosition(moveablePos - Vector2f(16, 16));

        m_textid.setPosition(moveablePos - Vector2f(8, 8));

        m_screenproc.setPosition(moveablePos - Vector2f(320, 240));
    }
}

Vector2f Dragger::getPosition() const
{
    return m_mover.getPosition();
}

void Dragger::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_mover);
    target.draw(m_textid);
    target.draw(m_screenproc);
}
