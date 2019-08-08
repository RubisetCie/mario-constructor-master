/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_poison.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif

using namespace sf;

Sprite_Poison::Sprite_Poison(const Texture& texture) : Walkable()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 32, 32));

    m_movedistance = Vector2f(0, 0);

    m_aabb.width = 32;
    m_aabb.height = 32;
}

void Sprite_Poison::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;
}

void Sprite_Poison::update()
{
    m_active = true;

    if (frame_Water % 12 == 0)
        m_sprite.setTextureRect(IntRect((frame_Water/12) * 32, 0, 32, 32));

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (player->m_animpowerup > 0)
        return;

    if (game_powerup <= 1 && player->m_animtrace > 0)
        return;

    if (player->m_invincibility > 0)
        return;

    if (blockkiller.intersects(m_aabb))
        m_movedistance.y = -4;

    if (player->m_aabb.intersects(m_aabb))
    {
        m_destroyed = true;

        listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left, m_aabb.top)));

        player->death();
    }
}

void Sprite_Poison::secureUpdate()
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

void Sprite_Poison::afterUpdate()
{
}

Vector2f Sprite_Poison::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Sprite_Poison::getSize() const
{
    return Vector2f(32, 32);
}

ID Sprite_Poison::getID() const
{
    return ID_USELESS;
}

void Sprite_Poison::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

