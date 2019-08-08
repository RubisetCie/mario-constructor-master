/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SCENERYSTATIC_INCLUDED
#define SCENERYSTATIC_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Scenery_Static : public Placeable
{
    public :

        Scenery_Static(const sf::Texture& texture, const sf::IntRect& rect);

        void setPosition(sf::Vector2f pos);

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
