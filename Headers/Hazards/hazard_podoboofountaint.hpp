/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDPODOBOOFOUNTAINT_INCLUDED
#define HAZARDPODOBOOFOUNTAINT_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_podoboofountain.hpp"

class Hazard_PodobooFountainT : public Hazard_PodobooFountain
{
    public :

        Hazard_PodobooFountainT();

        void secureUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
