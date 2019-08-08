/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDCANNONVR_INCLUDED
#define HAZARDCANNONVR_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_cannon.hpp"

class Hazard_CannonVR : public Hazard_Cannon
{
    public :

        Hazard_CannonVR(const sf::Texture& texture);

        void update();

        void shot();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
