/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_spikelauncher.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

Hazard_Spikelauncher::Hazard_Spikelauncher(const Texture& texture)
{
    m_sprite.setTexture(texture);

    m_timer = (rand() % 35) + 30;
}

Hazard_Spikelauncher::~Hazard_Spikelauncher()
{
    for (list<Projectile_Spike*>::iterator it = m_spike.begin(); it != m_spike.end(); it++)
        delete *it;
}

void Hazard_Spikelauncher::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);
}

void Hazard_Spikelauncher::update()
{
    if (m_timer > 0)
        m_timer--;

    if (m_timer == 0)
    {
        shot();

        m_timer = (rand() % 50) + 205;

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(12), sfxSamples[22], 0, NULL);
    }
}

void Hazard_Spikelauncher::secureUpdate()
{
    if (!m_spike.empty())
    {
        vector<list<Projectile_Spike*>::iterator> toDestroy;

        for (list<Projectile_Spike*>::iterator it = m_spike.begin(); it != m_spike.end(); it++)
        {
            (*it)->update();

            if ((*it)->m_destroyed)
                toDestroy.emplace_back(it);
        }

        for (vector<list<Projectile_Spike*>::iterator>::iterator itb = toDestroy.begin(); itb != toDestroy.end(); itb++)
        {
            delete **itb;
            m_spike.erase(*itb);
        }
    }
}

void Hazard_Spikelauncher::afterUpdate()
{
    for (list<Projectile_Spike*>::iterator it = m_spike.begin(); it != m_spike.end(); it++)
        mainTexture.draw(**it);
}

ID Hazard_Spikelauncher::getID() const
{
    return ID_USELESS;
}
