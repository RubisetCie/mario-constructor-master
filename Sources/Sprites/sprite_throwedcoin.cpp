/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_throwedcoin.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define COIN_MAXVSPEED 8

using namespace std;
using namespace sf;

Sprite_ThrowedCoin::Sprite_ThrowedCoin(const Texture& texture, Vector2f speed, Sprite_Lakitu* parent) : Pawn()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 19, 28));

    m_movedistance = speed;

    m_aabb.width = 19;
    m_aabb.height = 28;

    m_onfloor = false;

    m_parent = parent;
}

void Sprite_ThrowedCoin::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;
}

void Sprite_ThrowedCoin::update()
{
    if (frame_Hud % 6 == 0)
        m_sprite.setTextureRect(IntRect((frame_Hud / 6) * 19, 0, 19, 28));

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (blockkiller.intersects(m_aabb))
        m_movedistance.y = -4;

    if (player->m_aabb.intersects(FloatRect(Vector2f(m_aabb.left - player->m_movedistance.x, m_aabb.top - player->m_movedistance.y), Vector2f(19, 28))))
    {
        addCoin();

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(4), sfxSamples[1], 0, NULL);

        if (game_coins == 0)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[7], 0, NULL);

        addPoints(200);

        m_parent->m_enemies++;

        m_destroyed = true;
    }
}

void Sprite_ThrowedCoin::secureUpdate()
{
    if (!m_onfloor)
    {
        if (m_movedistance.y < COIN_MAXVSPEED - star_gravity)
            m_movedistance.y += star_gravity;
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
            {
                float newspeed = m_movedistance.y - 2;

                if (newspeed < 0)
                {
                    newspeed = 0;
                    m_onfloor = true;
                }

                m_movedistance.y = -newspeed;
            }
            else
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

    m_sprite.setPosition(roundf(m_aabb.left), roundf(m_aabb.top));

    if (zoneb)
    {
        if (m_aabb.top > levelScaleb.y * 480 + 64)
        {
            m_parent->m_enemies++;
            m_destroyed = true;
        }
    }
    else
    {
        if (m_aabb.top > levelScale.y * 480 + 64)
        {
            m_parent->m_enemies++;
            m_destroyed = true;
        }
    }
}

void Sprite_ThrowedCoin::afterUpdate()
{
}

Vector2f Sprite_ThrowedCoin::getPosition() const
{
    return Vector2f(m_aabb.left, m_aabb.top);
}

Vector2f Sprite_ThrowedCoin::getSize() const
{
    return Vector2f(19, 28);
}

ID Sprite_ThrowedCoin::getID() const
{
    return ID_USELESS;
}

void Sprite_ThrowedCoin::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

