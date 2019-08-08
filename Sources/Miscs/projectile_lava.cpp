/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/globals.hpp"
#include "../../Headers/Miscs/projectile_lava.hpp"

#define LAVA_MAXVSPEED 12

using namespace std;
using namespace sf;

Projectile_Lava::Projectile_Lava(Texture* texture, Vector2f position, Vector2f speed)
{
    m_sprite.setTexture(*texture);
    m_sprite.setTextureRect(IntRect(0, 0, 25, 26));
    m_sprite.setPosition(position);

    m_aabb = FloatRect(position.x, position.y, 25, 25);

    m_movedistance = speed;

    m_frame = 0;
}

void Projectile_Lava::update()
{
    if (m_frame < 7)
        m_frame++;
    else
        m_frame = 0;

    if (m_frame % 4 == 0)
        m_sprite.setTextureRect(IntRect((m_frame / 4) * 25, 0, 25, 26));

    if (m_movedistance.y < LAVA_MAXVSPEED - star_gravity)
        m_movedistance.y += star_gravity;

    m_aabb.left += m_movedistance.x;
    m_aabb.top += m_movedistance.y;

    m_sprite.setPosition(m_aabb.left, m_aabb.top);

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

void Projectile_Lava::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
