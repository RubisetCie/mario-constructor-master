/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Miscs/effect_coin.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/globals.hpp"

#define COIN_STARTSPEED -4.6
#define COIN_DECELERATION 0.2

using namespace std;
using namespace sf;

Effect_Coin::Effect_Coin(Texture* coinTexture, Texture* vanishTexture, Vector2f position) : Effect(position)
{
    m_sprite.setTexture(*coinTexture);
    m_sprite.setTextureRect(IntRect(0, 0, 21, 28));

    m_vanishtex = vanishTexture;

    m_movedistance = COIN_STARTSPEED;
    m_vanishframe = 12;
}

void Effect_Coin::update()
{
    if (m_movedistance < 1)
    {
        if (m_vanishframe < 15)
            m_vanishframe++;
        else
            m_vanishframe = 0;

        if (m_vanishframe % 4 == 0)
            m_sprite.setTextureRect(IntRect((m_vanishframe/4) * 21, 0, 21, 28));
    }
    else
    {
        m_vanishframe++;

        if (m_vanishframe % 4 == 0)
            m_sprite.setTextureRect(IntRect((m_vanishframe/4) * 37, 0, 37, 32));

        if (m_vanishframe >= 20)
            m_destroyed = true;

        return;
    }

    if (m_movedistance <= 0)
    {
        m_movedistance += COIN_DECELERATION;
        m_sprite.move(0, m_movedistance);
    }
    else
    {
        Vector2f pos(m_sprite.getPosition());

        m_movedistance = 1;
        m_vanishframe = 0;

        m_sprite.move(-8, -2);
        m_sprite.setTexture(*m_vanishtex);
        m_sprite.setTextureRect(IntRect(0, 0, 37, 32));

        listEffect.emplace_back(new Effect_Score(effectTxt[13], 1, Vector2f(pos.x-10, pos.y+56)));
    }
}

void Effect_Coin::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
