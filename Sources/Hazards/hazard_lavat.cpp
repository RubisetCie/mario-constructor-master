/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_lavat.hpp"
#include "../../Headers/globals.hpp"

#define LAVA_BOWSERSPLASH 1.5

using namespace sf;

Hazard_LavaT::Hazard_LavaT(const sf::Texture& texture)
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 32, 34));
}

void Hazard_LavaT::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Hazard_LavaT::update()
{
    Hazard_Lava::update();

    /*if (koopaHealth == 0)
    {
        if (m_movedistance == 0)
        {
            if (zoneb)
            {
                for (list<Collider*>::iterator it = listColliderb.begin(); it != listColliderb.end(); it++)
                {
                    if ((*it)->type != C_BOWSERSPLASH || (*it)->object == NULL)
                        continue;

                    m_movedistance = LAVA_BOWSERSPLASH;
                }
            }
        }
    }*/

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    Vector2f pos(m_sprite.getPosition());

    if (player->m_aabb.intersects(FloatRect(Vector2f(pos.x, pos.y + 4), Vector2f(32, 30))))
        player->death();
}

Vector2f Hazard_LavaT::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Hazard_LavaT::getSize() const
{
    return Vector2f(32, 34);
}

void Hazard_LavaT::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
