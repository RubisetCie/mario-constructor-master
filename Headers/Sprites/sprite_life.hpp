/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITELIFE_INCLUDED
#define SPRITELIFE_INCLUDED

#include <SFML/Graphics.hpp>
#include "walkable.hpp"

class Sprite_Life : public Walkable
{
    public :

        Sprite_Life(const sf::Texture& texture);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
