/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Sceneries/scenery_bush.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Scenery_Bush::Scenery_Bush(const Texture& texture) : Placeable()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 65, 30));
}

void Scenery_Bush::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Scenery_Bush::update()
{
    if (frame_Water % 8 == 0)
        m_sprite.setTextureRect(IntRect((frame_Water/8) * 65, 0, 65, 30));
}

void Scenery_Bush::secureUpdate()
{
}

void Scenery_Bush::afterUpdate()
{
}

Vector2f Scenery_Bush::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Scenery_Bush::getSize() const
{
    FloatRect bbox(m_sprite.getGlobalBounds());

    return Vector2f(bbox.width, bbox.height);
}

ID Scenery_Bush::getID() const
{
    return ID_USELESS;
}

void Scenery_Bush::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
