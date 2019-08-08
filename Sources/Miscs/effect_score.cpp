/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Miscs/effect_score.hpp"

#define SCORE_SPEED -1

using namespace sf;

Effect_Score::Effect_Score(Texture* scoreTexture, unsigned int value, Vector2f position) : Effect(position)
{
    m_sprite.setTexture(*scoreTexture);
    m_sprite.setTextureRect(IntRect(value * 41, 0, 41, 16));

    m_vanishcounter = 0;
}

void Effect_Score::update()
{
    m_vanishcounter++;

    if (m_vanishcounter < 40)
        m_sprite.move(0, SCORE_SPEED);

    if (m_vanishcounter == 150)
        m_destroyed = true;
}

void Effect_Score::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
