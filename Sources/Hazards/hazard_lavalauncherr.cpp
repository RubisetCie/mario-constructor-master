/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_lavalauncherr.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Hazard_LavalauncherR::Hazard_LavalauncherR(const Texture& texture) : Hazard_Lavalauncher(texture)
{
    m_sprite.setScale(-1, 1);
}

void Hazard_LavalauncherR::shot()
{
    Vector2f position(m_sprite.getPosition());
    m_lava.emplace_back(new Projectile_Lava(hazardsTxt[7], Vector2f(position.x - 3, position.y + 3), Vector2f(rand() / static_cast<float>(RAND_MAX) + 6.0f, rand() / static_cast<float>(RAND_MAX) - 2.0f)));
}

Vector2f Hazard_LavalauncherR::getPosition() const
{
    Vector2f position(m_sprite.getPosition());
    return Vector2f(position.x-64, position.y);
}

Vector2f Hazard_LavalauncherR::getSize() const
{
    return Vector2f(64, 32);
}

void Hazard_LavalauncherR::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
