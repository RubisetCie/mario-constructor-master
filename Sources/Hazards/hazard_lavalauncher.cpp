/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_lavalauncher.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

Hazard_Lavalauncher::Hazard_Lavalauncher(const Texture& texture)
{
    m_sprite.setTexture(texture);

    m_delay = (rand() % 40) + 20;
    m_timer = 0;
}

Hazard_Lavalauncher::~Hazard_Lavalauncher()
{
    for (vector<Projectile_Lava*>::iterator it = m_lava.begin(); it != m_lava.end(); it++)
        delete *it;
}

void Hazard_Lavalauncher::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Hazard_Lavalauncher::update()
{
    if (m_delay > 0)
        m_delay--;

    if (m_delay == 0)
    {
        m_delay = (rand() % 40) + 200;
        m_timer = (rand() % 30) + 60;

        for (vector<Projectile_Lava*>::iterator it = m_lava.begin(); it != m_lava.end(); it++)
            delete *it;

        m_lava.clear();

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(13), sfxSamples[23], 0, NULL);
    }

    if (m_timer > 0)
    {
        if (m_timer % 3 == 0)
            shot();

        m_timer--;
    }
}

void Hazard_Lavalauncher::secureUpdate()
{
    if (!m_lava.empty())
    {
        for (vector<Projectile_Lava*>::iterator it = m_lava.begin(); it != m_lava.end(); it++)
            (*it)->update();
    }
}

void Hazard_Lavalauncher::afterUpdate()
{
    for (vector<Projectile_Lava*>::iterator it = m_lava.begin(); it != m_lava.end(); it++)
        mainTexture.draw(**it);
}

ID Hazard_Lavalauncher::getID() const
{
    return ID_USELESS;
}
