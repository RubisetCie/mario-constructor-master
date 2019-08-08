/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDLAVAB_INCLUDED
#define HAZARDLAVAB_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_lava.hpp"

class Hazard_LavaB : public Hazard_Lava
{
    public :

        Hazard_LavaB(const sf::Texture& texture);

        void setPosition(sf::Vector2f pos);

        void update();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
