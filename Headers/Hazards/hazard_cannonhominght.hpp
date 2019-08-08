/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDCANNONHOMINGHT_INCLUDED
#define HAZARDCANNONHOMINGHT_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_cannonhoming.hpp"

class Hazard_CannonHomingHT : public Hazard_CannonHoming
{
    public :

        Hazard_CannonHomingHT(const sf::Texture& texture);

        void shot();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
