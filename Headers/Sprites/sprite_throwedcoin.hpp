/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITETHROWEDCOIN_INCLUDED
#define SPRITETHROWEDCOIN_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/pawn.hpp"
#include "sprite_lakitu.hpp"

class Sprite_ThrowedCoin : public Pawn
{
    public :

        Sprite_ThrowedCoin(const sf::Texture& texture, sf::Vector2f speed, Sprite_Lakitu* parent);

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
        sf::Vector2f m_movedistance;

        Sprite_Lakitu* m_parent;

        bool m_onfloor;
};

#endif
