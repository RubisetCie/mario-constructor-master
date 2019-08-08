/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_mushroom.hpp"

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif

using namespace sf;

Sprite_Mushroom::Sprite_Mushroom(const Texture& texture) : Walkable()
{
    m_sprite.setTexture(texture);

    m_movedistance = Vector2f(0, 0);

    m_aabb.width = 32;
    m_aabb.height = 32;
}

void Sprite_Mushroom::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;
}

void Sprite_Mushroom::update()
{
    m_active = true;

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (blockkiller.intersects(m_aabb))
        m_movedistance.y = -4;

    if (player->m_aabb.intersects(m_aabb))
    {
        if (game_powerup == 0)
            player->setPower(1, false);

        m_destroyed = true;

        addPoints(1000);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[5], 0, NULL);
    }
}

void Sprite_Mushroom::secureUpdate()
{
    if (!m_active)
        return;

    if (m_movedistance.x == 0 && player != NULL)
    {
        float playerpos = player->getPosition().x;
        float pos = m_sprite.getPosition().x;

        m_movedistance.x = pos>=playerpos+8?-1.5:1.5;
    }

    Walkable::update();

    if (zoneb)
    {
        if (m_aabb.top > levelScaleb.y * 480 + 64)
            m_destroyed = true;
    }
    else
    {
        if (m_aabb.top > levelScale.y * 480 + 64)
            m_destroyed = true;
    }

    m_sprite.setPosition(roundf(m_aabb.left), roundf(m_aabb.top));
}

void Sprite_Mushroom::afterUpdate()
{
}

Vector2f Sprite_Mushroom::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Sprite_Mushroom::getSize() const
{
    return Vector2f(32, 32);
}

ID Sprite_Mushroom::getID() const
{
    return ID_USELESS;
}

void Sprite_Mushroom::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

