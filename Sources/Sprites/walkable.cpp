/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Sprites/walkable.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define WALKABLE_MAXVSPEED 12

using namespace std;
using namespace sf;

Walkable::Walkable() : Pawn()
{
    m_onfloor = false;
    m_active = false;
}

void Walkable::update()
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
            if (m_movedistance.y < (WALKABLE_MAXVSPEED - sprite_gravity) / 2.0f)
                m_movedistance.y += sprite_gravity / 2;
        }
        else
        {
            if (m_movedistance.y < WALKABLE_MAXVSPEED - sprite_gravity)
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
}
