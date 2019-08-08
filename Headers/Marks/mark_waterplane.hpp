/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKWATERPLANE_INCLUDED
#define MARKWATERPLANE_INCLUDED

#include <SFML/Graphics.hpp>
#include "mark_liquidplane.hpp"

class Mark_WaterPlane : public Mark_LiquidPlane
{
    public :

        Mark_WaterPlane(sf::Texture* texture, float height, sf::Vector2i dimensions);

        float getHeight();
        bool getType() const;

        void setHeight(float height, sf::Vector2i dimensions);

        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::RectangleShape m_water;
        sf::RectangleShape m_waterdepth;
};

#endif
