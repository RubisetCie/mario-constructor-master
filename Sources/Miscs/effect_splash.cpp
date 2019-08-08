/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Miscs/effect_splash.hpp"

using namespace sf;

Effect_Splash::Effect_Splash(Texture* texture, Vector2f position) : Effect(position)
{
    m_sprite.setTexture(*texture);
    m_sprite.setTextureRect(IntRect(0, 0, 31, 32));

    m_frame = 0;
}

void Effect_Splash::update()
{
    if (m_frame < 14)
        m_frame++;
    else
        m_destroyed = true;

    m_sprite.setTextureRect(IntRect(m_frame * 31, 0, 31, 32));
}

void Effect_Splash::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
