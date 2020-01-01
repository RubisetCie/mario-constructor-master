/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDSPIKE_INCLUDED
#define HAZARDSPIKE_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Hazard_Spike : public Placeable
{
    public :

        Hazard_Spike(const sf::Vector2i& coords);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;
};

#endif
