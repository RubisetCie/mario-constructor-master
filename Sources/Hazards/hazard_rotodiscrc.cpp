/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_rotodiscrc.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Hazard_RotodiscRC::Hazard_RotodiscRC(const Texture& texture, Vector2f disk_pos, float speed) : Hazard_Rotodisc(texture, disk_pos, speed)
{
    m_quarterspeed = speed * 1.5707f;

    m_grow = false;
}

void Hazard_RotodiscRC::setPosition(Vector2f pos)
{
    Hazard_Rotodisc::setPosition(pos);

    m_distance = m_radius;
}

void Hazard_RotodiscRC::secureUpdate()
{
    t += m_speed;

    if (t > 6.283185f)
        t -= 6.283185f;

    if (m_grow)
    {
        if (m_distance < m_radius - m_quarterspeed)
            m_distance += m_quarterspeed;
        else
        {
            m_distance = m_radius;
            m_grow = false;
        }
    }
    else
    {
        if (m_distance > m_quarterspeed)
            m_distance -= m_quarterspeed;
        else
        {
            m_distance = 0;
            m_grow = true;
        }
    }

    m_aabb.left = cos(t) * m_distance + m_position.x + 1;
    m_aabb.top = sin(t) * m_distance + m_position.y + 1;

    Hazard_Rotodisc::secureUpdate();
}

void Hazard_RotodiscRC::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
    target.draw(m_disk);
}
