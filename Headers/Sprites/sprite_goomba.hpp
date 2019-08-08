/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEGOOMBA_INCLUDED
#define SPRITEGOOMBA_INCLUDED

#include <SFML/Graphics.hpp>
#include "walkable.hpp"
#include "gotcollision.hpp"

class Sprite_Goomba : public Walkable, public GotCollision
{
    public :

        Sprite_Goomba(const sf::Texture& texture, Collider* collision);

        void setPosition(sf::Vector2f pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        unsigned char m_state;
};

#endif
