/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Miscs/effect_lavasplash.hpp"

using namespace sf;

Effect_LavaSplash::Effect_LavaSplash(Texture* texture, Vector2f position, Vector2f size, float angle) : Effect(position)
{
    m_sprite.setTexture(*texture);
    m_sprite.setTextureRect(IntRect(0, 0, 31, 32));
    m_sprite.setScale(size);
    m_sprite.setRotation(angle);

    m_frame = 0;
}

void Effect_LavaSplash::update()
{
    if (m_frame < 21)
        m_frame++;
    else
        m_destroyed = true;

    if (m_frame % 2 == 0)
        m_sprite.setTextureRect(IntRect((m_frame / 2) * 73, 0, 73, 22));
}

void Effect_LavaSplash::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
