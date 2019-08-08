/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Miscs/projectile_flame.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif

using namespace sf;

#define FLAME_RAISESPEED 3

Projectile_Flame::Projectile_Flame(Texture* texture, Vector2f position, float speed, float height, bool chasing)
{
    m_sprite.setTexture(*texture);
    m_sprite.setTextureRect(IntRect(0, 0, 53, 30));
    m_sprite.setPosition(position);
    m_sprite.setOrigin(18, 15);

    if (speed > 0)
        m_sprite.setScale(-1, 1);

    m_aabb = FloatRect(position.x - 15, position.y - 15, 30, 30);

    m_movedistance = Vector2f(speed, 0);
    m_targetheight = height;

    m_chasing = chasing;

    m_destroyed = false;
}

void Projectile_Flame::update()
{
    if (frame_Flame % 4 == 0)
        m_sprite.setTextureRect(IntRect((frame_Flame/4) * 53, 0, 53, 30));

    if (m_chasing)
    {
        if (player != NULL)
        {
            if (player->m_aabb.top > m_aabb.top)
            {
                if (m_movedistance.x < 0)
                    m_sprite.rotate(-0.9);
                else
                    m_sprite.rotate(0.9);

                m_movedistance.y += 0.08715;

                m_aabb.top += m_movedistance.y;
            }
            else
            {
                if (m_movedistance.x < 0)
                    m_sprite.rotate(0.9);
                else
                    m_sprite.rotate(-0.9);

                m_movedistance.y -= 0.08715;

                m_aabb.top += m_movedistance.y;
            }
        }
    }
    else
    {
        if (m_aabb.top < m_targetheight - FLAME_RAISESPEED)
            m_aabb.top += FLAME_RAISESPEED;
        else if (m_aabb.top > m_targetheight + FLAME_RAISESPEED)
            m_aabb.top -= FLAME_RAISESPEED;
    }

    m_aabb.left += m_movedistance.x;

    if (m_aabb.left > cameraPos.x + 384 || m_aabb.left < cameraPos.x - 414)
        m_destroyed = true;

    m_sprite.setPosition(m_aabb.left+15, m_aabb.top+15);

    if (player == NULL)
        return;

    if (player->m_fireball[0].m_aabb.left > -64)
    {
        const FloatRect ballAABB(player->m_fireball[0].m_aabb);

        if (ballAABB.intersects(m_aabb))
        {
            listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(ballAABB.left-9, ballAABB.top-7)));
            player->m_fireball[0].m_aabb.left = -64;
        }
    }

    if (player->m_fireball[1].m_aabb.left > -64)
    {
        const FloatRect ballAABB(player->m_fireball[1].m_aabb);

        if (ballAABB.intersects(m_aabb))
        {
            listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(ballAABB.left-9, ballAABB.top-7)));
            player->m_fireball[1].m_aabb.left = -64;
        }
    }

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

void Projectile_Flame::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
