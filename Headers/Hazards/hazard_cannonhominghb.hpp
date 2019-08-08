/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDCANNONHOMINGHB_INCLUDED
#define HAZARDCANNONHOMINGHB_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_cannonhoming.hpp"

class Hazard_CannonHomingHB : public Hazard_CannonHoming
{
    public :

        Hazard_CannonHomingHB(const sf::Texture& texture);

        void shot();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
