/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Hazards/hazard_cannonhominghb.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/globals.hpp"

#define BULLETBILL_SPEED 3.2

using namespace sf;

Hazard_CannonHomingHB::Hazard_CannonHomingHB(const Texture& texture) : Hazard_CannonHoming(texture)
{
    m_sprite.setScale(1, -1);
}

void Hazard_CannonHomingHB::shot()
{
    Vector2f position(m_sprite.getPosition());

    if (player->m_aabb.left < position.x + 2)
    {
        m_bullet = new Projectile_Bulletbill(hazardsTxt[3], Vector2f(position.x + 16, position.y - 15), Vector2f(-BULLETBILL_SPEED, 0), false, true);
        listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(position.x - 15, position.y - 31)));
    }
    else
    {
        m_bullet = new Projectile_Bulletbill(hazardsTxt[3], Vector2f(position.x + 16, position.y - 15), Vector2f(BULLETBILL_SPEED, 0), false, true);
        listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(position.x + 17, position.y - 31)));
    }
}

Vector2f Hazard_CannonHomingHB::getPosition() const
{
    Vector2f position(m_sprite.getPosition());
    return Vector2f(position.x, position.y - 31);
}

Vector2f Hazard_CannonHomingHB::getSize() const
{
    return Vector2f(32, 31);
}

void Hazard_CannonHomingHB::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
