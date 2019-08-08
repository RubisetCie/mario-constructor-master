/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_lui.hpp"

#define LUI_MAXVSPEED 12
#define LUI_BOUNCESPEED 10

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_Lui::Sprite_Lui(const Texture& texture) : Pawn()
{
    m_movedistance = 0;

    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 30, 31));

    m_aabb.width = 30;
    m_aabb.height = 31;

    m_frame = 0;
}

void Sprite_Lui::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;
}

void Sprite_Lui::update()
{
    if (m_frame < 71)
        m_frame++;
    else
        m_frame = 0;

    if (m_frame % 4 == 0)
    {
        if (m_frame >= 56)
            m_sprite.setTextureRect(IntRect((3 - ((m_frame - 56)/4)) * 30, 0, 30, 31));
        else if (m_frame >= 40)
            m_sprite.setTextureRect(IntRect(((m_frame-40)/4) * 30, 0, 30, 31));
    }

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (player->m_aabb.intersects(m_aabb))
    {
        if (game_powerup == 0)
            player->setPower(1, false);
        else if (game_powerup != 4)
            player->setPower(4, false);

        m_destroyed = true;

        addPoints(1000);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[5], 0, NULL);
    }
}

void Sprite_Lui::secureUpdate()
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

    if (swimming)
    {
        if (m_movedistance < (LUI_MAXVSPEED - sprite_gravity) / 2)
            m_movedistance += sprite_gravity / 2;
    }
    else
    {
        if (m_movedistance < LUI_MAXVSPEED - sprite_gravity)
            m_movedistance += sprite_gravity;
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

    if (makeMoveVertical(m_movedistance, *currentScale, currentMatrix, currentList))
    {
        if (m_movedistance > 0)
        {
            if (swimming)
                m_movedistance = -LUI_BOUNCESPEED / 2;
            else
                m_movedistance = -LUI_BOUNCESPEED;
        }
        else
            m_movedistance = 0;
    }

    if (m_aabb.top > currentScale->y * 480 + 64)
        m_destroyed = true;

    m_sprite.setPosition(roundf(m_aabb.left), roundf(m_aabb.top));
}

void Sprite_Lui::afterUpdate()
{
}

Vector2f Sprite_Lui::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Sprite_Lui::getSize() const
{
    return Vector2f(30, 31);
}

ID Sprite_Lui::getID() const
{
    return ID_USELESS;
}

void Sprite_Lui::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

