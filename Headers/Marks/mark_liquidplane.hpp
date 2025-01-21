/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKLIQUIDPLANE_INCLUDED
#define MARKLIQUIDPLANE_INCLUDED

#include <SFML/Graphics.hpp>

class Mark_LiquidPlane : public sf::Drawable
{
    public :

        virtual float getHeight() = 0;
        virtual bool getType() const = 0;

        virtual void setHeight(float height, sf::Vector2u dimensions) = 0;

        virtual void update() = 0;
};

#endif
