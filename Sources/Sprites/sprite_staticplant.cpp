/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif // DEBUGMODE

#include "../../Headers/Sprites/sprite_staticplant.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Sprite_StaticPlant::Sprite_StaticPlant(const Vector2f& pos, float angle) : m_mainPos(pos)
{
    m_sprite.setTexture(*enemiesTxt[41]);
    m_sprite.setTextureRect(IntRect(0, 0, 31, 31));
    m_sprite.setRotation(angle);
}

void Sprite_StaticPlant::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Sprite_StaticPlant::update()
{
    if (frame_Flower % 4 == 0)
    {
        if (frame_Flower == 12)
            m_sprite.setTextureRect(IntRect(31, 0, 31, 31));
        else
            m_sprite.setTextureRect(IntRect((frame_Flower / 4) * 31, 0, 31, 31));
    }

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

    if (player->m_aabb.intersects(FloatRect(m_mainPos, Vector2f(32, 32))))
    {
        if (game_powerup == 0)
            player->death();
        else if (game_powerup == 1)
        {
            player->setPower(0, true);

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[6], 0, NULL);
        }
        else
        {
            player->setPower(1, true);

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[6], 0, NULL);
        }
    }
}

void Sprite_StaticPlant::secureUpdate()
{
}

void Sprite_StaticPlant::afterUpdate()
{
}

Vector2f Sprite_StaticPlant::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Sprite_StaticPlant::getSize() const
{
    return Vector2f(31, 31);
}

ID Sprite_StaticPlant::getID() const
{
    return ID_USELESS;
}

void Sprite_StaticPlant::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
