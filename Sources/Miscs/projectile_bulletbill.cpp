/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Miscs/projectile_bulletbill.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif

#define BULLETBILL_MAXSPEED 3.5
#define BULLETBILL_ACCELERATION 0.15

using namespace sf;

Projectile_Bulletbill::Projectile_Bulletbill(Texture* texture, Vector2f position, Vector2f speed, bool inverted, bool chasing)
{
    m_sprite.setTexture(*texture);
    m_sprite.setTextureRect(IntRect(0, 0, 34, 28));
    m_sprite.setPosition(position);
    m_sprite.setOrigin(17, 14);

    if (speed.x > 0)
        m_sprite.setScale(-1, 1);

    if (speed.y != 0)
    {
        m_sprite.setRotation(90);

        if (speed.y > 0)
            m_sprite.setScale(-1, inverted?-1:1);
    }

    m_aabb = FloatRect(position.x - 14, position.y - 14, 28, 28);

    m_movedistance = speed;

    m_chasing = chasing;
    m_killed = false;
    m_destroyed = false;
}

void Projectile_Bulletbill::update()
{
    if (!m_killed)
    {
        if (m_chasing)
        {
            if (m_frame < 11)
                m_frame++;
            else
                m_frame = 0;

            if (m_frame % 4 == 0)
                m_sprite.setTextureRect(IntRect(0, (m_frame / 3) * 28, 34, 28));

            if (player != NULL)
            {
                if (player->m_active)
                {
                    if (m_sprite.getRotation() == 0)
                    {
                        if (player->m_aabb.left < m_aabb.left)
                        {
                            if (m_movedistance.x > -BULLETBILL_MAXSPEED + BULLETBILL_ACCELERATION)
                                m_movedistance.x -= BULLETBILL_ACCELERATION;

                            if (m_movedistance.x < 0)
                                m_sprite.setScale(1, 1);
                        }
                        else
                        {
                            if (m_movedistance.x < BULLETBILL_MAXSPEED - BULLETBILL_ACCELERATION)
                                m_movedistance.x += BULLETBILL_ACCELERATION;

                            if (m_movedistance.x > 0)
                                m_sprite.setScale(-1, 1);
                        }
                    }
                    else
                    {
                        if (player->m_aabb.top < m_aabb.top)
                        {
                            if (m_movedistance.y > -BULLETBILL_MAXSPEED + BULLETBILL_ACCELERATION)
                                m_movedistance.y -= BULLETBILL_ACCELERATION;

                            if (m_movedistance.y < 0)
                                m_sprite.setScale(1, 1);
                        }
                        else
                        {
                            if (m_movedistance.y < BULLETBILL_MAXSPEED - BULLETBILL_ACCELERATION)
                                m_movedistance.y += BULLETBILL_ACCELERATION;

                            if (m_movedistance.y > 0)
                                m_sprite.setScale(-1, 1);
                        }
                    }
                }
            }
        }

        m_aabb.left += m_movedistance.x;
        m_aabb.top += m_movedistance.y;

        if (m_aabb.left > cameraPos.x + 384 || m_aabb.left < cameraPos.x - 416 || m_aabb.top > cameraPos.y + 304 || m_aabb.top < cameraPos.y - 332)
            m_destroyed = true;

        m_sprite.setPosition(m_aabb.left+14, m_aabb.top+14);

        if (player == NULL)
            goto LBL_BULLETNONLETHAL;

        if (!player->m_active)
            goto LBL_BULLETNONLETHAL;

        if (player->m_invincibility > 0)
            goto LBL_BULLETNONLETHAL;

        if (player->m_aabb.intersects(m_aabb))
        {
            if (player->m_aabb.top + player->m_aabb.height < m_aabb.top + 16 && player->m_movedistance.y > 0)
            {
                if (m_sprite.getRotation() == 0)
                    m_sprite.scale(1, -1);
                else
                    m_sprite.scale(-1, 1);

                m_sprite.setTextureRect(IntRect(0, 0, 34, 28));

                player->bounce(13);

                m_killed = true;
                m_movedistance.x /= 2;
                m_movedistance.y = 0;

                addPoints(100);
                listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-5, m_aabb.top+16)));

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(8), sfxSamples[11], 0, NULL);
            }
            else
            {
                if (player->m_animpowerup > 0)
                    goto LBL_BULLETNONLETHAL;

                if (game_powerup <= 1 && player->m_animtrace > 0)
                    goto LBL_BULLETNONLETHAL;

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

    LBL_BULLETNONLETHAL :

    if (m_killed)
    {
        m_sprite.move(m_movedistance);
        m_movedistance.y += 0.11;

        float dist = m_sprite.getPosition().y;

        if (dist > m_aabb.top + 512)
            m_destroyed = true;
    }
}

void Projectile_Bulletbill::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
