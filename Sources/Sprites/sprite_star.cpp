/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_star.hpp"

#define STAR_MAXVSPEED 8
#define STAR_BOUNCESPEED 4.5

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_Star::Sprite_Star(const Texture& texture) : Pawn()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 32, 32));

    m_movedistance = Vector2f(0, 0);

    m_aabb.width = 32;
    m_aabb.height = 32;
}

void Sprite_Star::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;
}

void Sprite_Star::update()
{
    m_active = true;

    if (frame_Water % 6 == 0)
        m_sprite.setTextureRect(IntRect((frame_Water/6) * 32, 0, 32, 32));

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (player->m_aabb.intersects(m_aabb))
    {
        m_destroyed = true;

        player->m_invincibility = 620;

        killcount = 0;

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[5], 0, NULL);

        if (koopaEngaged == 0)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), musicSamples[26], 0, &musicChannel);
    }
}

void Sprite_Star::secureUpdate()
{
    if (!m_active)
        return;

    if (m_movedistance.x == 0 && player != NULL)
    {
        float playerpos = player->getPosition().x;
        float pos = m_sprite.getPosition().x;

        m_movedistance.x = pos>=playerpos+8?-2:2;
    }

    if (m_movedistance.y < STAR_MAXVSPEED - star_gravity)
        m_movedistance.y += star_gravity;

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

    if (makeMoveHorizontal(m_movedistance.x, *currentScale, currentMatrix, currentList))
        m_movedistance.x = -m_movedistance.x;

    if (makeMoveVertical(m_movedistance.y, *currentScale, currentMatrix, currentList))
    {
        if (m_movedistance.y > 0)
            m_movedistance.y = -STAR_BOUNCESPEED;
        else
            m_movedistance.y = 0;
    }

    if (m_aabb.top > currentScale->y * 480 + 64)
        m_destroyed = true;

    m_sprite.setPosition(roundf(m_aabb.left), roundf(m_aabb.top));
}

void Sprite_Star::afterUpdate()
{
}

Vector2f Sprite_Star::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Sprite_Star::getSize() const
{
    return Vector2f(32, 32);
}

ID Sprite_Star::getID() const
{
    return ID_USELESS;
}

void Sprite_Star::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

