/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITETHWOMPT_INCLUDED
#define SPRITETHWOMPT_INCLUDED

#include <SFML/Graphics.hpp>

class Sprite_ThwompT : public Pawn
{
    public :

        Sprite_ThwompT(const sf::Texture& texture);

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

        float m_startheight;
        float m_movedistance;

        unsigned char m_state;
        unsigned char m_laugh;
        unsigned char m_frame;
};

#endif
