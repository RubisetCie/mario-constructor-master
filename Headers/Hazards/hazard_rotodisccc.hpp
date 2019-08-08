/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDROTODISCCC_INCLUDED
#define HAZARDROTODISCCC_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_rotodisc.hpp"

class Hazard_RotodiscCC : public Hazard_Rotodisc
{
    public :

        Hazard_RotodiscCC(const sf::Texture& texture, sf::Vector2f disk_pos, float speed);

        void secureUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
