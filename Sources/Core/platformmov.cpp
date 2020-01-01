/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include "../../Headers/Core/platformmov.hpp"

using namespace sf;

PlatformMov::PlatformMov(const Texture& texture, float speed, Collider* const collision) : Placeable()
{
    m_sprite.setTexture(texture);

    m_forward = true;
    m_speed = speed;

    m_movespeed = 0;

    m_collider = collision;
}

void PlatformMov::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;
}

void PlatformMov::afterUpdate()
{
}

ID PlatformMov::getID() const
{
    return ID_USELESS;
}

Vector2f PlatformMov::getPosition() const
{
    return m_sprite.getPosition();
}

