/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Graphics/entitymultitext.hpp"

using namespace sf;

Entity_MultiText::Entity_MultiText(Texture* texture, IntRect textureRect, float x, float y, float originx, float originy, unsigned int type, unsigned short layer, float angle) : Entity(texture, x, y, originx, originy, type)
{
    m_sprite.setTextureRect(textureRect);
    m_sprite.setRotation(angle);
}

Vector2f Entity_MultiText::getPosition() const
{
    FloatRect bbox(m_sprite.getGlobalBounds());

    return Vector2f(bbox.left, bbox.top);
}
