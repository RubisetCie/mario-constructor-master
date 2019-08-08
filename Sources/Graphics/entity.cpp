/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Graphics/entity.hpp"

using namespace sf;
using namespace std;

Entity::Entity(Texture* texture, float x, float y, float originx, float originy, unsigned int type)
{
    m_sprite.setTexture(*texture);
    m_sprite.setPosition(x, y);
    m_sprite.setOrigin(originx, originy);

    m_enttype = type;
}

Vector2f Entity::getPosition() const
{
    return m_sprite.getPosition() - m_sprite.getOrigin();
}

Vector2f Entity::getOriginalPosition() const
{
    return m_sprite.getPosition();
}

Vector2u Entity::getSize() const
{
    FloatRect globalBnds(m_sprite.getGlobalBounds());

    return Vector2u(globalBnds.width, globalBnds.height);
}

unsigned int Entity::getType() const
{
    return m_enttype;
}

void Entity::roomResized(Vector2i newsize)
{
}

void Entity::save(vector<float>& entData)
{
    Vector2f position(m_sprite.getPosition());

    entData.emplace_back(position.x);
    entData.emplace_back(position.y);
}

void Entity::load(vector<float>& entData)
{

}

void Entity::update()
{

}

void Entity::drawb(RenderTarget* target) const
{

}

void Entity::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
