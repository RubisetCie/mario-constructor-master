/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKLAVAPLANE_INCLUDED
#define MARKLAVAPLANE_INCLUDED

#include <SFML/Graphics.hpp>
#include "mark_liquidplane.hpp"

class Mark_LavaPlane : public Mark_LiquidPlane
{
    public :

        Mark_LavaPlane(sf::Texture* texture, float height, sf::Vector2i dimensions);

        float getHeight();
        bool getType() const;

        void setHeight(float height, sf::Vector2i dimensions);

        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::RectangleShape m_lava;
        sf::RectangleShape m_lavadepth;
};

#endif
