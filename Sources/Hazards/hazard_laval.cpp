/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_laval.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Hazard_LavaL::Hazard_LavaL(const sf::Texture& texture)
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 32, 34));
    m_sprite.setRotation(90);
    m_sprite.setScale(1, -1);
}

void Hazard_LavaL::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Hazard_LavaL::update()
{
    Hazard_Lava::update();

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    Vector2f pos(m_sprite.getPosition());

    if (player->m_aabb.intersects(FloatRect(Vector2f(pos.x + 4, pos.y), Vector2f(30, 32))))
        player->death();
}

Vector2f Hazard_LavaL::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Hazard_LavaL::getSize() const
{
    return Vector2f(34, 32);
}

void Hazard_LavaL::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
