/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDPODOBOOR_INCLUDED
#define HAZARDPODOBOOR_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_podoboo.hpp"

class Hazard_PodobooR : public Hazard_Podoboo
{
    public :

        Hazard_PodobooR(const sf::Texture& texture, float maxheight);

        void setPosition(const sf::Vector2f& pos);

        void secureUpdate();

        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
