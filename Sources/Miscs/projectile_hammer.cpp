/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Miscs/projectile_hammer.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif

#define HAMMER_MAXVSPEED 12

using namespace sf;

Projectile_Hammer::Projectile_Hammer(Texture* texture, Vector2f position, Vector2f speed)
{
    m_sprite.setTexture(*texture);
    m_sprite.setPosition(position);
    m_sprite.setOrigin(12, 16);

    m_aabb = FloatRect(position.x - 12, position.y - 12, 24, 24);

    m_movedistance = speed;

    m_destroyed = false;
}

void Projectile_Hammer::update()
{
    if (m_movedistance.y < HAMMER_MAXVSPEED - star_gravity)
        m_movedistance.y += star_gravity;

    m_sprite.rotate(m_movedistance.x * 3);

    m_aabb.left += m_movedistance.x;
    m_aabb.top += m_movedistance.y;

    if (m_aabb.top > cameraPos.y + 272)
        m_destroyed = true;

    m_sprite.setPosition(m_aabb.left+12, m_aabb.top+12);

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

void Projectile_Hammer::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
