/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Graphics/entityj.hpp"

using namespace sf;

Entityj::Entityj(sf::Texture* texture, sf::Texture* textText, float x, float y, float originx, float originy, unsigned int type, float angle, bool ispodoboo) : Entity(texture, x, y, originx, originy, type)
{
    m_sprite.setRotation(angle);

    if (ispodoboo)
        m_sprite.setTextureRect(IntRect(0, 0, 27, 32));
    else
        m_sprite.setTextureRect(IntRect(0, 0, 32, 47));

    m_spriteb.setTexture(*textText);
    m_spriteb.setTextureRect(IntRect(264, 0, 15, 16));
    m_spriteb.setPosition(x + 16, y + 16);
}

void Entityj::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
    target.draw(m_spriteb);
}
