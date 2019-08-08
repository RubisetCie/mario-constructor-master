/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Marks/mark_platformcloudtouchh.hpp"
#include "../../Headers/Marks/mark_mario.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Mark_PlatformCloudTouchH::Mark_PlatformCloudTouchH(const Texture& texture, float target, float speed, Collider* const collision) : PlatformMov(texture, speed, collision)
{
    m_sprite.setTextureRect(IntRect(0, 0, 126, 32));

    m_targetpoint = target;

    m_start = false;

    m_aabb.width = 126;
    m_aabb.height = 32;
}

void Mark_PlatformCloudTouchH::setPosition(Vector2f pos)
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

void Mark_PlatformCloudTouchH::update()
{
    if (frame_Water % 6 == 0)
        m_sprite.setTextureRect(IntRect((frame_Water/6) * 126, 0, 126, 32));

    if (m_start || m_speed == 0)
        return;

    if (player != NULL)
    {
        if (player->m_platform == m_collider)
            m_start = true;
    }
}

void Mark_PlatformCloudTouchH::secureUpdate()
{
    // Move :
    if (m_speed > 0 && m_start)
    {
        if (m_forward)
        {
            if (m_targetpoint < m_aabb.left + 63 - m_speed)
            {
                m_aabb.left -= m_speed;
                m_collider->speed.x = -m_speed;
            }
            else
            {
                m_speed = 0;

                m_collider->rect.left = m_targetpoint - 63;
                m_collider->speed.x = 0;
                m_collider->type = C_IMMOBILE;
            }
        }
        else
        {
            if (m_startpoint > m_aabb.left + 63 + m_speed)
            {
                m_aabb.left += m_speed;
                m_collider->speed.x = m_speed;
            }
            else
            {
                m_speed = 0;

                m_collider->rect.left = m_startpoint - 63;
                m_collider->speed.x = 0;
                m_collider->type = C_IMMOBILE;
            }
        }

        m_sprite.setPosition(roundf(m_aabb.left), m_aabb.top);

        m_collider->rect.left = m_aabb.left;
    }
}

Vector2f Mark_PlatformCloudTouchH::getSize() const
{
    return Vector2f(126, 32);
}

void Mark_PlatformCloudTouchH::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
