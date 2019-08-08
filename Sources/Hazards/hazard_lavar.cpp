/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_lavar.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Hazard_LavaR::Hazard_LavaR(const sf::Texture& texture)
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 32, 34));
    m_sprite.setRotation(90);
}

void Hazard_LavaR::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);
}

void Hazard_LavaR::update()
{
    Hazard_Lava::update();

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    Vector2f pos(m_sprite.getPosition());

    if (player->m_aabb.intersects(FloatRect(Vector2f(pos.x - 34, pos.y), Vector2f(30, 32))))
        player->death();
}

Vector2f Hazard_LavaR::getPosition() const
{
    Vector2f pos(m_sprite.getPosition());
    return Vector2f(pos.x - 34, pos.y);
}

Vector2f Hazard_LavaR::getSize() const
{
    return Vector2f(34, 32);
}

void Hazard_LavaR::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
