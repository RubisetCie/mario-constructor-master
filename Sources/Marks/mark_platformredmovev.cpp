/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Marks/mark_platformredmovev.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Mark_PlatformRedMoveV::Mark_PlatformRedMoveV(const Texture& texture, float target, float speed, Collider* const collision) : PlatformMov(texture, speed, collision)
{
    m_targetpoint = target;

    m_aabb.width = 96;
    m_aabb.height = 32;
}

void Mark_PlatformRedMoveV::setPosition(const Vector2f& pos)
{
    m_startpoint = pos.y;

    if (m_targetpoint > m_startpoint)
    {
        m_startpoint = m_targetpoint;
        m_targetpoint = pos.y;

        m_forward = false;
    }
    else if (m_targetpoint == m_startpoint)
    {
        m_speed = 0;
        m_collider->type = C_IMMOBILE;
    }

    PlatformMov::setPosition(pos);
}

void Mark_PlatformRedMoveV::update()
{
}

void Mark_PlatformRedMoveV::secureUpdate()
{
    // Move :
    if (m_speed > 0)
    {
        if (m_forward)
        {
            if (m_targetpoint < m_aabb.top - 8)
            {
                if (m_movespeed > -m_speed)
                    m_movespeed -= m_speed / 8.0;
            }
            else
                m_forward = false;
        }
        else
        {
            if (m_startpoint > m_aabb.top + 8)
            {
                if (m_movespeed < m_speed)
                    m_movespeed += m_speed / 8.0;
            }
            else
                m_forward = true;
        }

        m_aabb.top += m_movespeed;

        m_sprite.setPosition(m_aabb.left, roundf(m_aabb.top));

        m_collider->rect.top = m_aabb.top;
        m_collider->speed.y = m_movespeed;
    }
}

Vector2f Mark_PlatformRedMoveV::getSize() const
{
    return Vector2f(96, 16);
}

void Mark_PlatformRedMoveV::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
