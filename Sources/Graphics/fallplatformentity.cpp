/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Graphics/fallplatformentity.hpp"
#include "../../Headers/Graphics/entity.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

FallPlatform_Entity::FallPlatform_Entity(Texture* texture, Texture* platformText, float x, float y, float originx, float originy, float textPos, unsigned int type, bool iscloud) : Entity(texture, x, y, originx, originy, type)
{
    m_platformtext.setTexture(*platformText);
    m_platformtext.setTextureRect(IntRect(211, 0, 53, 16));
    m_platformtext.setPosition(x-textPos, y+16);

    if (iscloud)
        m_sprite.setTextureRect(IntRect(0, 0, 126, 32));
}

Vector2f FallPlatform_Entity::getPosition() const
{
    return m_sprite.getPosition() - m_sprite.getOrigin();
}

Vector2u FallPlatform_Entity::getSize() const
{
    return Vector2u(m_sprite.getGlobalBounds().width, 32);
}

void FallPlatform_Entity::update()
{

}

void FallPlatform_Entity::drawb(RenderTarget* target) const
{
    target->draw(m_platformtext);
}

void FallPlatform_Entity::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
