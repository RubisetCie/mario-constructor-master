/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDLAVAT_INCLUDED
#define HAZARDLAVAT_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_lava.hpp"

class Hazard_LavaT : public Hazard_Lava
{
    public :

        Hazard_LavaT(const sf::Texture& texture);

        void setPosition(const sf::Vector2f& pos);

        void update();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
