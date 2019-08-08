/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Marks/mark_platformredmoveh.hpp"

#ifdef DEBUGMODE
#include <iostream>
using namespace std;
#endif // DEBUGMODE

using namespace sf;

Mark_PlatformRedMoveH::Mark_PlatformRedMoveH(const Texture& texture, float target, float speed, Collider* const collision) : PlatformMov(texture, speed, collision)
{
    m_targetpoint = target + 16;

    m_aabb.width = 96;
    m_aabb.height = 32;
}

void Mark_PlatformRedMoveH::setPosition(Vector2f pos)
{
    float start(pos.x + 48);

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

void Mark_PlatformRedMoveH::update()
{
}

void Mark_PlatformRedMoveH::secureUpdate()
{
    // Move :
    if (m_speed > 0)
    {
        if (m_forward)
        {
            if (m_targetpoint < m_aabb.left + 40)
            {
                if (m_movespeed > -m_speed)
                    m_movespeed -= m_speed / 8.0;
            }
            else
                m_forward = false;
        }
        else
        {
            if (m_startpoint > m_aabb.left + 56)
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

Vector2f Mark_PlatformRedMoveH::getSize() const
{
    return Vector2f(96, 16);
}

void Mark_PlatformRedMoveH::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
