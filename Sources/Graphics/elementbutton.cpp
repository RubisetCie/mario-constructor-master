/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Graphics/elementbutton.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

ElementButton::ElementButton(const Texture& texture, const IntRect& rectangle, unsigned int elementnumb, void (*ptrFunction) (unsigned int elementNumb))
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(rectangle);
    m_sprite.setOrigin(24, 24);

    m_selected = false;

    m_elementnumb = elementnumb;

    m_callfunction = ptrFunction;
}

void ElementButton::setSound(FMOD_SOUND** clickSound)
{
    m_clicksound = clickSound;
}

void ElementButton::setPosition(float x, float y)
{
    m_sprite.setPosition(x, y);
}

void ElementButton::Unselected()
{
    m_selected = false;

    m_sprite.setColor(Color::White);
}

void ElementButton::update()
{
    FloatRect aabb(m_sprite.getPosition().x-24, m_sprite.getPosition().y-24, 48, 48);

    if (aabb.contains(mpos_relative))
    {
        float scale = m_sprite.getScale().x;

        if (scale > 0.85)
            scale -= 0.1;
        else
            scale = 0.75;

        m_sprite.setScale(scale, scale);

        if (mousePressed == 1)
        {
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), *m_clicksound, 0, NULL);

            if (m_selected)
            {
                Unselected();

                (*m_callfunction) (255);
            }
            else
            {
                m_selected = true;

                m_sprite.setColor(Color::Yellow);

                (*m_callfunction) (m_elementnumb);
            }
        }
    }
    else
    {
        float scale = m_sprite.getScale().x;

        if (scale < 0.9)
            scale += 0.1;
        else
            scale = 1;

        m_sprite.setScale(scale, scale);
    }
}

void ElementButton::freeze()
{
    m_sprite.setScale(1, 1);
}

void ElementButton::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
