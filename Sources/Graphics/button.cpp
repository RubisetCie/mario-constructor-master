/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Graphics/button.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace sf;
using namespace std;

Button::Button(Texture& texture, void (*ptrFunction) (), string helptext, const IntRect& rectangle)
{
    m_helptext = helptext;

    Vector2u texSize(texture.getSize());

    m_sprite.setTexture(texture);

    if (rectangle != IntRect(0, 0, 0, 0))
        m_sprite.setTextureRect(rectangle);

    m_sprite.setOrigin(texSize.x / 2, texSize.y / 2);

    m_callfunction = ptrFunction;
}

void Button::setTexture(Texture& texture)
{
    m_sprite.setTexture(texture);
}

void Button::setTextureRect(IntRect rectangle)
{
    m_sprite.setTextureRect(rectangle);
}

void Button::setSound(FMOD_SOUND** clickSound)
{
    m_clicksound = clickSound;
}

void Button::setPosition(float x, float y)
{
    m_sprite.setPosition(x, y);
}

void Button::setRotation(float angle)
{
    m_sprite.setRotation(angle);
}

void Button::setScale(float x, float y)
{
    m_sprite.setScale(x, y);
}

void Button::update()
{
    extern Text* helpText;

    FloatRect aabb(m_sprite.getGlobalBounds());

    if (aabb.contains(mpos_relative))
    {
        Uint8 alpha = m_sprite.getColor().a;

        if (helpText->getString() != m_helptext)
            helpText->setString(m_helptext);

        if (alpha > 140)
            alpha -= 12;
        else
            alpha = 128;

        m_sprite.setColor(Color(255, 255, 255, alpha));

        if (mousePressed == 1)
        {
            if (m_clicksound != NULL)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), *m_clicksound, 0, NULL);

            (*m_callfunction)();
        }
    }
    else
    {
        Uint8 alpha = m_sprite.getColor().a;

        if (helpText->getString() == m_helptext)
            helpText->setString("");

        if (alpha < 232)
            alpha += 12;
        else
            alpha = 255;

        m_sprite.setColor(Color(255, 255, 255, alpha));
    }
}

void Button::freeze()
{
    m_sprite.setColor(Color::White);
}

void Button::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
