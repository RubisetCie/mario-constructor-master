/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_paratroopa.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define PARATROOPA_MAXVSPEED 12

using namespace std;
using namespace sf;

Sprite_Paratroopa::Sprite_Paratroopa(const Texture& texture, Texture* const turtle_texture, Texture* const shell_texture, Collider* collision) : Pawn()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 31, 32));
    m_sprite.setOrigin(16, 0);

    m_shelltexture = shell_texture;
    m_turtletexture = turtle_texture;

    m_movedistance = Vector2f(0, 0);

    m_aabb.width = 32;
    m_aabb.height = 32;

    m_collider = collision;

    m_state = 0;
    m_frame = 0;

    m_onfloor = false;
}

void Sprite_Paratroopa::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x-16;
    m_aabb.top = pos.y+16;
}

void Sprite_Paratroopa::update()
{
    m_active = true;

    if (m_state == 2)
        return;

    if (m_state < 3)
    {
        if (frame_Water % 12 == 0)
        {
            if (m_state == 0)
                m_sprite.setTextureRect(IntRect((frame_Water/12) * 32, 0, 32, 48));
            else
                m_sprite.setTextureRect(IntRect((frame_Water/12) * 32, 0, 32, 47));
        }
    }
    else
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

    if (m_state > 3)
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

        if (m_state == 0)
            m_sprite.move(0, 48);
        else if (m_state == 1)
            m_sprite.move(0, 47);
        else
            m_sprite.move(0, 32);

        m_collider->rect.left = -64;
        m_collider->destroyed = true;

        m_state = 2;
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
            m_sprite.setTexture(*m_turtletexture);
            m_sprite.setTextureRect(IntRect(0, 0, 32, 47));
            m_sprite.move(0, 1);

            m_movedistance.y = 0;

            m_state = 1;

            toTurtle();
        }
        else if (m_state == 1)
        {
            m_sprite.setTexture(*m_shelltexture);
            m_sprite.setTextureRect(IntRect(0, 0, 33, 32));
            m_sprite.setScale(1, 1);
            m_sprite.move(0, 15);

            m_movedistance = Vector2f(0, 0);

            m_state = 3;

            m_collider->type = C_UNSTOMPABLE;
        }
        else
        {
            m_sprite.setTextureRect(IntRect(0, 0, 33, 32));

            m_movedistance = Vector2f(0, 0);

            m_collider->type = C_UNSTOMPABLE;
        }

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

            if (m_state == 0)
                m_sprite.move(0, 48);
            else if (m_state == 1)
                m_sprite.move(0, 47);
            else
                m_sprite.move(0, 32);

            m_collider->rect.left = -64;
            m_collider->destroyed = true;

            m_state = 2;
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

                if (m_state <= 3)
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
                    m_state = 13;

                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);
                }
            }
        }
    }
}

void Sprite_Paratroopa::secureUpdate()
{
    if (m_state != 2)
    {
        if (enemykiller.intersects(m_aabb))
        {
            m_sprite.scale(1, -1);

            if (m_state == 0)
                m_sprite.move(0, 47);
            else if (m_state == 1)
                m_sprite.move(0, 47);
            else
                m_sprite.move(0, 32);

            m_collider->rect.left = -64;
            m_collider->destroyed = true;

            m_state = 2;
            m_movedistance = Vector2f(0, -3);

            addPoints(100);
            listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-5, m_aabb.top+16)));

            if (m_aabb.left > cameraPos.x - 384 && m_aabb.left < cameraPos.x + 352 && m_aabb.top > cameraPos.y - 304 && m_aabb.top < cameraPos.y + 272)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);
        }
    }

    if (m_state == 2)
    {
        m_sprite.move(0, m_movedistance.y);
        m_movedistance.y += 0.11;

        float dist = m_sprite.getPosition().y;

        if (dist > m_aabb.top + 512)
            m_destroyed = true;
    }
    else
    {
        if (m_state == 0)
            flyUpdate();
        else
            walkUpdate();

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
}

void Sprite_Paratroopa::walkUpdate()
{
    bool swimming(false);

    if (zoneb)
    {
        if (waterb != NULL)
        {
            if (!waterb->getType())
            {
                float height = waterb->getHeight();

                if (m_aabb.top > height - 24)
                    swimming = true;
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

                if (m_aabb.top > height - 24)
                    swimming = true;
            }
        }
    }

    if (m_state == 1)
    {
        if (m_movedistance.x < 0)
            m_sprite.setScale(1, 1);
        else
            m_sprite.setScale(-1, 1);
    }

    if (!m_onfloor)
    {
        if (swimming)
        {
            if (m_movedistance.y < (PARATROOPA_MAXVSPEED - sprite_gravity) / 2)
                m_movedistance.y += sprite_gravity / 2;
        }
        else
        {
            if (m_movedistance.y < PARATROOPA_MAXVSPEED - sprite_gravity)
                m_movedistance.y += sprite_gravity;
        }
    }

    Matrix* currentMatrix;
    Vector2i* currentScale;
    list<Collider*>* currentList;

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

    if (m_movedistance.x != 0)
    {
        if (makeMoveHorizontal(m_movedistance.x, *currentScale, currentMatrix, currentList))
            m_movedistance.x = -m_movedistance.x;
    }

    if (m_movedistance.y != 0)
    {
        if (makeMoveVertical(m_movedistance.y, *currentScale, currentMatrix, currentList))
        {
            if (m_movedistance.y > 0)
                m_onfloor = true;

            m_movedistance.y = 0;
        }
    }

    if (m_platform != NULL)
    {
        makeMoveHorizontalPlatform(m_platform->speed.x, *currentScale, currentMatrix, currentList);
        makeMoveVerticalPlatform(m_platform->speed.y, *currentScale, currentMatrix, currentList);
    }

    if (m_onfloor)
    {
        if (!testVertical(1, *currentScale, currentMatrix, currentList))
        {
            m_onfloor = false;
            m_platform = NULL;
        }
    }

    if (m_state == 1)
        m_sprite.setPosition(roundf(m_aabb.left+16), roundf(m_aabb.top-15));
    else
        m_sprite.setPosition(roundf(m_aabb.left+16), roundf(m_aabb.top));

    m_collider->rect.left = m_aabb.left;
    m_collider->rect.top = m_aabb.top;

    m_collider->speed.x = m_movedistance.x;
}

void Sprite_Paratroopa::afterUpdate()
{
}

Vector2f Sprite_Paratroopa::getPosition() const
{
    return Vector2f(m_aabb.left, m_aabb.top-16);
}

Vector2f Sprite_Paratroopa::getSize() const
{
    return Vector2f(32, 48);
}
