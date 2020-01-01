/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/Bonuses/bonus_toggle.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Bonus_Toggle::Bonus_Toggle(Texture* blockTexture, unsigned char slot, bool transparent) : Placeable()
{
    m_sprite.setTexture(*blockTexture);
    m_sprite.setTextureRect(IntRect(slot * 32, 0, 32, 32));

    m_slot = slot;
    m_inverted = transparent;

    if (transparent)
        m_sprite.setColor(Color(255, 255, 255, 128));
}

void Bonus_Toggle::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Bonus_Toggle::update()
{
}

void Bonus_Toggle::secureUpdate()
{
}

void Bonus_Toggle::afterUpdate()
{
    if (game_toggleSolidity[m_slot] == 1)
    {
        if (m_inverted)
        {
            m_sprite.setColor(Color(255, 255, 255, 128));

            (*m_solid) = 0;
        }
        else
        {
            m_sprite.setColor(Color::White);

            (*m_solid) = 1;
        }
    }
    else if (game_toggleSolidity[m_slot] == 3)
    {
        if (m_inverted)
        {
            m_sprite.setColor(Color::White);

            (*m_solid) = 1;
        }
        else
        {
            m_sprite.setColor(Color(255, 255, 255, 128));

            (*m_solid) = 0;
        }
    }
}

void Bonus_Toggle::attributeSolid(unsigned char* s)
{
    m_solid = s;
}

Vector2f Bonus_Toggle::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Bonus_Toggle::getSize() const
{
    return Vector2f(32, 32);
}

ID Bonus_Toggle::getID() const
{
    return ID_USELESS;
}

void Bonus_Toggle::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
