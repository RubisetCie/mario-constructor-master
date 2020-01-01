/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PLATFORMFALL_INCLUDED
#define PLATFORMFALL_INCLUDED

#include <SFML/Graphics.hpp>
#include "placeable.hpp"
#include "collider.hpp"

#define PLATFORMFALL_MAXVSPEED 8

class PlatformFall : public Placeable
{
    public :

        PlatformFall(const sf::Texture& texture, Collider* const collision);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;

        ID getID() const;

    protected :

        sf::Sprite m_sprite;
        sf::FloatRect m_aabb;

        Collider* m_collider;

        float m_movespeed;
};

#endif
