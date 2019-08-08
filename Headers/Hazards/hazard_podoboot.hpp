/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDPODOBOOT_INCLUDED
#define HAZARDPODOBOOT_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_podoboo.hpp"

class Hazard_PodobooT : public Hazard_Podoboo
{
    public :

        Hazard_PodobooT(const sf::Texture& texture, float maxheight);

        void setPosition(sf::Vector2f pos);

        void secureUpdate();

        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
