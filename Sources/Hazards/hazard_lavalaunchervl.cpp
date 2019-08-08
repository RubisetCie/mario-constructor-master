/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_lavalaunchervl.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Hazard_LavalauncherVL::Hazard_LavalauncherVL(const Texture& texture) : Hazard_Lavalauncher(texture)
{
}

void Hazard_LavalauncherVL::shot()
{
    Vector2f position(m_sprite.getPosition());
    m_lava.emplace_back(new Projectile_Lava(hazardsTxt[7], Vector2f(position.x - 4, position.y - 14), Vector2f((rand() / (RAND_MAX / 2.0)) - 6.0f, (rand() / (RAND_MAX / 2.0)) - 6.0f)));
}

Vector2f Hazard_LavalauncherVL::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Hazard_LavalauncherVL::getSize() const
{
    return Vector2f(56, 69);
}

void Hazard_LavalauncherVL::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
