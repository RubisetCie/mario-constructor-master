/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_brother.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define BRO_SPEED 1.5
#define BRO_MAXVSPEED 8
#define BRO_JUMPSTRENGHT 8

using namespace std;
using namespace sf;

Sprite_Brother::Sprite_Brother(const Texture& texture, Collider* collision) : Pawn()
{
    m_sprite.setTexture(texture);

    m_movedistance = Vector2f(0, 0);

    m_aabb.width = 32;
    m_aabb.height = 32;

    m_collider = collision;

    m_killed = false;
    m_onfloor = false;
    m_upper = false;
    m_passthrough = false;

    m_frame = 0;
    m_timer = 0;
    m_animspeed = rand() % 6 + 10;

    m_jumppoints[0] = -96;
    m_jumppoints[1] = -96;

    m_alarm[0] = rand() % 40 + 40;
    m_alarm[1] = rand() % 125 + 55;
    m_alarm[2] = rand() % 70 + 30;
    m_alarm[3] = rand() % 40 - 60;
    m_alarm[4] = rand() % 90 + 40;
}

void Sprite_Brother::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    m_limit[0] = pos.x-96;
    m_limit[1] = pos.x+96;

    m_aabb.left = pos.x-16;
    m_aabb.top = pos.y+16;
}

void Sprite_Brother::update()
{
    if (m_jumppoints[0] <= -96)
    {
        unsigned char solid = 0;

        for (int i = 16; i <= 176; i += 32)
        {
            if (zoneb)
            {
                if (testVertical(-i, levelScaleb, collisionMatrixb, &listColliderb))
                {
                    if (solid == 0)
                        solid++;
                }
                else
                {
                    if (solid == 1)
                        solid++;
                    else if (solid == 2)
                    {
                        m_jumppoints[0] = m_aabb.top + 32;
                        m_jumppoints[1] = m_aabb.top - i + 48;

                        goto LBL_BRODETECTION;
                    }
                }
            }
            else
            {
                if (testVertical(-i, levelScale, collisionMatrix, &listCollider))
                {
                    if (solid == 0)
                        solid++;
                }
                else
                {
                    if (solid == 1)
                        solid++;
                    else if (solid == 2)
                    {
                        m_jumppoints[0] = m_aabb.top + 32;
                        m_jumppoints[1] = m_aabb.top - i + 48;

                        goto LBL_BRODETECTION;
                    }
                }
            }
        }

        solid = 0;

        for (int i = 16; i <= 176; i += 32)
        {
            if (zoneb)
            {
                if (testVertical(i, levelScaleb, collisionMatrixb, &listColliderb))
                {
                    if (solid == 2)
                    {
                        m_jumppoints[0] = m_aabb.top + i + 16;
                        m_jumppoints[1] = m_aabb.top + 32;

                        m_upper = true;

                        goto LBL_BRODETECTION;
                    }
                }
                else
                {
                    if (i == 16)
                        goto LBL_BRODETECTION;

                    if (solid < 2)
                        solid++;
                }
            }
            else
            {
                if (testVertical(i, levelScale, collisionMatrix, &listCollider))
                {
                    if (solid == 2)
                    {
                        m_jumppoints[0] = m_aabb.top + i + 16;
                        m_jumppoints[1] = m_aabb.top + 32;

                        m_upper = true;

                        goto LBL_BRODETECTION;
                    }
                }
                else
                {
                    if (i == 16)
                        goto LBL_BRODETECTION;

                    if (solid < 2)
                        solid++;
                }
            }
        }

        LBL_BRODETECTION :

        if (m_jumppoints[0] <= -96)
            m_jumppoints[0] = -64;
    }

    // Horizontal moves :
    if (m_alarm[0] > 0)
        m_alarm[0]--;
    else
    {
        if (m_movedistance.x == 0)
            m_movedistance.x = rand()%2==1?BRO_SPEED:-BRO_SPEED;
        else
        {
            if (rand() % 2 == 0)
                m_movedistance.x = -m_movedistance.x;
            else
                m_movedistance.x = 0;
        }

        m_alarm[0] = rand() % 40 + 40;
    }

    // Jump move :
    if (m_alarm[1] > 0)
        m_alarm[1]--;

    // Throwing moves :
    if (m_alarm[2] > m_alarm[3])
        m_alarm[2]--;
    else
        thruw();

    if (m_movedistance.x != 0)
    {
        if (m_passthrough)
            m_aabb.left += m_movedistance.x;
        else
        {
            if (zoneb)
            {
                if (makeMoveHorizontal(m_movedistance.x, levelScaleb, collisionMatrixb, &listColliderb))
                    m_movedistance.x = 0;
            }
            else
            {
                if (makeMoveHorizontal(m_movedistance.x, levelScale, collisionMatrix, &listCollider))
                    m_movedistance.x = 0;
            }
        }
    }

    if (player == NULL)
        return;

    if (m_aabb.left >= player->m_aabb.left - 2)
        m_sprite.setScale(1, 1);
    else
        m_sprite.setScale(-1, 1);

    if (!player->m_active)
        return;

    if (blockkiller.intersects(m_aabb))
    {
        m_sprite.scale(1, -1);
        m_sprite.move(0, 48);

        m_collider->rect.left = -64;
        m_collider->destroyed = true;

        m_killed = true;
        m_movedistance = Vector2f(0, -3);

        addPoints(100);
        listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-5, m_aabb.top+16)));

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);

        return;
    }

    if (enemystomper.intersects(m_aabb) || enemystomper_pt == m_collider)
    {
        m_sprite.scale(1, -1);
        m_sprite.move(0, 48);

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
            m_sprite.move(0, 48);

            m_collider->rect.left = -64;
            m_collider->destroyed = true;

            m_killed = true;
            m_movedistance = Vector2f(0, -3);

            listEffect.emplace_back(new Effect_Score(effectTxt[13], killcount, Vector2f(aabb.left-10, aabb.top-20)));
            listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left+1, m_aabb.top+1)));

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

void Sprite_Brother::secureUpdate()
{
    if (!m_killed)
    {
        if (enemykiller.intersects(m_aabb))
        {
            m_sprite.scale(1, -1);
            m_sprite.move(0, 48);

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
            dead();
    }
    else
    {
        Matrix* currentMatrix;
        Vector2i* currentScale;
        list<Collider*>* currentList;

        if (m_onfloor)
        {
            if (m_alarm[1] == 0)
            {
                if (m_jumppoints[0] > -64)
                {
                    if (m_upper)
                    {
                        if (rand()%2==0)
                            m_movedistance.y = -BRO_JUMPSTRENGHT;
                        else
                        {
                            m_movedistance.y = -BRO_JUMPSTRENGHT / 3.0f;
                            m_passthrough = true;
                        }
                    }
                    else
                    {
                        m_movedistance.y = -BRO_JUMPSTRENGHT;
                        m_passthrough = true;
                    }
                }
                else
                    m_movedistance.y = -BRO_JUMPSTRENGHT;

                m_alarm[1] = rand() % 125 + 55;
            }
        }
        else
        {
            if (m_movedistance.y < BRO_MAXVSPEED - star_gravity)
                m_movedistance.y += star_gravity;

            if (m_jumppoints[0] > -64)
            {
                if (m_upper)
                {
                    if (m_aabb.top > m_jumppoints[0] - m_movedistance.y - 32)
                    {
                        m_onfloor = true;
                        m_upper = false;
                        m_passthrough = false;

                        m_movedistance.y = 0;

                        m_aabb.top = m_jumppoints[0] - 32;
                    }
                }
                else
                {
                    if (m_aabb.top < m_jumppoints[1] - 32)
                    {
                        m_upper = true;
                        m_passthrough = false;
                    }
                }
            }
        }

        // Collision Events :
        if (zoneb)
        {
            currentMatrix = collisionMatrixb;
            currentScale = &levelScaleb;
            currentList = &listColliderb;
        }
        else
        {
            currentMatrix = collisionMatrix;
            currentScale = &levelScale;
            currentList = &listCollider;
        }

        if (m_movedistance.y != 0)
        {
            if (m_passthrough)
                makeMoveVerticalFake(m_movedistance.y);
            else
            {
                if (makeMoveVertical(m_movedistance.y, *currentScale, currentMatrix, currentList))
                {
                    if (m_movedistance.y > 0)
                        m_onfloor = true;

                    m_movedistance.y = 0;
                }
            }
        }

        if (m_platform != NULL)
        {
            float hspeed(m_platform->speed.x);

            makeMoveHorizontalPlatform(hspeed, *currentScale, currentMatrix, currentList);
            makeMoveVerticalPlatform(m_platform->speed.y, *currentScale, currentMatrix, currentList);

            m_limit[0] += hspeed;
            m_limit[1] += hspeed;
        }

        if (m_onfloor)
        {
            if (!testVertical(1, *currentScale, currentMatrix, currentList))
            {
                m_onfloor = false;
                m_platform = NULL;
            }
        }

        if (m_aabb.left < m_limit[0])
        {
            m_aabb.left = m_limit[0];
            m_movedistance.x = 0;
        }
        else if (m_aabb.left > m_limit[1])
        {
            m_aabb.left = m_limit[1];
            m_movedistance.x = 0;
        }

        m_sprite.setPosition(roundf(m_aabb.left+16), roundf(m_aabb.top-16));

        m_collider->rect.left = m_aabb.left;
        m_collider->rect.top = m_aabb.top;
    }

    if (zoneb)
    {
        if (m_aabb.top > levelScaleb.y * 480 + 64)
        {
            if (m_collider != NULL)
            {
                m_collider->destroyed = true;
                m_collider = NULL;
            }

            dead();
        }
    }
    else
    {
        if (m_aabb.top > levelScale.y * 480 + 64)
        {
            if (m_collider != NULL)
            {
                m_collider->destroyed = true;
                m_collider = NULL;
            }

            dead();
        }
    }
}

ID Sprite_Brother::getID() const
{
    if (!m_killed)
        return ID_STOMPABLE;
    else
        return ID_USELESS;
}
