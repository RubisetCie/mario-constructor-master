/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/globals.hpp"
#include "../../Headers/Miscs/projectile_podoboo.hpp"

#define PODOBOO_MAXVSPEED 8

using namespace sf;

Projectile_Podoboo::Projectile_Podoboo(Texture* texture, Vector2f position, Vector2f speed, Vector2f size, float angle)
{
    m_sprite.setTexture(*texture);
    m_sprite.setTextureRect(IntRect(0, 0, 27, 32));
    m_sprite.setOrigin(13, 16);
    m_sprite.setScale(size);
    m_sprite.setRotation(angle);

    m_movedistance = speed;

    m_aabb.left = position.x - 13;
    m_aabb.top = position.y - 13;
    m_aabb.width = 27;
    m_aabb.height = 27;

    m_destroyed = false;
}

void Projectile_Podoboo::update()
{
    if (frame_Podoboo % 2 == 0)
        m_sprite.setTextureRect(IntRect((frame_Podoboo/2) * 27, 0, 27, 32));

    if (m_movedistance.y < PODOBOO_MAXVSPEED - star_gravity)
        m_movedistance.y += star_gravity;

    if (m_sprite.getRotation() == 0)
    {
        if (m_movedistance.y > 0)
            m_sprite.setScale(1, -1);
    }
    else
    {
        if (m_movedistance.y > 3.5)
        {
            m_sprite.setRotation(0);
            m_sprite.setScale(1, -1);
        }
    }

    m_aabb.left += m_movedistance.x;
    m_aabb.top += m_movedistance.y;

    if (m_aabb.top > cameraPos.y + 400)
        m_destroyed = true;

    m_sprite.setPosition(m_aabb.left + 13, m_aabb.top + 13);

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

void Projectile_Podoboo::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
