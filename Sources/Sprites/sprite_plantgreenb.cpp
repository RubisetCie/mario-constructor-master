/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Sprites/sprite_plantgreenb.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_PlantGreenB::Sprite_PlantGreenB(Texture* texture, Collider* collision) : Sprite_PlantB(texture, collision)
{
    m_timer = 340;
}

void Sprite_PlantGreenB::update()
{
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

void Sprite_PlantGreenB::secureUpdate()
{
    if (m_timer < 340)
    {
        m_timer++;

        if (m_timer >= 40 && m_timer < 100)
        {
            m_array[2].position.y += 0.7833333;
            m_array[3].position.y += 0.7833333;

            m_array[0].texCoords.y += 0.7833333;
            m_array[1].texCoords.y += 0.7833333;

            m_aabb.height += 0.7833333;

            m_collider->rect.height += 0.7833333;
        }

        if (m_timer >= 240 && m_timer < 300)
        {
            m_array[2].position.y -= 0.7833333;
            m_array[3].position.y -= 0.7833333;

            m_array[0].texCoords.y -= 0.7833333;
            m_array[1].texCoords.y -= 0.7833333;

            m_aabb.height -= 0.7833333;

            m_collider->rect.height -= 0.7833333;
        }

        if (m_timer == 100)
        {
            m_array[2].position.y = m_startheight+47;
            m_array[3].position.y = m_startheight+47;

            m_array[0].texCoords.y = 47;
            m_array[1].texCoords.y = 47;

            m_aabb.height = 47;

            m_collider->rect.height = 47;
        }
        else if (m_timer == 300)
        {
            m_array[2].position.y = m_startheight;
            m_array[3].position.y = m_startheight;

            m_array[0].texCoords.y = 0;
            m_array[1].texCoords.y = 0;

            m_aabb.height = 0;

            m_collider->rect.height = 0;
        }
    }

    if (m_aabb.height <= 0)
        return;

    if (enemykiller.intersects(m_aabb) && enemykiller_fire != 2)
    {
        m_destroyed = true;

        m_collider->rect.left = -64;
        m_collider->destroyed = true;

        addPoints(100);
        listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-5, m_aabb.top+16)));

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);
    }
}

void Sprite_PlantGreenB::afterUpdate()
{
}

void Sprite_PlantGreenB::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_array, m_texture);
}