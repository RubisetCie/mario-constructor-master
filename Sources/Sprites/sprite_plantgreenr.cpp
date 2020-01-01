/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Sprites/sprite_plantgreenr.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_PlantGreenR::Sprite_PlantGreenR(Texture* texture, Collider* collision) : Sprite_PlantR(texture, collision)
{
    m_timer = 340;
}

void Sprite_PlantGreenR::update()
{
    if (player != NULL)
    {
        if (!(player->m_aabb.left > m_aabb.left - 2 && player->m_aabb.left < m_aabb.left + 5 && player->m_aabb.top > m_aabb.top - 32 && player->m_aabb.top < m_aabb.top + 32))
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

    Sprite_PlantR::update();
}

void Sprite_PlantGreenR::secureUpdate()
{
    if (m_timer < 340)
    {
        m_timer++;

        if (m_timer < 60)
        {
            m_array[1].position.x += 0.7833333;
            m_array[2].position.x += 0.7833333;

            m_array[0].texCoords.y += 0.7833333;
            m_array[3].texCoords.y += 0.7833333;

            m_aabb.width += 0.7833333;

            m_collider->rect.width += 0.7833333;
        }

        if (m_timer >= 200 && m_timer < 260)
        {
            m_array[1].position.x -= 0.7833333;
            m_array[2].position.x -= 0.7833333;

            m_array[0].texCoords.y -= 0.7833333;
            m_array[3].texCoords.y -= 0.7833333;

            m_aabb.width -= 0.7833333;

            m_collider->rect.width -= 0.7833333;
        }

        if (m_timer == 60)
        {
            m_array[1].position.x = m_startwidth + 47;
            m_array[2].position.x = m_startwidth + 47;

            m_array[0].texCoords.y = 47;
            m_array[3].texCoords.y = 47;

            m_aabb.width = 47;

            m_collider->rect.width = 47;
        }
        else if (m_timer == 260)
        {
            m_array[1].position.x = m_startwidth;
            m_array[2].position.x = m_startwidth;

            m_array[0].texCoords.y = 0;
            m_array[3].texCoords.y = 0;

            m_aabb.width = 0;

            m_collider->rect.width = 0;
        }
    }

    if (m_aabb.width <= 0)
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

void Sprite_PlantGreenR::afterUpdate()
{
}

void Sprite_PlantGreenR::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_array, m_texture);
}
