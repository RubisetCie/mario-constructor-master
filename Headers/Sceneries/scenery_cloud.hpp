/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SCENERYCLOUD_INCLUDED
#define SCENERYCLOUD_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Scenery_Cloud : public Placeable
{
    public :

        Scenery_Cloud(const sf::Texture& texture);

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
