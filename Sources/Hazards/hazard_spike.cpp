/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_spike.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Hazard_Spike::Hazard_Spike(const Vector2i& coords)
{
    m_sprite.setTexture(*mainTileset);
    m_sprite.setTextureRect(IntRect(coords, Vector2i(32, 32)));
}

void Hazard_Spike::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);
}

void Hazard_Spike::update()
{
    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (player->m_animpowerup > 0)
        return;

    if (game_powerup <= 1 && player->m_animtrace > 0)
        return;

    if (player->m_invincibility > 0)
        return;

    if (player->m_aabb.intersects(FloatRect(m_sprite.getPosition(), Vector2f(32, 32))))
    {
        if (game_powerup == 0)
            player->death();
        else if (game_powerup == 1)
        {
            player->setPower(0, true);

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[6], 0, NULL);
        }
        else
        {
            player->setPower(1, true);

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[6], 0, NULL);
        }
    }
}

void Hazard_Spike::secureUpdate()
{
}

void Hazard_Spike::afterUpdate()
{
}

Vector2f Hazard_Spike::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Hazard_Spike::getSize() const
{
    return Vector2f(32, 32);
}

ID Hazard_Spike::getID() const
{
    return ID_USELESS;
}

void Hazard_Spike::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
