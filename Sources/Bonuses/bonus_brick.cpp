/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/Bonuses/bonus_brick.hpp"
#include "../../Headers/Miscs/effect_shard.hpp"
#include "../../Headers/globals.hpp"

using namespace std;
using namespace sf;

Bonus_Brick::Bonus_Brick(Texture* brickTexture) : Placeable()
{
    m_sprite.setTexture(*brickTexture);
    m_sprite.setTextureRect(IntRect(0, 0, 32, 32));

    m_hitspeed = 0;
    m_hit = 0;
}

void Bonus_Brick::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);

    m_startheight = pos.y;
}

void Bonus_Brick::update()
{
    if (m_hit > 0)
        return;

    FloatRect bbox(m_sprite.getPosition(), Vector2f(32, 32));

    if (blockhitter.intersects(bbox))
    {
        if (game_powerup == 0)
        {
            m_hit = 1;
            m_hitspeed = -3;

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(6), sfxSamples[2], 0, NULL);
        }
        else
        {
            if (zoneb)
                collisionMatrixb->setValue(bbox.left / 32, bbox.top / 32, 0);
            else
                collisionMatrix->setValue(bbox.left / 32, bbox.top / 32, 0);

            m_destroyed = true;

            addPoints(50);

            listEffect.emplace_back(new Effect_Shard(effectTxt[14], Vector2f(-2, -7.5), Vector2f(bbox.left + 8, bbox.top + 8), 0, bbox.top + 32));
            listEffect.emplace_back(new Effect_Shard(effectTxt[14], Vector2f(-4, -5.5), Vector2f(bbox.left + 8, bbox.top + 24), 0, bbox.top + 32));
            listEffect.emplace_back(new Effect_Shard(effectTxt[14], Vector2f(2, -7.5), Vector2f(bbox.left + 24, bbox.top + 8), 90, bbox.top + 32));
            listEffect.emplace_back(new Effect_Shard(effectTxt[14], Vector2f(4, -5.5), Vector2f(bbox.left + 24, bbox.top + 24), 90, bbox.top + 32));

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(7), sfxSamples[3], 0, NULL);
        }

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
    }
}

void Bonus_Brick::secureUpdate()
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
            m_hit = 0;
        }

        return;
    }
    else if (m_hit == 0)
    {
        FloatRect bbox(pos, Vector2f(32, 32));

        if (spritehitter.intersects(bbox))
        {
            if (zoneb)
                collisionMatrixb->setValue(bbox.left / 32, bbox.top / 32, 0);
            else
                collisionMatrix->setValue(bbox.left / 32, bbox.top / 32, 0);

            m_destroyed = true;

            addPoints(50);

            listEffect.emplace_back(new Effect_Shard(effectTxt[14], Vector2f(-2, -7.5), Vector2f(bbox.left + 8, bbox.top + 8), 0, bbox.top + 32));
            listEffect.emplace_back(new Effect_Shard(effectTxt[14], Vector2f(-4, -5.5), Vector2f(bbox.left + 8, bbox.top + 24), 0, bbox.top + 32));
            listEffect.emplace_back(new Effect_Shard(effectTxt[14], Vector2f(2, -7.5), Vector2f(bbox.left + 24, bbox.top + 8), 90, bbox.top + 32));
            listEffect.emplace_back(new Effect_Shard(effectTxt[14], Vector2f(4, -5.5), Vector2f(bbox.left + 24, bbox.top + 24), 90, bbox.top + 32));

            if (bbox.left > cameraPos.x - 384 && bbox.left < cameraPos.x + 352 && bbox.top > cameraPos.y - 304 && bbox.top < cameraPos.y + 272)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(7), sfxSamples[3], 0, NULL);
        }
    }
}

void Bonus_Brick::afterUpdate()
{
}

Vector2f Bonus_Brick::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Bonus_Brick::getSize() const
{
    return Vector2f(32, 32);
}

ID Bonus_Brick::getID() const
{
    return ID_USELESS;
}

void Bonus_Brick::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
