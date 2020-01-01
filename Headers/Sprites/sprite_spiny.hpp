/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITESPINY_INCLUDED
#define SPRITESPINY_INCLUDED

#include <SFML/Graphics.hpp>
#include "walkable.hpp"
#include "gotcollision.hpp"

class Sprite_Spiny : public Walkable, public GotCollision
{
    public :

        Sprite_Spiny(const sf::Texture& texture, Collider* collision);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        bool m_killed;
};

#endif
