/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Sceneries/scenery_scroll.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Scenery_Scroll::Scenery_Scroll(const Texture& texture) : Placeable()
{
    m_sprite.setTexture(texture);
}

void Scenery_Scroll::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Scenery_Scroll::update()
{
    m_sprite.setTextureRect(IntRect(0, -effectLavafallPos, 54, 64));
}

void Scenery_Scroll::secureUpdate()
{
}

void Scenery_Scroll::afterUpdate()
{
}

Vector2f Scenery_Scroll::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Scenery_Scroll::getSize() const
{
    FloatRect bbox(m_sprite.getGlobalBounds());

    return Vector2f(bbox.width, bbox.height);
}

ID Scenery_Scroll::getID() const
{
    return ID_USELESS;
}

void Scenery_Scroll::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
