/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDCANNONHB_INCLUDED
#define HAZARDCANNONHB_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_cannon.hpp"

class Hazard_CannonHB : public Hazard_Cannon
{
    public :

        Hazard_CannonHB(const sf::Texture& texture);

        void shot();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
