/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Marks/mark_platformcloudmoveh.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Mark_PlatformCloudMoveH::Mark_PlatformCloudMoveH(const Texture& texture, float target, float speed, Collider* const collision) : PlatformMov(texture, speed, collision)
{
    m_sprite.setTextureRect(IntRect(0, 0, 126, 32));

    m_targetpoint = target + 16;

    m_aabb.width = 126;
    m_aabb.height = 32;
}

void Mark_PlatformCloudMoveH::setPosition(const Vector2f& pos)
{
    float start(pos.x + 63);

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

void Mark_PlatformCloudMoveH::update()
{
    if (frame_Water % 6 == 0)
        m_sprite.setTextureRect(IntRect((frame_Water/6) * 126, 0, 126, 32));
}

void Mark_PlatformCloudMoveH::secureUpdate()
{
    // Move :
    if (m_speed > 0)
    {
        if (m_forward)
        {
            if (m_targetpoint < m_aabb.left + 55)
            {
                if (m_movespeed > -m_speed)
                    m_movespeed -= m_speed / 8.0;
            }
            else
                m_forward = false;
        }
        else
        {
            if (m_startpoint > m_aabb.left + 71)
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

Vector2f Mark_PlatformCloudMoveH::getSize() const
{
    return Vector2f(126, 32);
}

void Mark_PlatformCloudMoveH::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
