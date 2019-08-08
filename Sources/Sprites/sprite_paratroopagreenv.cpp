/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_paratroopagreenv.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define M_PARATROOPA_MAXVSPEED 1.5
#define M_PARATROOPA_ACCELERATION 0.1

using namespace sf;
using namespace std;

Sprite_ParatroopaGreenV::Sprite_ParatroopaGreenV(const Texture& texture, Texture* const turtle_texture, Texture* const shell_texture, Collider* collision, float target) : Sprite_Paratroopa(texture, turtle_texture, shell_texture, collision)
{
    m_target = target;

    m_forward = true;
}

void Sprite_ParatroopaGreenV::setPosition(Vector2f pos)
{
    m_startpos = pos.y;

    Sprite_Paratroopa::setPosition(pos);

    if (m_target > m_startpos)
    {
        m_startpos = m_target;
        m_target = pos.y;

        m_forward = false;
    }

    if (m_startpos - m_target < 32)
        m_target -= 32;

    if (!m_forward)
    {
        m_sprite.setPosition(pos.x, m_target);

        m_aabb.left = pos.x - 16;
        m_aabb.top = m_target + 16;
    }
}

void Sprite_ParatroopaGreenV::flyUpdate()
{
    if (player != NULL)
    {
        if (player->m_aabb.left < m_aabb.left - 2)
            m_sprite.setScale(1, 1);
        else
            m_sprite.setScale(-1, 1);
    }

    if (m_forward)
    {
        if (m_target < m_aabb.top - 16)
        {
            if (m_movedistance.y > -M_PARATROOPA_MAXVSPEED)
                m_movedistance.y -= M_PARATROOPA_ACCELERATION;
        }
        else
            m_forward = false;
    }
    else
    {
        if (m_startpos > m_aabb.top)
        {
            if (m_movedistance.y < M_PARATROOPA_MAXVSPEED)
                m_movedistance.y += M_PARATROOPA_ACCELERATION;
        }
        else
            m_forward = true;
    }

    // Collision Events :
    if (zoneb)
    {
        if (makeMoveVertical(m_movedistance.y, levelScaleb, collisionMatrixb, &listColliderb))
        {
            m_movedistance.y = -m_movedistance.y;
            m_forward = !m_forward;
        }
    }
    else
    {
        if (makeMoveVertical(m_movedistance.y, levelScale, collisionMatrix, &listCollider))
        {
            m_movedistance.y = -m_movedistance.y;
            m_forward = !m_forward;
        }
    }

    m_sprite.setPosition(roundf(m_aabb.left+16), roundf(m_aabb.top-16));

    m_collider->rect.left = m_aabb.left;
    m_collider->rect.top = m_aabb.top;
}

void Sprite_ParatroopaGreenV::toTurtle()
{
    if (m_sprite.getScale().x < 0)
        m_movedistance.x = 1.2;
    else
        m_movedistance.x = -1.2;
}

ID Sprite_ParatroopaGreenV::getID() const
{
    if (m_state == 0)
        return ID_STOMPABLE;
    else if (m_state >= 2)
        return ID_SHELL;
    else
        return ID_ENEMY;
}

void Sprite_ParatroopaGreenV::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
