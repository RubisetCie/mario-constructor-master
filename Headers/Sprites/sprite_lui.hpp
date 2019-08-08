/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITELUI_INCLUDED
#define SPRITELUI_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/pawn.hpp"

class Sprite_Lui : public Pawn
{
    public :

        Sprite_Lui(const sf::Texture& texture);

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

        float m_movedistance;

        unsigned int m_frame;
};

#endif
