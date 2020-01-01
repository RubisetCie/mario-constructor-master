/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEBEET_INCLUDED
#define SPRITEBEET_INCLUDED

#include <SFML/Graphics.hpp>
#include "fallable.hpp"

class Sprite_Beet : public Fallable
{
    public :

        Sprite_Beet(const sf::Texture& texture);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        unsigned int m_frame;
};

#endif
