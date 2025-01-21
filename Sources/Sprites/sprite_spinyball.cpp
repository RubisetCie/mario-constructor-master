/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_spinyball.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define BALL_MAXVSPEED 8
#define SPINY_MAXVSPEED 12

using namespace std;
using namespace sf;

Sprite_SpinyBall::Sprite_SpinyBall(const Texture& texture, const Texture& walktexture, Collider* collision, Sprite_Lakitu* parent) : Pawn()
{
    m_sprite.setTexture(texture);
    m_sprite.setOrigin(15, 15);

    m_texture = &walktexture;

    m_movedistance = Vector2f(0, -3);

    m_aabb.width = 32;
    m_aabb.height = 32;

    m_collider = collision;
    m_parent = parent;

    m_state = 0;

    m_onfloor = false;
}

void Sprite_SpinyBall::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x-15;
    m_aabb.top = pos.y-15;
}

void Sprite_SpinyBall::update()
{
    if (m_state == 1)
    {
        if (frame_Flower % 8 == 0)
            m_sprite.setTextureRect(IntRect((frame_Flower/8) * 33, 0, 33, 32));
    }

    if (m_state == 2)
        return;

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (m_state == 1)
    {
        if (blockkiller.intersects(m_aabb))
        {
            if (m_state == 0)
            {
                m_sprite.setTexture(*m_texture);
                m_sprite.setTextureRect(IntRect(0, 0, 33, 32));
                m_sprite.setRotation(0);
                m_sprite.setOrigin(17, 0);
                m_sprite.scale(1, -1);
                m_sprite.move(1, 32);
            }
            else
            {
                m_sprite.scale(1, -1);
                m_sprite.move(0, 32);
            }

            m_collider->rect.left = -64;
            m_collider->destroyed = true;

            m_state = 2;
            m_movedistance = Vector2f(0, -3);

            addPoints(100);
            listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-4, m_aabb.top+16)));

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);

            return;
        }
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
            if (m_state == 0)
            {
                m_sprite.setTexture(*m_texture);
                m_sprite.setTextureRect(IntRect(0, 0, 33, 32));
                m_sprite.setRotation(0);
                m_sprite.setOrigin(17, 0);
                m_sprite.scale(1, -1);
                m_sprite.move(1, 32);
            }
            else
            {
                m_sprite.scale(1, -1);
                m_sprite.move(0, 32);
            }

            m_collider->rect.left = -64;
            m_collider->destroyed = true;

            m_state = 2;
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

void Sprite_SpinyBall::secureUpdate()
{
    if (m_state == 1)
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

    if (m_state < 2)
    {
        bool swimming(false);

        if (m_state > 0)
        {
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
        }

        Matrix* currentMatrix;
        Vector2u* currentScale;
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

        if (enemykiller.intersects(m_aabb))
        {
            if (m_state == 0)
            {
                m_sprite.setTexture(*m_texture);
                m_sprite.setTextureRect(IntRect(0, 0, 33, 32));
                m_sprite.setRotation(0);
                m_sprite.setOrigin(17, 0);
                m_sprite.scale(1, -1);
                m_sprite.move(1, 32);
            }
            else
            {
                m_sprite.scale(1, -1);
                m_sprite.move(0, 32);
            }

            m_collider->rect.left = -64;
            m_collider->destroyed = true;

            m_state = 2;
            m_movedistance = Vector2f(0, -3);

            addPoints(100);
            listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(m_aabb.left-4, m_aabb.top+16)));

            if (m_aabb.left > cameraPos.x - 384 && m_aabb.left < cameraPos.x + 352 && m_aabb.top > cameraPos.y - 304 && m_aabb.top < cameraPos.y + 272)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);
        }

        if (!m_onfloor)
        {
            if (m_state == 0)
            {
                if (m_movedistance.y < BALL_MAXVSPEED - star_gravity)
                    m_movedistance.y += star_gravity;
            }
            else
            {
                if (swimming)
                {
                    if (m_movedistance.y < (SPINY_MAXVSPEED - sprite_gravity) / 2)
                        m_movedistance.y += sprite_gravity / 2;
                }
                else
                {
                    if (m_movedistance.y < SPINY_MAXVSPEED - sprite_gravity)
                        m_movedistance.y += sprite_gravity;
                }
            }
        }

        if (m_movedistance.x != 0)
        {
            if (makeMoveHorizontal(m_movedistance.x, *currentScale, currentMatrix, currentList))
            {
                m_movedistance.x = -m_movedistance.x;
                m_movedistance.y = 0;
            }
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

        if (m_state == 0)
        {
            m_sprite.setPosition(m_aabb.left+15, roundf(m_aabb.top+15));
            m_sprite.rotate(18);

            if (m_onfloor)
            {
                m_sprite.setTexture(*m_texture);
                m_sprite.setTextureRect(IntRect(0, 0, 33, 32));
                m_sprite.setRotation(0);
                m_sprite.setOrigin(17, 0);

                m_state = 1;
            }
        }

        if (m_state == 1)
            m_sprite.setPosition(roundf(m_aabb.left+17), roundf(m_aabb.top));

        m_collider->rect.left = m_aabb.left;
        m_collider->rect.top = m_aabb.top;

        m_collider->speed.x = m_movedistance.x;
    }
    else
    {
        m_sprite.move(0, m_movedistance.y);
        m_movedistance.y += 0.11;

        float dist = m_sprite.getPosition().y;

        if (dist > m_aabb.top + 512)
        {
            m_parent->m_enemies++;
            m_destroyed = true;
        }
    }

    if (zoneb)
    {
        if (m_aabb.top > levelScaleb.y * 480 + 64)
        {
            m_parent->m_enemies++;

            m_collider->destroyed = true;
            m_destroyed = true;
        }
    }
    else
    {
        if (m_aabb.top > levelScale.y * 480 + 64)
        {
            m_parent->m_enemies++;

            m_collider->destroyed = true;
            m_destroyed = true;
        }
    }
}

void Sprite_SpinyBall::afterUpdate()
{
}

Vector2f Sprite_SpinyBall::getPosition() const
{
    return Vector2f(m_aabb.left, m_aabb.top);
}

Vector2f Sprite_SpinyBall::getSize() const
{
    return Vector2f(33, 32);
}

ID Sprite_SpinyBall::getID() const
{
    if (m_state == 0)
        return ID_USELESS;
    else
        return ID_ENEMY;
}

void Sprite_SpinyBall::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

