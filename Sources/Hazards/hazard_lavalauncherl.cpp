/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_lavalauncherl.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Hazard_LavalauncherL::Hazard_LavalauncherL(const Texture& texture) : Hazard_Lavalauncher(texture)
{
}

void Hazard_LavalauncherL::shot()
{
    Vector2f position(m_sprite.getPosition());
    m_lava.emplace_back(new Projectile_Lava(hazardsTxt[7], Vector2f(position.x - 21, position.y + 3), Vector2f(rand() / static_cast<float>(RAND_MAX) - 7.0f, rand() / static_cast<float>(RAND_MAX) - 2.0f)));
}

Vector2f Hazard_LavalauncherL::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Hazard_LavalauncherL::getSize() const
{
    return Vector2f(64, 32);
}

void Hazard_LavalauncherL::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
