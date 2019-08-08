/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_cheepcheepblue.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define CHEEPCHEEP_MAXVSPEED 8

using namespace std;
using namespace sf;

Sprite_CheepCheepBlue::Sprite_CheepCheepBlue(const Texture& texture, Collider* collision) : Pawn()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 31, 32));
    m_sprite.setOrigin(16, 0);

    m_movedistance = Vector2f(0, 0);

    m_aabb.width = 31;
    m_aabb.height = 32;

    m_collider = collision;

    m_active = false;
    m_swimming = false;
    m_killed = false;
}

void Sprite_CheepCheepBlue::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x-15;
    m_aabb.top = pos.y;
}

void Sprite_CheepCheepBlue::update()
{
    m_active = true;

    if (m_killed)
        return;

    if (frame_Flower % 8 == 0)
        m_sprite.setTextureRect(IntRect((frame_Flower/8) * 31, 0, 31, 32));

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if ((enemystomper.intersects(m_aabb) || enemystomper_pt == m_collider) && !m_swimming)
    {
        m_sprite.scale(1, -1);
        m_sprite.move(0, 32);

        m_collider->rect.left = -64;
        m_collider->destroyed = true;

        m_killed = true;
        m_movedistance = Vector2f(0, 0);

        addPoints(100);
        listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-5, m_aabb.top+16)));

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(8), sfxSamples[11], 0, NULL);

        return;
    }

    if (player->m_animpowerup > 0)
        return;

    if (game_powerup <= 1 && player->m_animtrace > 0)
        return;

    FloatRect const aabb = player->m_aabb;
    if (aabb.intersects(m_aabb))
    {
        if (player->m_invincibility > 0)
        {
            m_sprite.scale(1, -1);
            m_sprite.move(0, 32);

            m_collider->rect.left = -64;
            m_collider->destroyed = true;

            m_killed = true;
            m_movedistance = Vector2f(0, -3);

            listEffect.emplace_back(new Effect_Score(effectTxt[13], killcount, Vector2f(aabb.left-10, aabb.top-20)));
            listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left, m_aabb.top+1)));

            switch (killcount)
            {
                case 0 : addPoints(100); killcount++; FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL); break;
                case 1 : addPoints(200); killcount++; FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL); break;
                case 2 : addPoints(500); killcount++; FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL); break;
                case 3 : addPoints(1000); killcount++; FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL); break;
                case 4 : addPoints(2000); killcount++; FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL); break;
                case 5 : addPoints(5000); killcount++; FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL); break;
                case 6 : addPoints(10000); killcount++; FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL); break;
                case 7 : addLife(); killcount=0; FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[7], 0, NULL); break;
            }
        }
        else
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

void Sprite_CheepCheepBlue::secureUpdate()
{
    if (!m_active)
        return;

    if (!m_killed)
    {
        if (m_movedistance.x == 0 && player != NULL)
        {
            if (zoneb)
            {
                if (waterb != NULL)
                {
                    if (!waterb->getType())
                    {
                        float height = waterb->getHeight();
                        if (m_aabb.top + 32 > height)
                            m_swimming = true;
                    }
                }
            }
            else
            {
                if (watera != NULL)
                {
                    if (!watera->getType())
                    {
                        float height = watera->getHeight();
                        if (m_aabb.top + 32 > height)
                            m_swimming = true;
                    }
                }
            }

            if (m_swimming)
            {
                float playerpos = player->getPosition().x;
                float pos = m_sprite.getPosition().x;

                if (pos>= playerpos+14)
                    m_movedistance.x = -1.4;
                else
                {
                    m_movedistance.x = 1.4;

                    m_sprite.setScale(-1, 1);
                }

                m_collider->type = C_UNSTOMPABLE;
            }
            else
            {
                float playerpos = player->getPosition().x;
                Vector2f pos(m_sprite.getPosition());

                m_aabb.top = cameraPos.y + 240;

                float speed(sqrt((m_aabb.top - pos.y) * 0.3615));

                if (speed > 12.0f)
                    speed = 12.0f;

                if (pos.x >= playerpos+14)
                {
                    m_aabb.left = cameraPos.x + 320;

                    m_movedistance = Vector2f((rand() / (RAND_MAX / 5.0f)) - 8.0f, -speed);
                }
                else
                {
                    m_aabb.left = cameraPos.x - 352;

                    m_movedistance = Vector2f((rand() / (RAND_MAX / 5.0f)) + 3.0f, -speed);

                    m_sprite.setScale(-1, 1);
                }
            }
        }

        if (m_movedistance.x < 0)
            m_sprite.setScale(1, 1);
        else
            m_sprite.setScale(-1, 1);

        if (enemykiller.intersects(m_aabb) && enemykiller_fire == 1)
        {
            m_sprite.scale(1, -1);
            m_sprite.move(0, 32);

            m_collider->rect.left = -64;
            m_collider->destroyed = true;

            m_killed = true;
            m_movedistance = Vector2f(0, -3);

            addPoints(100);
            listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-5, m_aabb.top+16)));

            if (m_aabb.left > cameraPos.x - 384 && m_aabb.left < cameraPos.x + 352 && m_aabb.top > cameraPos.y - 304 && m_aabb.top < cameraPos.y + 272)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);
        }
    }

    if (m_killed)
    {
        m_sprite.move(0, m_movedistance.y);
        m_movedistance.y += 0.11;

        float dist = m_sprite.getPosition().y;

        if (dist > m_aabb.top + 512)
            m_destroyed = true;
    }
    else
    {
        if (m_swimming)
        {
            if (zoneb)
            {
                if (makeMoveHorizontal(m_movedistance.x, levelScaleb, collisionMatrixb, &listColliderb))
                    m_movedistance.x = -m_movedistance.x;
            }
            else
            {
                if (makeMoveHorizontal(m_movedistance.x, levelScale, collisionMatrix, &listCollider))
                    m_movedistance.x = -m_movedistance.x;
            }
        }
        else
        {
            if (m_movedistance.y < CHEEPCHEEP_MAXVSPEED - star_gravity)
                m_movedistance.y += star_gravity;

            m_aabb.left += m_movedistance.x;
            makeMoveVerticalFake(m_movedistance.y);
        }

        m_sprite.setPosition(roundf(m_aabb.left + 15), roundf(m_aabb.top));

        m_collider->rect.left = m_aabb.left;
        m_collider->rect.top = m_aabb.top;
    }

    if (zoneb)
    {
        if (m_aabb.top > levelScaleb.y * 480 + 64)
        {
            m_collider->destroyed = true;
            m_destroyed = true;
        }
    }
    else
    {
        if (m_aabb.top > levelScale.y * 480 + 64)
        {
            m_collider->destroyed = true;
            m_destroyed = true;
        }
    }
}

void Sprite_CheepCheepBlue::afterUpdate()
{
}

Vector2f Sprite_CheepCheepBlue::getPosition() const
{
    return Vector2f(m_aabb.left, m_aabb.top);
}

Vector2f Sprite_CheepCheepBlue::getSize() const
{
    return Vector2f(31, 32);
}

ID Sprite_CheepCheepBlue::getID() const
{
    return ID_USELESS;
}

void Sprite_CheepCheepBlue::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

