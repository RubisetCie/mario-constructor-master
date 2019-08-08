/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PLATFORMMOV_INCLUDED
#define PLATFORMMOV_INCLUDED

#include <SFML/Graphics.hpp>
#include <cmath>
#include "placeable.hpp"
#include "collider.hpp"

class PlatformMov : public Placeable
{
    public :

        PlatformMov(const sf::Texture& texture, float speed, Collider* const collision);

        void setPosition(sf::Vector2f pos);

        void afterUpdate();

        sf::Vector2f getPosition() const;

        ID getID() const;

    protected :

        sf::Sprite m_sprite;

        sf::FloatRect m_aabb;

        Collider* m_collider;

        float m_movespeed;
        float m_speed;

        float m_startpoint;
        float m_targetpoint;

        bool m_forward;
};

#endif
