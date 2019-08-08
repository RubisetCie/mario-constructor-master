/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDPODOBOOB_INCLUDED
#define HAZARDPODOBOOB_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_podoboo.hpp"

class Hazard_PodobooB : public Hazard_Podoboo
{
    public :

        Hazard_PodobooB(const sf::Texture& texture, float maxheight);

        void setPosition(sf::Vector2f pos);

        void secureUpdate();

        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
