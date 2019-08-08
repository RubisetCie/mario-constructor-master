/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_buzzybeetle.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_BuzzyBeetle::Sprite_BuzzyBeetle(const Texture& sprite_texture, Texture* const shell_texture, Collider* collision) : Walkable()
{
    m_sprite.setTexture(sprite_texture);
    m_sprite.setTextureRect(IntRect(0, 0, 32, 32));
    m_sprite.setOrigin(16, 0);

    m_movedistance = Vector2f(0, 0);

    m_aabb.width = 31;
    m_aabb.height = 32;

    m_collider = collision;

    m_shelltexture = shell_texture;

    m_state = 0;
    m_frame = 0;
}

void Sprite_BuzzyBeetle::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x-15;
    m_aabb.top = pos.y;
}

void Sprite_BuzzyBeetle::update()
{
    m_active = true;

    if (m_state == 1)
        return;

    if (m_state == 0)
    {
        if (frame_Water % 12 == 0)
            m_sprite.setTextureRect(IntRect((frame_Water/12) * 31, 0, 31, 32));
    }
    else if (m_state >= 2)
    {
        if (m_movedistance.x != 0)
        {
            if (m_movedistance.x > 0)
            {
                if (m_frame < 11)
                    m_frame++;
                else
                    m_frame = 0;
            }
            else
            {
                if (m_frame > 0)
                    m_frame--;
                else
                    m_frame = 11;
            }

            if (m_frame % 3 == 0)
                m_sprite.setTextureRect(IntRect((m_frame/3) * 33, 0, 33, 32));
        }
    }

    if (m_state > 2)
        m_state--;
    else
    {
        if (m_movedistance.x != 0)
            m_collider->type = C_STOMPABLE;
    }

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

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

    if ((enemystomper.intersects(m_aabb) || enemystomper_pt == m_collider) && m_collider->type == C_STOMPABLE)
    {
        if (m_state == 0)
        {
            m_sprite.setTexture(*m_shelltexture);
            m_sprite.setTextureRect(IntRect(0, 0, 33, 32));
            m_sprite.setScale(1, 1);
        }
        else
            m_sprite.setTextureRect(IntRect(0, 0, 33, 32));

        m_collider->type = C_UNSTOMPABLE;

        m_state = 2;
        m_movedistance = Vector2f(0, 0);

        addPoints(100);
        listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-5, m_aabb.top+16)));

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(8), sfxSamples[11], 0, NULL);

        return;
    }

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
            listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left-15, m_aabb.top+1)));

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
            if (m_collider->type == C_STOMPABLE)
            {
                if (player->m_animpowerup > 0)
                    return;

                if (game_powerup <= 1 && player->m_animtrace > 0)
                    return;

                if (m_state <= 2)
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
            else
            {
                if (m_movedistance.x == 0)
                {
                    m_movedistance.x = player->getPosition().x + 21 >= m_sprite.getPosition().x?-6:6;
                    m_state = 12;

                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);
                }
            }
        }
    }
}

void Sprite_BuzzyBeetle::secureUpdate()
{
    if (!m_active)
        return;

    if (m_state != 1)
    {
        if (m_state == 0)
        {
            if (m_movedistance.x == 0 && player != NULL)
            {
                float playerpos = player->getPosition().x;
                float pos = m_sprite.getPosition().x;

                m_movedistance.x = pos>=playerpos+14?-1.2:1.2;
            }

            if (m_movedistance.x < 0)
                m_sprite.setScale(1, 1);
            else
                m_sprite.setScale(-1, 1);
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
    else
    {
        Walkable::update();

        m_sprite.setPosition(roundf(m_aabb.left+15), roundf(m_aabb.top));

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

void Sprite_BuzzyBeetle::afterUpdate()
{
}

Vector2f Sprite_BuzzyBeetle::getPosition() const
{
    return Vector2f(m_aabb.left, m_aabb.top);
}

Vector2f Sprite_BuzzyBeetle::getSize() const
{
    return Vector2f(31, 32);
}

ID Sprite_BuzzyBeetle::getID() const
{
    if (m_state < 2)
    {
        if (m_collider->type == C_STOMPABLE)
            return ID_ENEMY;
        else
            return ID_USELESS;
    }
    else
        return ID_SHELL;
}

void Sprite_BuzzyBeetle::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
