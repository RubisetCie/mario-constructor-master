/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Marks/mark_platformshorttouchv.hpp"
#include "../../Headers/Marks/mark_mario.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Mark_PlatformShortTouchV::Mark_PlatformShortTouchV(const Texture& texture, float target, float speed, Collider* const collision) : PlatformMov(texture, speed, collision)
{
    m_targetpoint = target;

    m_start = false;

    m_aabb.width = 32;
    m_aabb.height = 32;
}

void Mark_PlatformShortTouchV::setPosition(const Vector2f& pos)
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

void Mark_PlatformShortTouchV::update()
{
    if (m_start || m_speed == 0)
        return;

    if (player != NULL)
    {
        if (player->m_platform == m_collider)
            m_start = true;
    }
}

void Mark_PlatformShortTouchV::secureUpdate()
{
    // Move :
    if (m_speed > 0 && m_start)
    {
        if (m_forward)
        {
            if (m_targetpoint < m_aabb.top - m_speed)
            {
                m_aabb.top -= m_speed;
                m_collider->speed.y = -m_speed;
            }
            else
            {
                m_speed = 0;

                m_collider->rect.top = m_targetpoint;
                m_collider->speed.y = 0;
                m_collider->type = C_IMMOBILE;
            }
        }
        else
        {
            if (m_startpoint > m_aabb.top + m_speed)
            {
                m_aabb.top += m_speed;
                m_collider->speed.y = m_speed;
            }
            else
            {
                m_speed = 0;

                m_collider->rect.top = m_startpoint;
                m_collider->speed.y = 0;
                m_collider->type = C_IMMOBILE;
            }
        }

        m_sprite.setPosition(m_aabb.left, roundf(m_aabb.top));

        m_collider->rect.top = m_aabb.top;
    }
}

Vector2f Mark_PlatformShortTouchV::getSize() const
{
    return Vector2f(32, 16);
}

void Mark_PlatformShortTouchV::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
