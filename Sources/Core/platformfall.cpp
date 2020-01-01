/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <cmath>
#include "../../Headers/Core/platformfall.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

PlatformFall::PlatformFall(const Texture& texture, Collider* const collision) : Placeable()
{
    m_sprite.setTexture(texture);

    m_movespeed = 0;

    m_collider = collision;
}

void PlatformFall::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;
}

void PlatformFall::update()
{
    if (m_movespeed > 0)
        return;

    if (player != NULL)
    {
        if (player->m_platform == m_collider)
            m_movespeed = 0.1f;
    }
}

void PlatformFall::secureUpdate()
{
    // Move :
    if (m_movespeed > 0)
    {
        if (m_movespeed < PLATFORMFALL_MAXVSPEED)
            m_movespeed += star_gravity;

        m_aabb.top += m_movespeed;

        m_sprite.setPosition(m_aabb.left, roundf(m_aabb.top));

        m_collider->rect.top = m_aabb.top;
        m_collider->speed.y = m_movespeed;
    }

    if (zoneb)
    {
        if (m_aabb.top > levelScaleb.y * 480 + 128)
        {
            m_collider->destroyed = true;
            m_destroyed = true;
        }
    }
    else
    {
        if (m_aabb.top > levelScale.y * 480 + 128)
        {
            m_collider->destroyed = true;
            m_destroyed = true;
        }
    }
}

Vector2f PlatformFall::getPosition() const
{
    return m_sprite.getPosition();
}

ID PlatformFall::getID() const
{
    return ID_USELESS;
}

void PlatformFall::afterUpdate()
{
}
