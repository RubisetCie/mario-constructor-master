/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Hazards/hazard_cannonvl.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/globals.hpp"

#define BULLETBILL_SPEED 3.2

using namespace sf;

Hazard_CannonVL::Hazard_CannonVL(const Texture& texture) : Hazard_Cannon(texture)
{
    m_sprite.setScale(1, -1);
    m_sprite.setRotation(90);
}

void Hazard_CannonVL::shot()
{
    Vector2f position(m_sprite.getPosition());

    if (player->m_aabb.top < position.y)
    {
        m_bullets.emplace_back(new Projectile_Bulletbill(hazardsTxt[3], Vector2f(position.x + 15, position.y + 16), Vector2f(0, -BULLETBILL_SPEED), true));
        listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(position.x, position.y - 15)));
    }
    else
    {
        m_bullets.emplace_back(new Projectile_Bulletbill(hazardsTxt[3], Vector2f(position.x + 15, position.y + 16), Vector2f(0, BULLETBILL_SPEED), true));
        listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(position.x, position.y + 17)));
    }
}

Vector2f Hazard_CannonVL::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Hazard_CannonVL::getSize() const
{
    return Vector2f(31, 32);
}

void Hazard_CannonVL::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
