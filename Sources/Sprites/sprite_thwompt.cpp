/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_thwompt.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define THWOMP_MAXVSPEED 18
#define THWOMP_LAUGHDURATION 70

using namespace std;
using namespace sf;

Sprite_ThwompT::Sprite_ThwompT(const Texture& texture) : Pawn()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 54, 68));
    m_sprite.setScale(1, -1);

    m_movedistance = 0;

    m_aabb.width = 50;
    m_aabb.height = 64;

    m_state = 0;
    m_laugh = 0;
    m_frame = 0;
}

void Sprite_ThwompT::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);

    m_startheight = pos.y - 68;

    m_aabb.left = pos.x + 2;
    m_aabb.top = m_startheight;
}

void Sprite_ThwompT::update()
{
    if (m_laugh > 0)
    {
        if (m_laugh % 4 == 0)
        {
            m_sprite.setTextureRect(IntRect((m_frame * 54) + 54, 0, 54, 68));

            m_frame++;
            if (m_frame >= 3)
                m_frame = 0;
        }
    }
    else
        m_sprite.setTextureRect(IntRect(0, 0, 54, 68));

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (player->m_animpowerup > 0)
        return;

    if (game_powerup <= 1 && player->m_animtrace > 0)
        return;

    if (player->m_invincibility > 0)
        return;

    if (player->m_aabb.intersects(m_aabb))
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

        m_laugh = THWOMP_LAUGHDURATION;
    }
}

void Sprite_ThwompT::secureUpdate()
{
    if (m_state == 1)
    {
        Vector2u* currentScale;

        if (m_movedistance > -THWOMP_MAXVSPEED + mario_gravity)
            m_movedistance -= mario_gravity;

        if (zoneb)
        {
            currentScale = &levelScaleb;

            if (makeMoveVerticalHigh(m_movedistance, *currentScale, collisionMatrixb, &listColliderb))
            {
                m_state = 2;

                m_movedistance = 0;

                levelCamera_shake = 16;

                spritehitter.left = m_aabb.left;
                spritehitter.top = m_aabb.top-4;
                spritehitter.width = 50;
                spritehitter.height = 8;
                spritehitter_create = false;

                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left - 7, m_aabb.top - 15)));
                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left + 26, m_aabb.top - 15)));

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), sfxSamples[9], 0, NULL);
            }
        }
        else
        {
            currentScale = &levelScale;

            if (makeMoveVerticalHigh(m_movedistance, *currentScale, collisionMatrix, &listCollider))
            {
                m_state = 2;

                m_movedistance = 0;

                levelCamera_shake = 16;

                spritehitter.left = m_aabb.left;
                spritehitter.top = m_aabb.top-4;
                spritehitter.width = 50;
                spritehitter.height = 8;
                spritehitter_create = false;

                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left - 7, m_aabb.top - 15)));
                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left + 26, m_aabb.top - 15)));

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), sfxSamples[9], 0, NULL);
            }
        }

        if (m_aabb.top > currentScale->y * 480 + 96)
            m_destroyed = true;
    }
    else if (m_state == 100)
    {
        if (m_aabb.top < m_startheight)
            m_aabb.top += 1.1;
        else
        {
            m_aabb.top = m_startheight;
            m_state = 0;
        }
    }

    if (m_state > 1 && m_state < 100)
        m_state++;

    m_sprite.setPosition(m_aabb.left - 2, roundf(m_aabb.top + 68));

    if (m_state == 0)
    {
        if (player == NULL)
            return;

        if (!player->m_active)
            return;

        if (player->m_aabb.top < m_aabb.top + 64 && player->m_aabb.left > m_aabb.left - 70 && player->m_aabb.left < m_aabb.left + 92)
            m_state = 1;
    }

    if (m_laugh > 0)
    {
        m_laugh--;

        if (m_laugh == 1)
        {
            m_sprite.setTextureRect(IntRect(0, 0, 54, 68));
            m_frame = 0;
        }
    }
}

void Sprite_ThwompT::afterUpdate()
{
}

Vector2f Sprite_ThwompT::getPosition() const
{
    return Vector2f(m_aabb.left - 2, m_aabb.top - 4);
}

Vector2f Sprite_ThwompT::getSize() const
{
    return Vector2f(58, 68);
}

ID Sprite_ThwompT::getID() const
{
    return ID_USELESS;
}

void Sprite_ThwompT::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
