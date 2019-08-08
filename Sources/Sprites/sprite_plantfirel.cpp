/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Sprites/sprite_plantfirel.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_PlantFireL::Sprite_PlantFireL(Texture* texture, Collider* collision) : Sprite_PlantL(texture, collision), m_fire {(itemsTxt[9]), (itemsTxt[9]), (itemsTxt[9])}
{
    m_timer = 340;
    m_killed = false;
}

void Sprite_PlantFireL::update()
{
    if (m_killed)
        return;

    if (player != NULL)
    {
        if (!(player->m_aabb.left > m_aabb.left - 33 && player->m_aabb.left < m_aabb.left - 26 && player->m_aabb.top > m_aabb.top - 32 && player->m_aabb.top < m_aabb.top + 32))
        {
            if (m_timer == 340)
                m_timer = 0;
        }
    }
    else
    {
        if (m_timer == 340)
            m_timer = 0;
    }

    Sprite_PlantL::update();
}

void Sprite_PlantFireL::secureUpdate()
{
    if (!m_killed)
    {
        if (m_timer < 340)
        {
            m_timer++;

            if (m_timer >= 40 && m_timer < 100)
            {
                m_array[0].position.x -= 0.7833333;
                m_array[3].position.x -= 0.7833333;

                m_array[1].texCoords.y += 0.7833333;
                m_array[2].texCoords.y += 0.7833333;

                m_aabb.left -= 0.7833333;
                m_aabb.width += 0.7833333;

                m_collider->rect.left -= 0.7833333;
                m_collider->rect.width += 0.7833333;
            }

            if (m_timer >= 240 && m_timer < 300)
            {
                m_array[0].position.x += 0.7833333;
                m_array[3].position.x += 0.7833333;

                m_array[1].texCoords.y -= 0.7833333;
                m_array[2].texCoords.y -= 0.7833333;

                m_aabb.left += 0.7833333;
                m_aabb.width -= 0.7833333;

                m_collider->rect.left += 0.7833333;
                m_collider->rect.width -= 0.7833333;
            }

            if (m_timer == 100)
            {
                m_array[0].position.x = m_startwidth;
                m_array[3].position.x = m_startwidth;

                m_array[1].texCoords.y = 47;
                m_array[2].texCoords.y = 47;

                m_aabb.left = m_startwidth;
                m_aabb.width = 47;

                m_collider->rect.left = m_startwidth;
                m_collider->rect.width = 47;
            }
            else if (m_timer == 130)
            {
                m_fire[0].cast(Vector2f(m_aabb.left + 7, m_aabb.top + 15), Vector2f((rand() / static_cast<float>(RAND_MAX)) - 5.5f, (rand() / (RAND_MAX / 2.0f)) - 1.0f));

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
            }
            else if (m_timer == 140)
            {
                m_fire[1].cast(Vector2f(m_aabb.left + 7, m_aabb.top + 15), Vector2f((rand() / static_cast<float>(RAND_MAX)) - 5.5f, (rand() / (RAND_MAX / 2.0f)) - 1.0f));

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
            }
            else if (m_timer == 150)
            {
                m_fire[2].cast(Vector2f(m_aabb.left + 7, m_aabb.top + 15), Vector2f((rand() / static_cast<float>(RAND_MAX)) - 5.5f, (rand() / (RAND_MAX / 2.0f)) - 1.0f));

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
            }
            else if (m_timer == 300)
            {
                m_array[0].position.x = m_startwidth + 47;
                m_array[3].position.x = m_startwidth + 47;

                m_array[1].texCoords.y = 0;
                m_array[2].texCoords.y = 0;

                m_aabb.left = m_startwidth + 47;
                m_aabb.width = 0;

                m_collider->rect.left = m_startwidth + 47;
                m_collider->rect.width = 0;
            }
        }

        if (m_aabb.width > 0)
        {
            if (enemykiller.intersects(m_aabb) && enemykiller_fire != 2)
            {
                m_killed = true;

                m_collider->rect.left = -64;
                m_collider->destroyed = true;

                addPoints(100);
                listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-5, m_aabb.top+16)));

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);
            }
        }
    }

    if (m_fire[0].m_aabb.left > -64)
        m_fire[0].update();

    if (m_fire[1].m_aabb.left > -64)
        m_fire[1].update();

    if (m_fire[2].m_aabb.left > -64)
        m_fire[2].update();

    if (m_killed)
    {
        if (m_fire[0].m_aabb.left <= -64 && m_fire[1].m_aabb.left <= -64 && m_fire[2].m_aabb.left <= -64)
            m_destroyed = true;
    }
}

void Sprite_PlantFireL::afterUpdate()
{
    if (m_fire[0].m_aabb.left > -64)
        mainTexture.draw(m_fire[0]);

    if (m_fire[1].m_aabb.left > -64)
        mainTexture.draw(m_fire[1]);

    if (m_fire[2].m_aabb.left > -64)
        mainTexture.draw(m_fire[2]);
}

void Sprite_PlantFireL::draw(RenderTarget& target, RenderStates) const
{
    if (!m_killed)
        target.draw(m_array, m_texture);
}
