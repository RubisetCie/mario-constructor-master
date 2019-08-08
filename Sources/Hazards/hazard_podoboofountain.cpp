/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <vector>
#include "../../Headers/Hazards/hazard_podoboofountain.hpp"
#include "../../Headers/globals.hpp"

using namespace std;
using namespace sf;

Hazard_PodobooFountain::Hazard_PodobooFountain() : Placeable()
{
    m_timer = 0;
}

Hazard_PodobooFountain::~Hazard_PodobooFountain()
{
    for (list<Projectile_Podoboo*>::iterator it = m_podoboo.begin(); it != m_podoboo.end(); it++)
        delete *it;
}

void Hazard_PodobooFountain::setPosition(Vector2f pos)
{
    m_position = pos;
}

void Hazard_PodobooFountain::update()
{
    if (m_timer == 0)
        m_timer = 320;
}

void Hazard_PodobooFountain::secureUpdate()
{
    if (!m_podoboo.empty())
    {
        vector<list<Projectile_Podoboo*>::iterator> toDestroy;

        for (list<Projectile_Podoboo*>::iterator it = m_podoboo.begin(); it != m_podoboo.end(); it++)
        {
            (*it)->update();

            mainTexture.draw(**it);

            if ((*it)->m_destroyed)
                toDestroy.emplace_back(it);
        }

        for (vector<list<Projectile_Podoboo*>::iterator>::iterator itb = toDestroy.begin(); itb != toDestroy.end(); itb++)
        {
            delete **itb;
            m_podoboo.erase(*itb);
        }
    }
}

void Hazard_PodobooFountain::afterUpdate()
{
}

Vector2f Hazard_PodobooFountain::getPosition() const
{
    return m_position;
}

Vector2f Hazard_PodobooFountain::getSize() const
{
    return Vector2f(32, 32);
}

ID Hazard_PodobooFountain::getID() const
{
    return ID_USELESS;
}
