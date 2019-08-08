/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEMUSHROOM_INCLUDED
#define SPRITEMUSHROOM_INCLUDED

#include <SFML/Graphics.hpp>
#include "walkable.hpp"

class Sprite_Mushroom : public Walkable
{
    public :

        Sprite_Mushroom(const sf::Texture& texture);

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