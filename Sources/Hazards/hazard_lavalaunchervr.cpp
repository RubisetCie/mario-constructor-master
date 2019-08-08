/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_lavalaunchervr.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Hazard_LavalauncherVR::Hazard_LavalauncherVR(const Texture& texture) : Hazard_Lavalauncher(texture)
{
    m_sprite.setScale(-1, 1);
}

void Hazard_LavalauncherVR::shot()
{
    Vector2f position(m_sprite.getPosition());
    m_lava.emplace_back(new Projectile_Lava(hazardsTxt[7], Vector2f(position.x - 20, position.y - 14), Vector2f((rand() / (RAND_MAX / 2.0)) + 4.0f, (rand() / (RAND_MAX / 2.0)) - 6.0f)));
}

Vector2f Hazard_LavalauncherVR::getPosition() const
{
    Vector2f position(m_sprite.getPosition());
    return Vector2f(position.x-56, position.y);
}

Vector2f Hazard_LavalauncherVR::getSize() const
{
    return Vector2f(56, 69);
}

void Hazard_LavalauncherVR::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
