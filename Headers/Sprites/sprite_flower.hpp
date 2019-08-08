/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEFLOWER_INCLUDED
#define SPRITEFLOWER_INCLUDED

#include <SFML/Graphics.hpp>
#include "fallable.hpp"

class Sprite_Flower : public Fallable
{
    public :

        Sprite_Flower(const sf::Texture& texture);

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
