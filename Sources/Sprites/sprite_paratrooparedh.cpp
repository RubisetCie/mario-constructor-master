/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_paratrooparedh.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define M_PARATROOPA_MAXHSPEED 1.5
#define M_PARATROOPA_ACCELERATION 0.1

using namespace sf;
using namespace std;

Sprite_ParatroopaRedH::Sprite_ParatroopaRedH(const Texture& texture, Texture* const turtle_texture, Texture* const shell_texture, Collider* collision, float target) : Sprite_Paratroopa(texture, turtle_texture, shell_texture, collision)
{
    m_target = target + 16;

    m_forward = true;
}

void Sprite_ParatroopaRedH::setPosition(const Vector2f& pos)
{
    m_startpos = pos.x;

    Sprite_Paratroopa::setPosition(pos);

    if (m_target > m_startpos)
    {
        m_startpos = m_target;
        m_target = pos.x;

        m_forward = false;
    }

    if (m_startpos - m_target < 32)
        m_target -= 32;

    if (!m_forward)
    {
        m_sprite.setPosition(m_target, pos.y);
        m_sprite.setScale(1, 1);

        m_aabb.left = m_target-16;
        m_aabb.top = pos.y+16;
    }
}

void Sprite_ParatroopaRedH::flyUpdate()
{
    if (m_movedistance.x < 0)
        m_sprite.setScale(1, 1);
    else if (m_movedistance.x > 0)
        m_sprite.setScale(-1, 1);

    if (m_forward)
    {
        if (m_target < m_aabb.left)
        {
            if (m_movedistance.x > -M_PARATROOPA_MAXHSPEED)
                m_movedistance.x -= M_PARATROOPA_ACCELERATION;
        }
        else
            m_forward = false;
    }
    else
    {
        if (m_startpos > m_aabb.left + 32)
        {
            if (m_movedistance.x < M_PARATROOPA_MAXHSPEED)
                m_movedistance.x += M_PARATROOPA_ACCELERATION;
        }
        else
            m_forward = true;
    }

    // Collision Events :
    if (zoneb)
    {
        if (makeMoveHorizontal(m_movedistance.x, levelScaleb, collisionMatrixb, &listColliderb))
        {
            m_movedistance.x = -m_movedistance.x;
            m_forward = !m_forward;
        }
    }
    else
    {
        if (makeMoveHorizontal(m_movedistance.x, levelScale, collisionMatrix, &listCollider))
        {
            m_movedistance.x = -m_movedistance.x;
            m_forward = !m_forward;
        }
    }

    m_sprite.setPosition(roundf(m_aabb.left+16), roundf(m_aabb.top-16));

    m_collider->rect.left = m_aabb.left;
    m_collider->rect.top = m_aabb.top;
}

void Sprite_ParatroopaRedH::toTurtle()
{
    if (m_sprite.getScale().x < 0)
        m_movedistance.x = 1.8;
    else
        m_movedistance.x = -1.8;
}

ID Sprite_ParatroopaRedH::getID() const
{
    if (m_state == 0)
        return ID_USELESS;
    else if (m_state >= 2)
        return ID_SHELL;
    else
        return ID_WONTFALL;
}

void Sprite_ParatroopaRedH::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
