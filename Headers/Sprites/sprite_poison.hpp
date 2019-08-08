/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPOISON_INCLUDED
#define SPRITEPOISON_INCLUDED

#include <SFML/Graphics.hpp>
#include "walkable.hpp"

class Sprite_Poison : public Walkable
{
    public :

        Sprite_Poison(const sf::Texture& texture);

        void setPosition(sf::Vector2f pos);

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
