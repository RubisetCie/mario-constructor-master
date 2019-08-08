/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_lavab.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Hazard_LavaB::Hazard_LavaB(const sf::Texture& texture)
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 32, 34));
    m_sprite.setScale(1, -1);
}

void Hazard_LavaB::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);
}

void Hazard_LavaB::update()
{
    Hazard_Lava::update();

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    Vector2f pos(m_sprite.getPosition());

    if (player->m_aabb.intersects(FloatRect(Vector2f(pos.x, pos.y - 34), Vector2f(32, 30))))
        player->death();
}

Vector2f Hazard_LavaB::getPosition() const
{
    Vector2f pos(m_sprite.getPosition());
    return Vector2f(pos.x, pos.y - 34);
}

Vector2f Hazard_LavaB::getSize() const
{
    return Vector2f(32, 34);
}

void Hazard_LavaB::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
