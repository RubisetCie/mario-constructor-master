/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDPODOBOOFOUNTAINL_INCLUDED
#define HAZARDPODOBOOFOUNTAINL_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_podoboofountain.hpp"

class Hazard_PodobooFountainL : public Hazard_PodobooFountain
{
    public :

        Hazard_PodobooFountainL();

        void secureUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
