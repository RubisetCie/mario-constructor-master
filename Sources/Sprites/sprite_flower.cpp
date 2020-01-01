/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_flower.hpp"

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif

using namespace sf;

Sprite_Flower::Sprite_Flower(const Texture& texture) : Fallable()
{
    m_movedistance = 0;

    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 32, 32));

    m_aabb.width = 32;
    m_aabb.height = 32;
}

void Sprite_Flower::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;
}

void Sprite_Flower::update()
{
    if (frame_Flower % 4 == 0)
        m_sprite.setTextureRect(IntRect((frame_Flower/4) * 32, 0, 32, 32));

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (blockkiller.intersects(m_aabb) && m_movedistance == 0)
        m_movedistance = -4;

    if (player->m_aabb.intersects(m_aabb))
    {
        if (game_powerup == 0)
            player->setPower(1, false);
        else if (game_powerup != 2)
            player->setPower(2, false);

        m_destroyed = true;

        addPoints(1000);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[5], 0, NULL);
    }
}

void Sprite_Flower::secureUpdate()
{
    Fallable::update();

    m_sprite.setPosition(roundf(m_aabb.left), roundf(m_aabb.top));
}

void Sprite_Flower::afterUpdate()
{
}

Vector2f Sprite_Flower::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Sprite_Flower::getSize() const
{
    return Vector2f(32, 32);
}

ID Sprite_Flower::getID() const
{
    return ID_USELESS;
}

void Sprite_Flower::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

