/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_paratroopabluej.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define PARATROOPA_BOUNCE 12
#define PARATROOPA_MAXVSPEED 10

using namespace sf;
using namespace std;

Sprite_ParatroopaBlueJ::Sprite_ParatroopaBlueJ(const Texture& texture, Texture* const turtle_texture, Texture* const shell_texture, Collider* collision) : Sprite_Paratroopa(texture, turtle_texture, shell_texture, collision)
{
}

void Sprite_ParatroopaBlueJ::flyUpdate()
{
    if (!m_active)
        return;

    if (m_movedistance.x == 0 && player != NULL)
    {
        float playerpos = player->getPosition().x;
        float pos = m_sprite.getPosition().x;

        m_movedistance.x = pos>=playerpos+14?-1.8:1.8;
    }

    if (m_movedistance.x < 0)
        m_sprite.setScale(1, 1);
    else
        m_sprite.setScale(-1, 1);

    if (m_movedistance.y < PARATROOPA_MAXVSPEED - sprite_gravity)
        m_movedistance.y += sprite_gravity;

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

    if (makeMoveHorizontal(m_movedistance.x, *currentScale, currentMatrix, currentList))
        m_movedistance.x = -m_movedistance.x;

    if (makeMoveVertical(m_movedistance.y, *currentScale, currentMatrix, currentList))
    {
        if (m_movedistance.y > 0)
            m_movedistance.y = -PARATROOPA_BOUNCE;
        else
            m_movedistance.y = 0;
    }

    m_sprite.setPosition(roundf(m_aabb.left+16), roundf(m_aabb.top-16));

    m_collider->rect.left = m_aabb.left;
    m_collider->rect.top = m_aabb.top;
}

void Sprite_ParatroopaBlueJ::toTurtle()
{
}

ID Sprite_ParatroopaBlueJ::getID() const
{
    if (m_state == 0)
        return ID_STOMPABLE;
    else if (m_state >= 2)
        return ID_SHELL;
    else
        return ID_ENEMY;
}

void Sprite_ParatroopaBlueJ::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
