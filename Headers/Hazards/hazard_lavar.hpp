/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDLAVAR_INCLUDED
#define HAZARDLAVAR_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_lava.hpp"

class Hazard_LavaR : public Hazard_Lava
{
    public :

        Hazard_LavaR(const sf::Texture& texture);

        void setPosition(const sf::Vector2f& pos);

        void update();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
