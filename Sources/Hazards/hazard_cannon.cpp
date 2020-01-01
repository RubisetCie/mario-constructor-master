/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <vector>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_cannon.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

Hazard_Cannon::Hazard_Cannon(const Texture& texture)
{
    m_sprite.setTexture(texture);

    m_timer = (rand() % 20) + 60;
}

Hazard_Cannon::~Hazard_Cannon()
{
    for (list<Projectile_Bulletbill*>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
        delete *it;
}

void Hazard_Cannon::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Hazard_Cannon::update()
{
    if (m_timer > 0)
        m_timer--;

    if (m_timer == 0)
    {
        if (player != NULL)
        {
            Vector2f position(m_sprite.getPosition());

            if (player->m_aabb.left > position.x - 34 && player->m_aabb.left < position.x + 38)
                m_timer = 15;
            else
            {
                shot();

                m_timer = (rand() % 20) + 180;

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(12), sfxSamples[20], 0, NULL);
            }
        }
        else
        {
            shot();

            m_timer = (rand() % 20) + 180;

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(12), sfxSamples[20], 0, NULL);
        }
    }
}

void Hazard_Cannon::secureUpdate()
{
    if (!m_bullets.empty())
    {
        vector<list<Projectile_Bulletbill*>::iterator> toDestroy;

        for (list<Projectile_Bulletbill*>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
        {
            (*it)->update();

            if ((*it)->m_destroyed)
                toDestroy.emplace_back(it);
        }

        for (vector<list<Projectile_Bulletbill*>::iterator>::iterator itb = toDestroy.begin(); itb != toDestroy.end(); itb++)
        {
            delete **itb;
            m_bullets.erase(*itb);
        }
    }
}

void Hazard_Cannon::afterUpdate()
{
    for (list<Projectile_Bulletbill*>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
        mainTexture.draw(**it);
}

ID Hazard_Cannon::getID() const
{
    return ID_USELESS;
}
