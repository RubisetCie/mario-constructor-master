/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Marks/mark_platformshortmoveh.hpp"
#include "../../Headers/Marks/mark_mario.hpp"

using namespace sf;

Mark_PlatformShortMoveH::Mark_PlatformShortMoveH(const Texture& texture, float target, float speed, Collider* const collision) : PlatformMov(texture, speed, collision)
{
    m_targetpoint = target + 16;

    m_aabb.width = 32;
    m_aabb.height = 32;
}

void Mark_PlatformShortMoveH::setPosition(Vector2f pos)
{
    float start(pos.x + 16);

    m_startpoint = start;

    if (m_targetpoint > m_startpoint)
    {
        m_startpoint = m_targetpoint;
        m_targetpoint = start;

        m_forward = false;
    }
    else if (m_targetpoint == m_startpoint)
    {
        m_speed = 0;
        m_collider->type = C_IMMOBILE;
    }

    PlatformMov::setPosition(pos);
}

void Mark_PlatformShortMoveH::update()
{
}

void Mark_PlatformShortMoveH::secureUpdate()
{
    // Move :
    if (m_speed > 0)
    {
        if (m_forward)
        {
            if (m_targetpoint < m_aabb.left + 8)
            {
                if (m_movespeed > -m_speed)
                    m_movespeed -= m_speed / 8.0;
            }
            else
                m_forward = false;
        }
        else
        {
            if (m_startpoint > m_aabb.left + 24)
            {
                if (m_movespeed < m_speed)
                    m_movespeed += m_speed / 8.0;
            }
            else
                m_forward = true;
        }

        m_aabb.left += m_movespeed;

        m_sprite.setPosition(roundf(m_aabb.left), m_aabb.top);

        m_collider->rect.left = m_aabb.left;
        m_collider->speed.x = m_movespeed;
    }
}

Vector2f Mark_PlatformShortMoveH::getSize() const
{
    return Vector2f(32, 16);
}

void Mark_PlatformShortMoveH::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
