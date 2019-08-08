/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Hazards/hazard_podobool.hpp"
#include "../../Headers/Miscs/effect_lavasplash.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define PODOBOO_MAXVSPEED 12

using namespace std;
using namespace sf;

Hazard_PodobooL::Hazard_PodobooL(const Texture& texture, float maxheight) : Hazard_Podoboo(texture, maxheight)
{
    m_sprite.setOrigin(0, 16);
    m_sprite.setRotation(90);

    m_aabb.width = 32;
    m_aabb.height = 27;
}

void Hazard_PodobooL::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos.x + 16, pos.y);

    m_aabb.left = pos.x;
    m_aabb.top = pos.y;

    m_startheight = pos.x;

    float const delta(m_startheight - m_maxheight);
    if (delta < 208)
    {
        m_speed = sqrt(delta * 0.7142857);

        if (m_speed < 4.0f)
            m_speed = 4.0f;
    }
}

void Hazard_PodobooL::secureUpdate()
{
    if (m_timer == 70)
    {
        m_sprite.setScale(1, -1);

        m_movedistance = -m_speed;
    }
    else if (m_timer == 69)
    {
        if (m_movedistance < PODOBOO_MAXVSPEED - 0.3571428)
            m_movedistance += 0.3571428;

        if (m_movedistance > 0)
            m_sprite.setScale(1, 1);
    }
    else if (m_timer > 0)
        m_timer--;

    m_aabb.left += m_movedistance;

    if (m_timer == 70)
    {
        if (m_aabb.left < m_maxheight + 208)
            m_timer = 69;
    }
    else if (m_timer == 69)
    {
        if (m_aabb.left > m_startheight)
        {
            m_timer = 68;

            m_movedistance = 0;
            m_aabb.left = m_startheight;

            listEffect.emplace_back(new Effect_LavaSplash(effectTxt[11], Vector2f(m_startheight + 6, m_aabb.top - 23), Vector2f(1, -1), 90));
        }
    }

    m_sprite.setPosition(m_aabb.left + 16, m_aabb.top);
}

Vector2f Hazard_PodobooL::getSize() const
{
    return Vector2f(32, 27);
}

void Hazard_PodobooL::draw(RenderTarget& target, RenderStates) const
{
    if (m_timer >= 68 && m_timer <= 70)
        target.draw(m_sprite);
}
