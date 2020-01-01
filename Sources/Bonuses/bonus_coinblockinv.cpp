/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Bonuses/bonus_coinblockinv.hpp"
#include "../../Headers/Miscs/effect_coin.hpp"
#include "../../Headers/Core/effect.hpp"
#include "../../Headers/globals.hpp"

using namespace std;
using namespace sf;

Bonus_CoinBlockInv::Bonus_CoinBlockInv(Texture* blockTexture, Collider* collision, char once) : Bonus_BlockInv(blockTexture, collision, once)
{
}

void Bonus_CoinBlockInv::setPosition(const Vector2f& pos)
{
    Bonus_BlockInv::setPosition(pos);
}

void Bonus_CoinBlockInv::update()
{
    Bonus_BlockInv::update();
}

void Bonus_CoinBlockInv::secureUpdate()
{
    Bonus_BlockInv::secureUpdate();
}

void Bonus_CoinBlockInv::hit()
{
    addCoin();
    addPoints(200);

    Vector2f pos(m_sprite.getPosition());

    listEffect.emplace_back(new Effect_Coin(effectTxt[8], effectTxt[12], Vector2f(pos.x + 6, pos.y - 24)));

    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(4), sfxSamples[1], 0, NULL);
}

void Bonus_CoinBlockInv::draw(RenderTarget& target, RenderStates) const
{
    if (m_hit > 0)
        target.draw(m_sprite);
}
