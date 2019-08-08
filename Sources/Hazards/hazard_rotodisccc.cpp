/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_rotodisccc.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Hazard_RotodiscCC::Hazard_RotodiscCC(const Texture& texture, Vector2f disk_pos, float speed) : Hazard_Rotodisc(texture, disk_pos, speed)
{
}

void Hazard_RotodiscCC::secureUpdate()
{
    t -= m_speed;

    if (t < -6.283185f)
        t += 6.283185f;

    m_aabb.left = cos(t) * m_radius + m_position.x + 1;
    m_aabb.top = sin(t) * m_radius + m_position.y + 1;

    Hazard_Rotodisc::secureUpdate();
}

void Hazard_RotodiscCC::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
    target.draw(m_disk);
}
