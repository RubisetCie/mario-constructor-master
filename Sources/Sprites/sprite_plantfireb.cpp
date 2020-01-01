/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Sprites/sprite_plantfireb.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_PlantFireB::Sprite_PlantFireB(Texture* texture, Collider* collision) : Sprite_PlantB(texture, collision), m_fire {(itemsTxt[9]), (itemsTxt[9]), (itemsTxt[9])}
{
    m_timer = 340;
    m_killed = false;
}

void Sprite_PlantFireB::update()
{
    if (m_killed)
        return;

    if (player != NULL)
    {
        if (player->m_aabb.left < m_aabb.left - 42 || player->m_aabb.left > m_aabb.left + 42)
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

    Sprite_PlantB::update();
}

void Sprite_PlantFireB::secureUpdate()
{
    if (!m_killed)
    {
        if (m_timer < 340)
        {
            m_timer++;

            if (m_timer < 60)
            {
                m_array[2].position.y += 0.7833333;
                m_array[3].position.y += 0.7833333;

                m_array[0].texCoords.y += 0.7833333;
                m_array[1].texCoords.y += 0.7833333;

                m_aabb.height += 0.7833333;

                m_collider->rect.height += 0.7833333;
            }

            if (m_timer >= 200 && m_timer < 260)
            {
                m_array[2].position.y -= 0.7833333;
                m_array[3].position.y -= 0.7833333;

                m_array[0].texCoords.y -= 0.7833333;
                m_array[1].texCoords.y -= 0.7833333;

                m_aabb.height -= 0.7833333;

                m_collider->rect.height -= 0.7833333;
            }

            if (m_timer == 60)
            {
                m_array[2].position.y = m_startheight+47;
                m_array[3].position.y = m_startheight+47;

                m_array[0].texCoords.y = 47;
                m_array[1].texCoords.y = 47;

                m_aabb.height = 47;

                m_collider->rect.height = 47;
            }
            else if (m_timer == 90)
            {
                m_fire[0].cast(Vector2f(m_aabb.left + 15, m_aabb.top + 40), Vector2f((rand() / (RAND_MAX / 6.0f)) - 3.0f, rand() / static_cast<float>(RAND_MAX)));

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
            }
            else if (m_timer == 100)
            {
                m_fire[1].cast(Vector2f(m_aabb.left + 15, m_aabb.top + 40), Vector2f((rand() / (RAND_MAX / 6.0f)) - 3.0f, rand() / static_cast<float>(RAND_MAX)));

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
            }
            else if (m_timer == 110)
            {
                m_fire[2].cast(Vector2f(m_aabb.left + 15, m_aabb.top + 40), Vector2f((rand() / (RAND_MAX / 6.0f)) - 3.0f, rand() / static_cast<float>(RAND_MAX)));

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
            }
            else if (m_timer == 260)
            {
                m_array[2].position.y = m_startheight;
                m_array[3].position.y = m_startheight;

                m_array[0].texCoords.y = 0;
                m_array[1].texCoords.y = 0;

                m_aabb.height = 0;

                m_collider->rect.height = 0;
            }
        }

        if (m_aabb.height > 0)
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

void Sprite_PlantFireB::afterUpdate()
{
    if (m_fire[0].m_aabb.left > -64)
        mainTexture.draw(m_fire[0]);

    if (m_fire[1].m_aabb.left > -64)
        mainTexture.draw(m_fire[1]);

    if (m_fire[2].m_aabb.left > -64)
        mainTexture.draw(m_fire[2]);
}

void Sprite_PlantFireB::draw(RenderTarget& target, RenderStates) const
{
    if (!m_killed)
        target.draw(m_array, m_texture);
}
