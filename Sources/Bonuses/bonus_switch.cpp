/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Bonuses/bonus_switch.hpp"

using namespace std;
using namespace sf;

Bonus_Switch::Bonus_Switch(Texture* blockTexture, unsigned char slot) : Placeable()
{
    m_sprite.setTexture(*blockTexture);
    m_sprite.setTextureRect(IntRect(slot * 32, 0, 32, 32));

    m_hitspeed = 0;
    m_hit = 0;

    m_slot = slot;
}

void Bonus_Switch::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);

    m_startheight = pos.y;
}

void Bonus_Switch::update()
{
    if (m_hit > 0)
        return;

    FloatRect bbox(m_sprite.getPosition(), Vector2f(32, 32));

    if (blockhitter.intersects(bbox))
    {
        m_hitspeed = -3;
        m_hit = 1;

        m_sprite.setTextureRect(IntRect(m_sprite.getTextureRect().left, 32, 32, 32));

        if (blockkiller_create)
        {
            blockkiller.left = bbox.left;
            blockkiller.width = 32;
            blockkiller.top = bbox.top - 1;
            blockkiller_create = false;
        }
        else
        {
            blockkiller.left = floor(blockhitter.left / 32.0) * 32.0;
            blockkiller.width = ceil((blockhitter.left + 28) / 32.0) * 32.0 - blockkiller.left;
            blockkiller.top = bbox.top - 1;
        }

        // Switching the slots :
        if (game_toggleSolidity[m_slot] == 0)
            game_toggleSolidity[m_slot] = 3;
        else if (game_toggleSolidity[m_slot] == 2)
            game_toggleSolidity[m_slot] = 1;

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(4), sfxSamples[34], 0, NULL);
    }
}

void Bonus_Switch::secureUpdate()
{
    Vector2f pos(m_sprite.getPosition());

    if (m_hit == 1)
    {
        if (pos.y <= m_startheight - m_hitspeed)
        {
            m_hitspeed += 0.4;
            m_sprite.move(0, m_hitspeed);
        }
        else
        {
            m_sprite.setPosition(pos.x, m_startheight);
            m_sprite.setTextureRect(IntRect(m_sprite.getTextureRect().left, 0, 32, 32));

            m_hit = 0;
        }

        return;
    }
    /*else if (m_hit == 0)
    {
        FloatRect bbox(pos, Vector2f(32, 32));

        if (spritehitter.intersects(bbox))
        {
            m_hitspeed = -3;
            m_hit = 1;

            // Switching the slots :
            if (game_toggleSolidity[m_slot] == 0)
                game_toggleSolidity[m_slot] = 3;
            else if (game_toggleSolidity[m_slot] == 2)
                game_toggleSolidity[m_slot] = 1;

            if (bbox.left > cameraPos.x - 384 && bbox.left < cameraPos.x + 352 && bbox.top > cameraPos.y - 304 && bbox.top < cameraPos.y + 272)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(4), sfxSamples[34], 0, NULL);
        }
    }*/
}

void Bonus_Switch::afterUpdate()
{
}

Vector2f Bonus_Switch::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Bonus_Switch::getSize() const
{
    return Vector2f(32, 32);
}

ID Bonus_Switch::getID() const
{
    return ID_USELESS;
}

void Bonus_Switch::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
