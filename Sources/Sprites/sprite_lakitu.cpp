/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_lakitu.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define LAKITU_MAXENEMIES 50

using namespace std;
using namespace sf;

Sprite_Lakitu::Sprite_Lakitu(const Texture& texture, Collider* collision) : Pawn()
{
    m_array.setPrimitiveType(Quads);

    m_texture = &texture;

    m_movedistance = Vector2f(0, 0);

    m_aabb.width = 32;
    m_aabb.height = 32;

    m_collider = collision;

    m_alarm[0] = -1;
    m_alarm[1] = rand() % 60 + 45;
    m_alarm[2] = rand() % 50 - 120;

    m_enemies = LAKITU_MAXENEMIES;

    m_targetspeed = 0;

    m_active = false;
    m_killed = false;
}

void Sprite_Lakitu::setPosition(Vector2f pos)
{
    m_array.append(Vertex(pos, Color::White, Vector2f(0, 0)));
    m_array.append(Vertex(Vector2f(pos.x + 31, pos.y), Color::White, Vector2f(31, 0)));
    m_array.append(Vertex(Vector2f(pos.x + 31, pos.y + 32), Color::White, Vector2f(31, 32)));
    m_array.append(Vertex(Vector2f(pos.x, pos.y + 32), Color::White, Vector2f(0, 32)));

    m_array.append(Vertex(Vector2f(pos.x + 3, pos.y - 16), Color::White, Vector2f(31, 0)));
    m_array.append(Vertex(Vector2f(pos.x + 27, pos.y - 16), Color::White, Vector2f(55, 0)));
    m_array.append(Vertex(Vector2f(pos.x + 27, pos.y), Color::White, Vector2f(55, 16)));
    m_array.append(Vertex(Vector2f(pos.x + 3, pos.y), Color::White, Vector2f(31, 16)));

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;
}

void Sprite_Lakitu::update()
{
    m_active = true;

    if (m_killed)
        return;

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (enemystomper.intersects(m_aabb) || enemystomper_pt == m_collider)
    {
        m_array[0].texCoords.y = 32;
        m_array[1].texCoords.y = 32;
        m_array[2].texCoords.y = 0;
        m_array[3].texCoords.y = 0;

        m_array[4].position.y = m_aabb.top + 48;
        m_array[5].position.y = m_aabb.top + 48;
        m_array[6].position.y = m_aabb.top + 32;
        m_array[7].position.y = m_aabb.top + 32;

        m_array[4].texCoords.y = 0;
        m_array[5].texCoords.y = 0;
        m_array[6].texCoords.y = 16;
        m_array[7].texCoords.y = 16;

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

    if (player->m_invincibility == 0)
        return;

    FloatRect const aabb = player->m_aabb;
    if (aabb.intersects(m_aabb))
    {
        m_array[0].texCoords.y = 32;
        m_array[1].texCoords.y = 32;
        m_array[2].texCoords.y = 0;
        m_array[3].texCoords.y = 0;

        m_array[4].position.y = m_aabb.top + 48;
        m_array[5].position.y = m_aabb.top + 48;
        m_array[6].position.y = m_aabb.top + 32;
        m_array[7].position.y = m_aabb.top + 32;

        m_array[4].texCoords.y = 0;
        m_array[5].texCoords.y = 0;
        m_array[6].texCoords.y = 16;
        m_array[7].texCoords.y = 16;

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
}

void Sprite_Lakitu::secureUpdate()
{
    if (!m_active)
        return;

    if (!m_killed)
    {
        if (enemykiller.intersects(m_aabb) && enemykiller_fire != 2)
        {
            m_array[0].texCoords.y = 32;
            m_array[1].texCoords.y = 32;
            m_array[2].texCoords.y = 0;
            m_array[3].texCoords.y = 0;

            m_array[4].position.y = m_aabb.top + 48;
            m_array[5].position.y = m_aabb.top + 48;
            m_array[6].position.y = m_aabb.top + 32;
            m_array[7].position.y = m_aabb.top + 32;

            m_array[4].texCoords.y = 0;
            m_array[5].texCoords.y = 0;
            m_array[6].texCoords.y = 16;
            m_array[7].texCoords.y = 16;

            m_collider->rect.left = -64;
            m_collider->destroyed = true;

            m_killed = true;
            m_movedistance = Vector2f(0, -3);

            addPoints(100);
            listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-5, m_aabb.top+16)));

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);
        }
    }

    if (m_killed)
    {
        for (register unsigned char i = 0; i < 8; i++)
            m_array[i].position.y += m_movedistance.y;

        m_movedistance.y += 0.11;

        float dist = m_array[0].position.y;

        if (dist > m_aabb.top + 512)
            m_destroyed = true;
    }
    else
    {
        // Horizontal speed :
        if (m_alarm[0] > 0)
            m_alarm[0]--;
        else
        {
            if (player != NULL)
            {
                float delta = m_aabb.left - player->m_aabb.left - 2;

                if (delta < 0)
                {
                    if (delta < -128 && m_alarm[0] == 0)
                        m_targetspeed = 5;
                    else
                        m_targetspeed = 2;
                }
                else
                {
                    if (delta > 128 && m_alarm[0] == 0)
                        m_targetspeed = -5;
                    else
                        m_targetspeed = -2;
                }
            }

            m_alarm[0] = 60;
        }

        // Throw action :
        if (m_alarm[1] > m_alarm[2])
            m_alarm[1]--;
        else
        {
            if (m_enemies > 0)
                thruw();
        }

        if (m_movedistance.x < m_targetspeed)
            m_movedistance.x += 0.12;
        else if (m_movedistance.x > m_targetspeed)
            m_movedistance.x -= 0.12;

        m_aabb.left += m_movedistance.x;
        m_aabb.top += m_movedistance.y;

        for (register unsigned char i = 0; i < 4; i++)
            m_array[i].position.x += m_movedistance.x;

        if (m_enemies > 0)
        {
            if (m_alarm[1] <= m_alarm[2] + 16)
            {
                if (m_array[4].position.y > m_aabb.top - 16)
                {
                    m_array[4].position.y -= 1;
                    m_array[5].position.y -= 1;

                    m_array[6].texCoords.y += 1;
                    m_array[7].texCoords.y += 1;
                }
                else
                {
                    m_array[4].position.y = m_aabb.top - 16;
                    m_array[5].position.y = m_aabb.top - 16;

                    m_array[6].texCoords.y = 16;
                    m_array[7].texCoords.y = 16;
                }
            }
            else if (m_alarm[1] <= 0)
            {
                if (m_array[4].position.y < m_aabb.top)
                {
                    m_array[4].position.y += 1;
                    m_array[5].position.y += 1;

                    m_array[6].texCoords.y -= 1;
                    m_array[7].texCoords.y -= 1;
                }
                else
                {
                    m_array[4].position.y = m_aabb.top;
                    m_array[5].position.y = m_aabb.top;

                    m_array[6].texCoords.y = 0;
                    m_array[7].texCoords.y = 0;
                }
            }
        }

        if (player != NULL)
        {
            float rnd = roundf(m_aabb.left);

            if (m_aabb.left >= player->m_aabb.left - 2)
            {
                m_array[0].texCoords.x = 31;
                m_array[1].texCoords.x = 0;
                m_array[2].texCoords.x = 0;
                m_array[3].texCoords.x = 31;

                m_array[4].position.x = rnd + 28;
                m_array[5].position.x = rnd + 4;
                m_array[6].position.x = rnd + 4;
                m_array[7].position.x = rnd + 28;
            }
            else
            {
                m_array[0].texCoords.x = 0;
                m_array[1].texCoords.x = 31;
                m_array[2].texCoords.x = 31;
                m_array[3].texCoords.x = 0;

                m_array[4].position.x = rnd + 3;
                m_array[5].position.x = rnd + 27;
                m_array[6].position.x = rnd + 27;
                m_array[7].position.x = rnd + 3;
            }
        }

        m_collider->rect.left = m_aabb.left;
        m_collider->rect.top = m_aabb.top;
    }
}

void Sprite_Lakitu::afterUpdate()
{
}

Vector2f Sprite_Lakitu::getPosition() const
{
    return Vector2f(m_aabb.left, m_aabb.top-16);
}

Vector2f Sprite_Lakitu::getSize() const
{
    return Vector2f(31, 48);
}

ID Sprite_Lakitu::getID() const
{
    return ID_USELESS;
}
