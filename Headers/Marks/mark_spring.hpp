/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKSPRING_INCLUDED
#define MARKSPRING_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Mark_Spring : public Placeable
{
    public :

        Mark_Spring(const sf::Texture& texture, unsigned int strength);

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

        unsigned int m_strengh, m_frame;
};

#endif
