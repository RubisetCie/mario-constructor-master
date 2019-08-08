/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/globals.hpp"
#include "../../Headers/Miscs/projectile_spike.hpp"

#define SPIKE_MAXVSPEED 12
#define SPIKE_ROTATION 24

using namespace std;
using namespace sf;

Projectile_Spike::Projectile_Spike(Texture* texture, Vector2f position, Vector2f speed)
{
    m_sprite.setTexture(*texture);
    m_sprite.setPosition(position);
    m_sprite.setOrigin(20, 20);

    m_aabb = FloatRect(position.x - 20, position.y - 20, 40, 40);

    m_movedistance = speed;

    m_tracetimer = 0;

    m_destroyed = false;
}

void Projectile_Spike::update()
{
    if (m_movedistance.y < SPIKE_MAXVSPEED - star_gravity)
        m_movedistance.y += star_gravity;

    if (m_movedistance.x > 0)
        m_sprite.rotate(SPIKE_ROTATION);
    else
        m_sprite.rotate(-SPIKE_ROTATION);

    m_aabb.left += m_movedistance.x;
    m_aabb.top += m_movedistance.y;

    if (m_aabb.top > cameraPos.y + 868)
        m_destroyed = true;

    m_sprite.setPosition(m_aabb.left+20, m_aabb.top+20);

    if (!m_tracing.empty() || !m_tracingb.empty())
        updateTrace();

    if (m_tracetimer < 2)
        m_tracetimer++;
    else
    {
        addTrace(Vector2f(m_aabb.left, m_aabb.top), IntRect(0, 0, 41, 40), true);

        m_tracetimer = 0;
    }

    if (!m_tracing.empty())
        mainTexture.draw(&m_tracing.front(), m_tracing.size(), Quads, hazardsTxt[9]);

    if (!m_tracingb.empty())
        mainTexture.draw(&m_tracingb.front(), m_tracingb.size(), Quads, hazardsTxt[9]);

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

    if (player->m_aabb.intersects(m_aabb))
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

void Projectile_Spike::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
