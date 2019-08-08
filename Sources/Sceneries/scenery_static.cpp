/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Sceneries/scenery_static.hpp"

using namespace sf;

Scenery_Static::Scenery_Static(const Texture& texture, const IntRect& rect) : Placeable()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(rect);
}

void Scenery_Static::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);
}

void Scenery_Static::update()
{
}

void Scenery_Static::secureUpdate()
{
}

void Scenery_Static::afterUpdate()
{
}

Vector2f Scenery_Static::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Scenery_Static::getSize() const
{
    FloatRect bbox(m_sprite.getGlobalBounds());

    return Vector2f(bbox.width, bbox.height);
}

ID Scenery_Static::getID() const
{
    return ID_USELESS;
}

void Scenery_Static::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
