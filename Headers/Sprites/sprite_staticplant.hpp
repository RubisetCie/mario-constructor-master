/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef STATICPLANT_INCLUDED
#define STATICPLANT_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Sprite_StaticPlant : public Placeable
{
    public :

        Sprite_StaticPlant(const sf::Vector2f& pos, float angle);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        const sf::Vector2f m_mainPos;
        sf::Sprite m_sprite;
};

#endif
