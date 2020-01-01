/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Sceneries/scenery_waterfall.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Scenery_Waterfall::Scenery_Waterfall(const Texture& texture) : Placeable()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 95, 91));
}

void Scenery_Waterfall::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Scenery_Waterfall::update()
{
    if (frame_Water % 6 == 0)
        m_sprite.setTextureRect(IntRect((frame_Water/6) * 95, 0, 95, 91));
}

void Scenery_Waterfall::secureUpdate()
{
}

void Scenery_Waterfall::afterUpdate()
{
}

Vector2f Scenery_Waterfall::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Scenery_Waterfall::getSize() const
{
    FloatRect bbox(m_sprite.getGlobalBounds());

    return Vector2f(bbox.width, bbox.height);
}

ID Scenery_Waterfall::getID() const
{
    return ID_USELESS;
}

void Scenery_Waterfall::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
