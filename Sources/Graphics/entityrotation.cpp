/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Graphics/entityrotation.hpp"

using namespace sf;

Entity_Rotation::Entity_Rotation(Texture* texture, float x, float y, float angle, float originx, float originy, unsigned int type) : Entity(texture, x, y, originx, originy, type)
{
    m_sprite.setRotation(angle);
}

Vector2f Entity_Rotation::getPosition() const
{
    FloatRect bbox(m_sprite.getGlobalBounds());

    return Vector2f(bbox.left, bbox.top);
}
