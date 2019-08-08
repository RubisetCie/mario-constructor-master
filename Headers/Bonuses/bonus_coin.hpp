/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BONUSCOIN_INCLUDED
#define BONUSCOIN_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Bonus_Coin : public Placeable
{
    public :

        Bonus_Coin(const sf::Texture& texture);

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
