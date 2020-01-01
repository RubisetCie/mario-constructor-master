/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Sceneries/scenery_cloud.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Scenery_Cloud::Scenery_Cloud(const Texture& texture) : Placeable()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 63, 48));
}

void Scenery_Cloud::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Scenery_Cloud::update()
{
    if (frame_Clouds % 10 == 0)
        m_sprite.setTextureRect(IntRect((frame_Clouds/10) * 63, 0, 63, 48));
}

void Scenery_Cloud::secureUpdate()
{
}

void Scenery_Cloud::afterUpdate()
{
}

Vector2f Scenery_Cloud::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Scenery_Cloud::getSize() const
{
    FloatRect bbox(m_sprite.getGlobalBounds());

    return Vector2f(bbox.width, bbox.height);
}

ID Scenery_Cloud::getID() const
{
    return ID_USELESS;
}

void Scenery_Cloud::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
