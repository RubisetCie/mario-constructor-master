/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDLAVAL_INCLUDED
#define HAZARDLAVAL_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_lava.hpp"

class Hazard_LavaL : public Hazard_Lava
{
    public :

        Hazard_LavaL(const sf::Texture& texture);

        void setPosition(const sf::Vector2f& pos);

        void update();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
