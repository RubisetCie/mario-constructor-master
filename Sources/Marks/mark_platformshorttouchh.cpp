/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Marks/mark_platformshorttouchh.hpp"
#include "../../Headers/Marks/mark_mario.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Mark_PlatformShortTouchH::Mark_PlatformShortTouchH(const Texture& texture, float target, float speed, Collider* const collision) : PlatformMov(texture, speed, collision)
{
    m_targetpoint = target;

    m_start = false;

    m_aabb.width = 32;
    m_aabb.height = 32;
}

void Mark_PlatformShortTouchH::setPosition(Vector2f pos)
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

void Mark_PlatformShortTouchH::update()
{
    if (m_start || m_speed == 0)
        return;

    if (player != NULL)
    {
        if (player->m_platform == m_collider)
            m_start = true;
    }
}

void Mark_PlatformShortTouchH::secureUpdate()
{
    // Move :
    if (m_speed > 0 && m_start)
    {
        if (m_forward)
        {
            if (m_targetpoint < m_aabb.left + 16 - m_speed)
            {
                m_aabb.left -= m_speed;
                m_collider->speed.x = -m_speed;
            }
            else
            {
                m_speed = 0;

                m_collider->rect.left = m_targetpoint - 16;
                m_collider->speed.x = 0;
                m_collider->type = C_IMMOBILE;
            }
        }
        else
        {
            if (m_startpoint > m_aabb.left + 16 + m_speed)
            {
                m_aabb.left += m_speed;
                m_collider->speed.x = m_speed;
            }
            else
            {
                m_speed = 0;

                m_collider->rect.left = m_startpoint - 16;
                m_collider->speed.x = 0;
                m_collider->type = C_IMMOBILE;
            }
        }

        m_sprite.setPosition(roundf(m_aabb.left), m_aabb.top);

        m_collider->rect.left = m_aabb.left;
    }
}

Vector2f Mark_PlatformShortTouchH::getSize() const
{
    return Vector2f(32, 16);
}

void Mark_PlatformShortTouchH::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
