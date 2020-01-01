/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/Bonuses/bonus_coinbrick.hpp"
#include "../../Headers/Miscs/effect_coin.hpp"
#include "../../Headers/Core/effect.hpp"
#include "../../Headers/globals.hpp"

using namespace std;
using namespace sf;

Bonus_CoinBrick::Bonus_CoinBrick(Texture* blockTexture, unsigned int coins) : Placeable()
{
    m_sprite.setTexture(*blockTexture);
    m_sprite.setTextureRect(IntRect(0, 0, 32, 32));

    m_hitspeed = 0;
    m_hit = 0;

    m_coins = coins;
}

void Bonus_CoinBrick::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);

    m_startheight = pos.y;
}

void Bonus_CoinBrick::update()
{
    if (m_hit > 0)
        return;

    Vector2f pos(m_sprite.getPosition());
    FloatRect bbox(pos, Vector2f(32, 32));

    if (blockhitter.intersects(bbox))
    {
        m_hitspeed = -3;
        m_hit = 1;

        m_coins--;

        if (m_coins == 0)
            m_sprite.setTextureRect(IntRect(32, 0, 32, 32));

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

        addCoin();
        addPoints(200);

        listEffect.emplace_back(new Effect_Coin(effectTxt[8], effectTxt[12], Vector2f(pos.x + 4, pos.y - 24)));

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(4), sfxSamples[1], 0, NULL);
    }
}

void Bonus_CoinBrick::secureUpdate()
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

            if (m_coins > 0)
                m_hit = 0;
            else
                m_hit = 2;
        }

        return;
    }
    else if (m_hit == 0)
    {
        FloatRect bbox(pos, Vector2f(32, 32));

        if (spritehitter.intersects(bbox))
        {
            m_hitspeed = -3;
            m_hit = 1;

            m_coins--;

            if (m_coins == 0)
                m_sprite.setTextureRect(IntRect(32, 0, 32, 32));

            addCoin();
            addPoints(200);

            listEffect.emplace_back(new Effect_Coin(effectTxt[8], effectTxt[12], Vector2f(pos.x + 6, pos.y - 24)));

            if (bbox.left > cameraPos.x - 384 && bbox.left < cameraPos.x + 352 && bbox.top > cameraPos.y - 304 && bbox.top < cameraPos.y + 272)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(4), sfxSamples[1], 0, NULL);
        }
    }
}

void Bonus_CoinBrick::afterUpdate()
{
}

Vector2f Bonus_CoinBrick::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Bonus_CoinBrick::getSize() const
{
    return Vector2f(32, 32);
}

ID Bonus_CoinBrick::getID() const
{
    return ID_USELESS;
}

void Bonus_CoinBrick::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
