/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDROTODISCC_INCLUDED
#define HAZARDROTODISCC_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_rotodisc.hpp"

class Hazard_RotodiscC : public Hazard_Rotodisc
{
    public :

        Hazard_RotodiscC(const sf::Texture& texture, sf::Vector2f disk_pos, float speed);

        void secureUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
