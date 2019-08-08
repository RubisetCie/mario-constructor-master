/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Miscs/projectile_plantfireball.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif

#define PLANTBALL_MAXVSPEED 12
#define PLANTBALL_ROTATION 20

using namespace sf;

Projectile_PlantFireball::Projectile_PlantFireball(Texture* texture)
{
    m_sprite.setTexture(*texture);
    m_sprite.setPosition(-64, -64);
    m_sprite.setOrigin(6, 8);

    m_aabb = FloatRect(-64, -64, 15, 16);

    m_movedistance = Vector2f(0, 0);
}

void Projectile_PlantFireball::update()
{
    if (m_movedistance.y < PLANTBALL_MAXVSPEED - star_gravity)
        m_movedistance.y += star_gravity;

    if (m_movedistance.x > 0)
        m_sprite.rotate(PLANTBALL_ROTATION);
    else
        m_sprite.rotate(-PLANTBALL_ROTATION);

    m_aabb.left += m_movedistance.x;
    m_aabb.top += m_movedistance.y;

    if (m_aabb.top > cameraPos.y + 336)
        m_aabb.left = -64;

    m_sprite.setPosition(m_aabb.left+6, m_aabb.top+8);

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

void Projectile_PlantFireball::cast(Vector2f position, Vector2f speed)
{
    m_sprite.setPosition(position);

    m_aabb.left = position.x - 6;
    m_aabb.top = position.y - 8;

    m_movedistance = speed;
}

void Projectile_PlantFireball::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
