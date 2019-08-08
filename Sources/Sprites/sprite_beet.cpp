/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_beet.hpp"

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif

using namespace sf;

Sprite_Beet::Sprite_Beet(const Texture& texture) : Fallable()
{
    m_movedistance = 0;

    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 27, 34));

    m_aabb.width = 27;
    m_aabb.height = 32;

    m_frame = 0;
}

void Sprite_Beet::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;
}

void Sprite_Beet::update()
{
    if (m_frame < 39)
        m_frame++;
    else
        m_frame = 0;

    if (m_frame % 10 == 0)
    {
        if (m_frame == 30)
            m_sprite.setTextureRect(IntRect(27, 0, 27, 34));
        else
            m_sprite.setTextureRect(IntRect((m_frame/10) * 27, 0, 27, 34));
    }

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (blockkiller.intersects(m_aabb) && m_movedistance == 0)
        m_movedistance = -4;

    if (player->m_aabb.intersects(m_aabb))
    {
        unsigned char poww = game_powerup;

        if (poww == 0)
            player->setPower(1, false);
        else if (poww != 3)
            player->setPower(3, false);

        m_destroyed = true;

        addPoints(1000);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[5], 0, NULL);
    }
}

void Sprite_Beet::secureUpdate()
{
    Fallable::update();

    m_sprite.setPosition(roundf(m_aabb.left), roundf(m_aabb.top-2));
}

void Sprite_Beet::afterUpdate()
{
}

Vector2f Sprite_Beet::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Sprite_Beet::getSize() const
{
    return Vector2f(27, 34);
}

ID Sprite_Beet::getID() const
{
    return ID_USELESS;
}

void Sprite_Beet::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

