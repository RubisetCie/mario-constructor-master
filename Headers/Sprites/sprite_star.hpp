/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITESTAR_INCLUDED
#define SPRITESTAR_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/pawn.hpp"

class Sprite_Star : public Pawn
{
    public :

        Sprite_Star(const sf::Texture& texture);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

        sf::Vector2f m_movedistance;

        bool m_active;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;
};

#endif
