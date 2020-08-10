/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_spikelauncherb.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Hazard_SpikelauncherB::Hazard_SpikelauncherB(const Texture& texture) : Hazard_Spikelauncher(texture)
{
    m_sprite.setScale(1, -1);
}

void Hazard_SpikelauncherB::shot()
{
    Vector2f position(m_sprite.getPosition());

    m_spike.emplace_back(new Projectile_Spike(hazardsTxt[9], Vector2f(position.x + 29, position.y), Vector2f((rand() / (RAND_MAX / 3.0)) - 1.5f, (rand() / (RAND_MAX / 2.0)) + 4.0f)));
    listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(position.x + 14, position.y - 15)));
}

Vector2f Hazard_SpikelauncherB::getPosition() const
{
    Vector2f position(m_sprite.getPosition());
    return Vector2f(position.x, position.y-40);
}

Vector2f Hazard_SpikelauncherB::getSize() const
{
    return Vector2f(58, 40);
}

void Hazard_SpikelauncherB::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
