/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Hazards/hazard_cannonvr.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/globals.hpp"

#define BULLETBILL_SPEED 3.2

using namespace sf;

Hazard_CannonVR::Hazard_CannonVR(const Texture& texture) : Hazard_Cannon(texture)
{
    m_sprite.setRotation(90);
}

void Hazard_CannonVR::update()
{
    if (m_timer > 0)
        m_timer--;

    if (m_timer == 0)
    {
        if (player != NULL)
        {
            Vector2f position(m_sprite.getPosition());

            if (player->m_aabb.left > position.x - 65 && player->m_aabb.left < position.x + 7)
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

void Hazard_CannonVR::shot()
{
    Vector2f position(m_sprite.getPosition());

    if (player->m_aabb.top < position.y)
    {
        m_bullets.emplace_back(new Projectile_Bulletbill(hazardsTxt[3], Vector2f(position.x - 15, position.y + 16), Vector2f(0, -BULLETBILL_SPEED), false));
        listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(position.x - 31, position.y - 15)));
    }
    else
    {
        m_bullets.emplace_back(new Projectile_Bulletbill(hazardsTxt[3], Vector2f(position.x - 15, position.y + 16), Vector2f(0, BULLETBILL_SPEED), false));
        listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(position.x - 31, position.y + 17)));
    }
}

Vector2f Hazard_CannonVR::getPosition() const
{
    Vector2f position(m_sprite.getPosition());
    return Vector2f(position.x - 31, position.y - 32);
}

Vector2f Hazard_CannonVR::getSize() const
{
    return Vector2f(31, 32);
}

void Hazard_CannonVR::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
