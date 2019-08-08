/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_plantr.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_PlantR::Sprite_PlantR(Texture* texture, Collider* collision) : Placeable()
{
    m_array.setPrimitiveType(Quads);

    m_texture = texture;

    m_aabb.width = 0;
    m_aabb.height = 31;

    m_collider = collision;
}

void Sprite_PlantR::setPosition(Vector2f pos)
{
    m_array.append(Vertex(pos, Color::White, Vector2f(0, 0)));
    m_array.append(Vertex(Vector2f(pos.x, pos.y), Color::White, Vector2f(0, 0)));
    m_array.append(Vertex(Vector2f(pos.x, pos.y + 31), Color::White, Vector2f(31, 0)));
    m_array.append(Vertex(Vector2f(pos.x, pos.y + 31), Color::White, Vector2f(31, 0)));

    m_startwidth = pos.x;

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;
}

void Sprite_PlantR::update()
{
    if (frame_Flower % 8 == 0)
    {
        m_array[0].texCoords.x = (frame_Flower/8) * 31;
        m_array[1].texCoords.x = (frame_Flower/8) * 31;
        m_array[2].texCoords.x = (frame_Flower/8) * 31 + 31;
        m_array[3].texCoords.x = (frame_Flower/8) * 31 + 31;
    }

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (player->m_animpowerup > 0)
        return;

    if (game_powerup <= 1 && player->m_animtrace > 0)
        return;

    FloatRect const aabb = player->m_aabb;
    if (aabb.intersects(m_aabb))
    {
        if (player->m_invincibility > 0)
        {
            m_destroyed = true;

            m_collider->rect.left = -64;
            m_collider->destroyed = true;

            listEffect.emplace_back(new Effect_Score(effectTxt[13], killcount, Vector2f(aabb.left-10, aabb.top-20)));
            listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left, m_aabb.top+7)));

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

Vector2f Sprite_PlantR::getPosition() const
{
    return Vector2f(m_startwidth, m_aabb.top);
}

Vector2f Sprite_PlantR::getSize() const
{
    return Vector2f(47, 31);
}

ID Sprite_PlantR::getID() const
{
    return ID_USELESS;
}
