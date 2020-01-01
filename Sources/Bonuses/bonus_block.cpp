/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/Bonuses/bonus_block.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Bonus_Block::Bonus_Block(Texture* blockTexture) : Placeable()
{
    m_sprite.setTexture(*blockTexture);
    m_sprite.setTextureRect(IntRect(0, 0, 32, 32));

    m_hitspeed = 0;
    m_hit = 0;
}

void Bonus_Block::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);

    m_startheight = pos.y;
}

void Bonus_Block::update()
{
    if (m_hit > 0)
        return;

    if (frame_Water % 8 == 0)
        m_sprite.setTextureRect(IntRect((frame_Water/8) * 32, 0, 32, 32));

    FloatRect bbox(m_sprite.getPosition(), Vector2f(32, 32));

    if (blockhitter.intersects(bbox))
    {
        m_sprite.setTextureRect(IntRect(96, 0, 32, 32));

        m_hitspeed = -2.5;
        m_hit = 1;

        if (blockkiller_create)
        {
            blockkiller.left = bbox.left;
            blockkiller.width = 32;
            blockkiller.top = bbox.top - 1;
            blockkiller_create = false;
        }
        else
        {
            blockkiller.left = floor(blockhitter.left / 32.0) * 32.0;
            blockkiller.width = ceil((blockhitter.left + 28) / 32.0) * 32.0 - blockkiller.left;
            blockkiller.top = bbox.top - 1;
        }

        hit();
    }
}

void Bonus_Block::secureUpdate()
{
    Vector2f pos(m_sprite.getPosition());

    if (m_hit == 1)
    {
        if (pos.y <= m_startheight - m_hitspeed)
        {
            m_hitspeed += 0.5;
            m_sprite.move(0, m_hitspeed);
        }
        else
        {
            m_sprite.setPosition(pos.x, m_startheight);
            m_hit = 2;
        }

        return;
    }
    else if (m_hit == 0)
    {
        FloatRect bbox(pos, Vector2f(32, 32));

        if (spritehitter.intersects(bbox))
        {
            m_sprite.setTextureRect(IntRect(96, 0, 32, 32));

            m_hitspeed = -2.5;
            m_hit = 1;

            hit();
        }
    }
}

void Bonus_Block::afterUpdate()
{
}

Vector2f Bonus_Block::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Bonus_Block::getSize() const
{
    return Vector2f(32, 32);
}

ID Bonus_Block::getID() const
{
    return ID_USELESS;
}
