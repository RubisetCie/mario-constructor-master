/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_spikelaunchert.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Hazard_SpikelauncherT::Hazard_SpikelauncherT(const Texture& texture) : Hazard_Spikelauncher(texture)
{
}

void Hazard_SpikelauncherT::shot()
{
    Vector2f position(m_sprite.getPosition());

    m_spike.emplace_back(new Projectile_Spike(hazardsTxt[9], Vector2f(position.x + 29, position.y), Vector2f((rand() / (RAND_MAX / 1.5)) - 3.0f, (rand() / (RAND_MAX / 4.0)) - 12.0f)));
    listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(position.x + 14, position.y - 15)));
}

Vector2f Hazard_SpikelauncherT::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Hazard_SpikelauncherT::getSize() const
{
    return Vector2f(58, 40);
}

void Hazard_SpikelauncherT::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
