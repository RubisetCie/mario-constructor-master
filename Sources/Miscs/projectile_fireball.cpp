/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/Miscs/projectile_fireball.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define FIREBALL_SPEED 7.5
#define FIREBALL_MAXVSPEED 10
#define FIREBALL_ROTATION 25
#define FIREBALL_BOUNCE 4.5

using namespace std;
using namespace sf;

Projectile_Fireball::Projectile_Fireball(Texture* texture)
{
    m_sprite.setTexture(*texture);
    m_sprite.setPosition(-64, -64);
    m_sprite.setOrigin(6, 8);

    m_aabb = FloatRect(-64, -64, 15, 16);

    m_movedistance = Vector2f(0, 0);
}

void Projectile_Fireball::update()
{
    if (m_movedistance.y < FIREBALL_MAXVSPEED - sprite_gravity)
        m_movedistance.y += sprite_gravity;

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

    if (m_movedistance.x > 0)
        m_sprite.rotate(FIREBALL_ROTATION);
    else
        m_sprite.rotate(-FIREBALL_ROTATION);

    if (makeMoveHorizontal(m_movedistance.x, *currentScale, currentMatrix, currentList, true))
    {
        listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left-9, m_aabb.top-7)));
        m_aabb.left = -64;
    }

    if (makeMoveVertical(m_movedistance.y, *currentScale, currentMatrix, currentList, false))
    {
        if (m_movedistance.y > 0)
            m_movedistance.y = -FIREBALL_BOUNCE;
        else
        {
            listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left-9, m_aabb.top-7)));
            m_aabb.left = -64;
        }
    }

    if (m_aabb.left > cameraPos.x + 320 ||
        m_aabb.left < cameraPos.x - 336 ||
        m_aabb.top > cameraPos.y + 240 ||
        m_aabb.top < cameraPos.y - 256)
        m_aabb.left = -64;

    m_sprite.setPosition(m_aabb.left+6, m_aabb.top+8);
}

void Projectile_Fireball::cast(Vector2f position, bool right)
{
    m_sprite.setPosition(position);

    m_aabb.left = position.x - 6;
    m_aabb.top = position.y - 8;

    if (right)
        m_movedistance = Vector2f(FIREBALL_SPEED, 0);
    else
        m_movedistance = Vector2f(-FIREBALL_SPEED, 0);
}

void Projectile_Fireball::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
