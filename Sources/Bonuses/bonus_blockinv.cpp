/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/Bonuses/bonus_blockinv.hpp"
#include "../../Headers/Core/matrix.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Bonus_BlockInv::Bonus_BlockInv(Texture* blockTexture, Collider* collision, char once) : Placeable()
{
    m_sprite.setTexture(*blockTexture);
    m_sprite.setTextureRect(IntRect(96, 0, 32, 32));

    m_collision = collision;

    m_hitspeed = 0;
    m_hit = 0;

    m_once = once;
}

void Bonus_BlockInv::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    m_startheight = pos.y;
}

void Bonus_BlockInv::update()
{
    if (m_hit > 0)
        return;

    Vector2f pos(m_sprite.getPosition());
    if (blockhitter.intersects(FloatRect(pos, Vector2f(32, 32))))
    {
        m_hitspeed = -2.5;
        m_hit = 1;

        m_collision->destroyed = true;

        if (blockkiller_create)
        {
            blockkiller.left = pos.x;
            blockkiller.width = 32;
            blockkiller.top = pos.y - 1;
            blockkiller_create = false;
        }
        else
        {
            blockkiller.left = floor(blockhitter.left / 32.0) * 32.0;
            blockkiller.width = ceil((blockhitter.left + 28) / 32.0) * 32.0 - blockkiller.left;
            blockkiller.top = pos.y - 1;
        }

        if (zoneb)
            collisionMatrixb->setValue(pos.x / 32, pos.y / 32, 1);
        else
            collisionMatrix->setValue(pos.x / 32, pos.y / 32, 1);

        hit();
    }
}

void Bonus_BlockInv::secureUpdate()
{
    if (m_hit == 1)
    {
        Vector2f pos(m_sprite.getPosition());

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
    }
}

void Bonus_BlockInv::afterUpdate()
{
}

Vector2f Bonus_BlockInv::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Bonus_BlockInv::getSize() const
{
    return Vector2f(32, 32);
}

ID Bonus_BlockInv::getID() const
{
    return ID_INVISIBLE;
}
