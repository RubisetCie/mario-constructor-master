/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SCENERYWATERFALL_INCLUDED
#define SCENERYWATERFALL_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Scenery_Waterfall : public Placeable
{
    public :

        Scenery_Waterfall(const sf::Texture& texture);

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
