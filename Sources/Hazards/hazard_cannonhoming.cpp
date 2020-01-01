/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <vector>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_cannonhoming.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

Hazard_CannonHoming::Hazard_CannonHoming(const Texture& texture)
{
    m_sprite.setTexture(texture);

    m_bullet = NULL;

    m_timer = (rand() % 40) + 80;
}

Hazard_CannonHoming::~Hazard_CannonHoming()
{
    delete m_bullet;
}

void Hazard_CannonHoming::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Hazard_CannonHoming::update()
{
    if (m_timer > 0)
        m_timer--;

    if (m_timer == 0)
    {
        if (player != NULL)
        {
            Vector2f position(m_sprite.getPosition());

            if (m_bullet != NULL || (player->m_aabb.left > position.x - 34 && player->m_aabb.left < position.x + 38))
                m_timer = 35;
            else
            {
                shot();

                m_timer = (rand() % 40) + 210;

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(12), sfxSamples[21], 0, NULL);
            }
        }
        else
        {
            shot();

            m_timer = (rand() % 40) + 210;

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(12), sfxSamples[21], 0, NULL);
        }
    }
}

void Hazard_CannonHoming::secureUpdate()
{
    if (m_bullet != NULL)
    {
        m_bullet->update();

        if (m_bullet->m_destroyed)
        {
            delete m_bullet;
            m_bullet = NULL;
        }
    }
}

void Hazard_CannonHoming::afterUpdate()
{
    if (m_bullet != NULL)
        mainTexture.draw(*m_bullet);
}

ID Hazard_CannonHoming::getID() const
{
    return ID_USELESS;
}
