/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_life.hpp"
#include "../../Headers/Miscs/effect_score.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_Life::Sprite_Life(const Texture& texture) : Walkable()
{
    m_sprite.setTexture(texture);

    m_movedistance = Vector2f(0, 0);

    m_aabb.width = 32;
    m_aabb.height = 32;
}

void Sprite_Life::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;
}

void Sprite_Life::update()
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
        addLife();

        m_destroyed = true;

        listEffect.emplace_back(new Effect_Score(effectTxt[13], 7, Vector2f(m_aabb.left-5, m_aabb.top + 16)));

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[7], 0, NULL);
    }
}

void Sprite_Life::secureUpdate()
{
    if (!m_active)
        return;

    if (m_movedistance.x == 0 && player != NULL)
    {
        float playerpos = player->getPosition().x;
        float pos = m_sprite.getPosition().x;

        m_movedistance.x = pos>=playerpos+8?-2.2:2.2;
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

void Sprite_Life::afterUpdate()
{
}

Vector2f Sprite_Life::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Sprite_Life::getSize() const
{
    return Vector2f(32, 32);
}

ID Sprite_Life::getID() const
{
    return ID_USELESS;
}

void Sprite_Life::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

