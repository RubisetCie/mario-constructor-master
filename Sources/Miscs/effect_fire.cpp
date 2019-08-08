/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Effect_Fire::Effect_Fire(Texture* texture, Vector2f position) : Effect(position)
{
    m_sprite.setTexture(*texture);
    m_sprite.setTextureRect(IntRect(0, 0, 31, 30));

    m_frame = 0;
}

void Effect_Fire::update()
{
    if (m_frame < 14)
        m_frame++;
    else
        m_destroyed = true;

    if (m_frame % 5 == 0)
        m_sprite.setTextureRect(IntRect((m_frame/5) * 31, 0, 31, 30));
}

void Effect_Fire::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
