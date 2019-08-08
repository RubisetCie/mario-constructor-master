/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>
#endif

#include "../../Headers/globals.hpp"
#include "../../Headers/Graphics/regulator.hpp"

using namespace sf;
using namespace std;

Regulator::Regulator(Texture* slider, string helptext, void (*ptrFunction) (float sliderPos))
{
    m_slider.setTexture(*slider);

    m_grabslider = false;

    m_minx = 0;
    m_maxx = 0;

    m_callfunction = ptrFunction;

    m_helptext = helptext;
}

void Regulator::setParams(float x, float y, float minx, float maxx)
{
    m_slider.setPosition(x, y);

    m_minx = minx;
    m_maxx = maxx;
}

void Regulator::setPosition(float position)
{
    Vector2f originalPos(m_slider.getPosition());

    m_slider.setPosition(m_minx + position, originalPos.y);
}

void Regulator::update()
{
    extern Text* helpText;

    Vector2f sliderPos(m_slider.getPosition());

    m_slider.setColor(Color(255, 128, 0, 255));

    if (FloatRect(sliderPos.x, sliderPos.y, 16, 32).contains(mpos_relative))
    {
        m_slider.setColor(Color::Yellow);

        if (!m_grabslider)
        {
            if (helpText->getString() != m_helptext)
                helpText->setString(m_helptext);
        }

        if (mousePressed == 1)
        {
            m_grabslider = true;

            if (helpText->getString() == m_helptext)
                helpText->setString("");
        }
    }
    else
    {
        if (helpText->getString() == m_helptext)
            helpText->setString("");
    }

    if (mousePressed == 0 && m_grabslider)
    {
        m_grabslider = false;

        m_callfunction(getPosition());
    }

    if (m_grabslider)
    {
        float moveableX = mpos_relative.x - 8;

        if (moveableX < m_minx)
            moveableX = m_minx;

        if (moveableX > m_maxx)
            moveableX = m_maxx;

        m_slider.setColor(Color::White);
        m_slider.setPosition(moveableX, sliderPos.y);
    }
}

float Regulator::getPosition() const
{
    return m_slider.getPosition().x - m_minx;
}

void Regulator::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_slider);
}
