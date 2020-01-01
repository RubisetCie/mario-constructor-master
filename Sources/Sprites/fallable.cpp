/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Sprites/fallable.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define FALLABLE_MAXVSPEED 12

using namespace std;
using namespace sf;

Fallable::Fallable() : Pawn()
{
    m_onfloor = false;
}

void Fallable::update()
{
    bool swimming(false);

    if (zoneb)
    {
        if (waterb != NULL)
        {
            if (!waterb->getType())
            {
                float height = waterb->getHeight();

                if (m_aabb.top + m_aabb.height > height + 8)
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

                if (m_aabb.top + m_aabb.height > height + 8)
                    swimming = true;
            }
        }
    }

    if (!m_onfloor)
    {
        if (swimming)
        {
            if (m_movedistance < (FALLABLE_MAXVSPEED - sprite_gravity) / 2.0f)
                m_movedistance += sprite_gravity / 2;
        }
        else
        {
            if (m_movedistance < FALLABLE_MAXVSPEED - sprite_gravity)
                m_movedistance += sprite_gravity;
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

    if (m_movedistance != 0)
    {
        if (makeMoveVertical(m_movedistance, *currentScale, currentMatrix, currentList))
        {
            if (m_movedistance > 0)
                m_onfloor = true;

            m_movedistance = 0;
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

    if (m_aabb.top > currentScale->y * 480 + 64)
        m_destroyed = true;
}
