/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include <list>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_goombagrey.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/Marks/mark_mario.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_GoombaGrey::Sprite_GoombaGrey(const Texture& texture, Collider* collision) : Walkable()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 31, 32));

    m_movedistance = Vector2f(0, 0);

    m_aabb.width = 31;
    m_aabb.height = 32;

    m_collider = collision;

    m_state = 0;
}

void Sprite_GoombaGrey::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;
}

void Sprite_GoombaGrey::update()
{
    m_active = true;

    if (m_state > 0)
        return;

    if (frame_Flower % 8 == 0)
        m_sprite.setTextureRect(IntRect((frame_Flower/8) * 31, 0, 31, 32));

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (enemystomper.intersects(m_aabb) || enemystomper_pt == m_collider)
    {
        m_sprite.setTextureRect(IntRect(62, 0, 31, 32));

        m_collider->rect.left = -64;
        m_collider->destroyed = true;

        m_state = 2;
        m_movedistance = Vector2f(0, 0);

        addPoints(100);
        listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-5, m_aabb.top+16)));

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(8), sfxSamples[11], 0, NULL);

        return;
    }

    if (blockkiller.intersects(m_aabb))
    {
        m_sprite.scale(1, -1);
        m_sprite.move(0, 32);

        m_collider->rect.left = -64;
        m_collider->destroyed = true;

        m_state = 1;
        m_movedistance = Vector2f(0, -3);

        addPoints(100);
        listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-5, m_aabb.top+16)));

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);

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

            m_state = 1;
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

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);
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

void Sprite_GoombaGrey::secureUpdate()
{
    if (!m_active)
        return;

    if (m_state == 0)
    {
        if (m_movedistance.x == 0 && player != NULL)
        {
            float playerpos = player->getPosition().x;
            float pos = m_sprite.getPosition().x;

            m_movedistance.x = pos>=playerpos-2?-1.2:1.2;
        }

        if (enemykiller.intersects(m_aabb) && enemykiller_fire != 0)
        {
            m_sprite.scale(1, -1);
            m_sprite.move(0, 32);

            m_collider->rect.left = -64;
            m_collider->destroyed = true;

            m_state = 1;
            m_movedistance = Vector2f(0, -3);

            addPoints(100);
            listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-5, m_aabb.top+16)));

            if (m_aabb.left > cameraPos.x - 384 && m_aabb.left < cameraPos.x + 352 && m_aabb.top > cameraPos.y - 304 && m_aabb.top < cameraPos.y + 272)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);
        }
    }

    if (m_state == 1)
    {
        m_sprite.move(0, m_movedistance.y);
        m_movedistance.y += 0.11;

        float dist = m_sprite.getPosition().y;

        if (dist > m_aabb.top + 512)
            m_destroyed = true;
    }
    else if (m_state > 1)
        m_state++;

    if (m_state != 1)
    {
        Walkable::update();

        m_sprite.setPosition(roundf(m_aabb.left), roundf(m_aabb.top));
    }

    if (m_state == 180)
        m_destroyed = true;

    if (m_state == 0)
    {
        m_collider->rect.left = m_aabb.left;
        m_collider->rect.top = m_aabb.top;

        m_collider->speed.x = m_movedistance.x;
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

void Sprite_GoombaGrey::afterUpdate()
{
}

Vector2f Sprite_GoombaGrey::getPosition() const
{
    return Vector2f(m_aabb.left, m_aabb.top);
}

Vector2f Sprite_GoombaGrey::getSize() const
{
    return Vector2f(31, 32);
}

ID Sprite_GoombaGrey::getID() const
{
    if (m_state == 0)
        return ID_ENEMY;
    else
        return ID_USELESS;
}

void Sprite_GoombaGrey::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

