/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_rotodisc.hpp"
#include "../../Headers/globals.hpp"

using namespace std;
using namespace sf;

Hazard_Rotodisc::Hazard_Rotodisc(const Texture& texture, Vector2f disk_pos, float speed)
{
    m_sprite.setTexture(*mainTileset);
    m_sprite.setTextureRect(IntRect(800, 96, 32, 32));

    m_disk.setTexture(texture);

    m_aabb.left = disk_pos.x + 1;
    m_aabb.top = disk_pos.y + 1;
    m_aabb.width = 30;
    m_aabb.height = 30;

    m_speed = fma((speed - 1.0f), 0.0156666f, 0.005f);
}

void Hazard_Rotodisc::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    float dx = m_aabb.left - pos.x - 1;
    float dy = m_aabb.top - pos.y - 1;

    m_radius = sqrt(dx * dx + dy * dy);

    t = atan2((pos.y - m_aabb.top + 1) / -m_radius, (pos.x - m_aabb.left + 1) / m_radius);

    m_position = pos;
}

void Hazard_Rotodisc::update()
{
    m_disk.setTextureRect(IntRect(frame_Rotodisc * 32, 0, 32, 30));

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (player->m_animpowerup > 0)
        return;

    if (game_powerup <= 1 && player->m_animtrace > 0)
        return;

    if (player->m_invincibility > 0)
        return;

    if (player->m_aabb.intersects(m_aabb))
    {
        if (game_powerup == 0)
            player->death();
        else if (game_powerup == 1)
        {
            player->setPower(0, true);

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[6], 0, NULL);
        }
        else
        {
            player->setPower(1, true);

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[6], 0, NULL);
        }
    }
}

void Hazard_Rotodisc::secureUpdate()
{
    m_disk.setPosition(m_aabb.left - 1, m_aabb.top);
}

void Hazard_Rotodisc::afterUpdate()
{
}

Vector2f Hazard_Rotodisc::getPosition() const
{
    return Vector2f(min(m_position.x, m_aabb.left), min(m_position.y, m_aabb.top));
}

Vector2f Hazard_Rotodisc::getSize() const
{
    return Vector2f(abs(m_position.x - m_aabb.left) + 32, abs(m_position.y - m_aabb.top) + 32);
}

ID Hazard_Rotodisc::getID() const
{
    return ID_USELESS;
}
