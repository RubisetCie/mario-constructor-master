/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDCANNONHOMINGVR_INCLUDED
#define HAZARDCANNONHOMINGVR_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_cannonhoming.hpp"

class Hazard_CannonHomingVR : public Hazard_CannonHoming
{
    public :

        Hazard_CannonHomingVR(const sf::Texture& texture);

        void update();

        void shot();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
