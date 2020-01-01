/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Bonuses/bonus_coinblock.hpp"
#include "../../Headers/Miscs/effect_coin.hpp"
#include "../../Headers/Core/effect.hpp"
#include "../../Headers/globals.hpp"

using namespace std;
using namespace sf;

Bonus_CoinBlock::Bonus_CoinBlock(Texture* blockTexture) : Bonus_Block(blockTexture)
{
}

void Bonus_CoinBlock::setPosition(const Vector2f& pos)
{
    Bonus_Block::setPosition(pos);
}

void Bonus_CoinBlock::update()
{
    Bonus_Block::update();
}

void Bonus_CoinBlock::secureUpdate()
{
    Bonus_Block::secureUpdate();
}

void Bonus_CoinBlock::hit()
{
    addCoin();
    addPoints(200);

    Vector2f pos(m_sprite.getPosition());

    listEffect.emplace_back(new Effect_Coin(effectTxt[8], effectTxt[12], Vector2f(pos.x + 6, pos.y - 24)));

    if (pos.x > cameraPos.x - 384 && pos.x < cameraPos.x + 352 && pos.y > cameraPos.y - 304 && pos.y < cameraPos.y + 272)
        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(4), sfxSamples[1], 0, NULL);
}

void Bonus_CoinBlock::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
