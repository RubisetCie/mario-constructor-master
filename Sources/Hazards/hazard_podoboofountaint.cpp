/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Hazards/hazard_podoboofountaint.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Hazard_PodobooFountainT::Hazard_PodobooFountainT() : Hazard_PodobooFountain()
{
}

void Hazard_PodobooFountainT::secureUpdate()
{
    if (m_timer > 0)
        m_timer--;

    if (m_timer > 140 && m_timer < 260)
    {
        if (m_timer % 15 == 0)
        {
            m_podoboo.emplace_back(new Projectile_Podoboo(hazardsTxt[1], Vector2f(m_position.x + 16, m_position.y + 16), Vector2f((rand() / (RAND_MAX / 8.0f)) - 4.0f, (rand() / (RAND_MAX / 4.0f)) - 10.0f), Vector2f(1, 1), 0));

            if (m_position.x > cameraPos.x - 384 && m_position.x < cameraPos.x + 352 && m_position.y > cameraPos.y - 304 && m_position.y < cameraPos.y + 272)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
        }
    }

    Hazard_PodobooFountain::secureUpdate();
}

void Hazard_PodobooFountainT::draw(RenderTarget& target, RenderStates) const
{
}
