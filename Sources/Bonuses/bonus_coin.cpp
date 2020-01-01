/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Graphics/spritefont.hpp"
#include "../../Headers/Bonuses/bonus_coin.hpp"
#include "../../Headers/Miscs/effect_coin.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

Bonus_Coin::Bonus_Coin(const Texture& texture) : Placeable()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 19, 28));
}

void Bonus_Coin::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Bonus_Coin::update()
{
    if (frame_Hud % 6 == 0)
        m_sprite.setTextureRect(IntRect((frame_Hud / 6) * 19, 0, 19, 28));

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    Vector2f pos(m_sprite.getPosition());

    if (blockkiller.intersects(FloatRect(pos, Vector2f(19, 32))))
    {
        addCoin();
        addPoints(200);

        listEffect.emplace_back(new Effect_Coin(effectTxt[8], effectTxt[12], Vector2f(pos.x, pos.y + 2)));

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(4), sfxSamples[1], 0, NULL);

        if (game_coins == 0)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[7], 0, NULL);

        m_destroyed = true;
    }

    if (player->m_aabb.intersects(FloatRect(pos - player->m_movedistance, Vector2f(19, 28))))
    {
        addCoin();
        addPoints(200);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(4), sfxSamples[1], 0, NULL);

        if (game_coins == 0)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[7], 0, NULL);

        m_destroyed = true;
    }
}

void Bonus_Coin::secureUpdate()
{
}

void Bonus_Coin::afterUpdate()
{
}

Vector2f Bonus_Coin::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Bonus_Coin::getSize() const
{
    return Vector2f(19, 28);
}

ID Bonus_Coin::getID() const
{
    return ID_USELESS;
}

void Bonus_Coin::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
