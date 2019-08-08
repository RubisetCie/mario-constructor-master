/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Hazards/hazard_cannonhominght.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/globals.hpp"

#define BULLETBILL_SPEED 3.2

using namespace sf;

Hazard_CannonHomingHT::Hazard_CannonHomingHT(const Texture& texture) : Hazard_CannonHoming(texture)
{
}

void Hazard_CannonHomingHT::shot()
{
    Vector2f position(m_sprite.getPosition());

    if (player->m_aabb.left < position.x + 2)
    {
        m_bullet = new Projectile_Bulletbill(hazardsTxt[3], Vector2f(position.x + 16, position.y + 15), Vector2f(-BULLETBILL_SPEED, 0), false, true);
        listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(position.x - 15, position.y)));
    }
    else
    {
        m_bullet = new Projectile_Bulletbill(hazardsTxt[3], Vector2f(position.x + 16, position.y + 15), Vector2f(BULLETBILL_SPEED, 0), false, true);
        listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(position.x + 17, position.y)));
    }
}

Vector2f Hazard_CannonHomingHT::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Hazard_CannonHomingHT::getSize() const
{
    return Vector2f(32, 31);
}

void Hazard_CannonHomingHT::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
