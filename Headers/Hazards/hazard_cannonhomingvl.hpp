/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDCANNONHOMINGVL_INCLUDED
#define HAZARDCANNONHOMINGL_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_cannonhoming.hpp"

class Hazard_CannonHomingVL : public Hazard_CannonHoming
{
    public :

        Hazard_CannonHomingVL(const sf::Texture& texture);

        void shot();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
