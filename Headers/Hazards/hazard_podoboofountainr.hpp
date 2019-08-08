/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDPODOBOOFOUNTAINR_INCLUDED
#define HAZARDPODOBOOFOUNTAINR_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_podoboofountain.hpp"

class Hazard_PodobooFountainR : public Hazard_PodobooFountain
{
    public :

        Hazard_PodobooFountainR();

        void secureUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
