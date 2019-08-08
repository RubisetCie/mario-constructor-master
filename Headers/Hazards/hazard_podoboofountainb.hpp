/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDPODOBOOFOUNTAINB_INCLUDED
#define HAZARDPODOBOOFOUNTAINB_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_podoboofountain.hpp"

class Hazard_PodobooFountainB : public Hazard_PodobooFountain
{
    public :

        Hazard_PodobooFountainB();

        void secureUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
