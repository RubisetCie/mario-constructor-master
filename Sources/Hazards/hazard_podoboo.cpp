/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Hazards/hazard_podoboo.hpp"
#include "../../Headers/globals.hpp"

#define PODOBOO_MAXVSPEED 12

using namespace sf;

Hazard_Podoboo::Hazard_Podoboo(const Texture& texture, float maxheight) : Placeable()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 27, 32));

    m_movedistance = 0;
    m_maxheight = maxheight;

    m_timer = 50;

    m_speed = PODOBOO_MAXVSPEED;
}

void Hazard_Podoboo::update()
{
    if (frame_Podoboo % 2 == 0)
        m_sprite.setTextureRect(IntRect((frame_Podoboo/2) * 27, 0, 27, 32));

    if (m_timer == 0)
        m_timer = 70;

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

    if (m_timer >= 68 && m_timer <= 70)
    {
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
}

void Hazard_Podoboo::afterUpdate()
{
}

Vector2f Hazard_Podoboo::getPosition() const
{
    return Vector2f(m_aabb.left, m_aabb.top);
}

ID Hazard_Podoboo::getID() const
{
    return ID_USELESS;
}
