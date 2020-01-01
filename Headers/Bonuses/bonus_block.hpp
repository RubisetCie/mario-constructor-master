/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCK_INCLUDED
#define BLOCK_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Bonus_Block : public Placeable
{
    public :

        Bonus_Block(sf::Texture* blockTexture);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

        virtual void hit() = 0;

    protected :

        sf::Sprite m_sprite;

        float m_startheight;
        float m_hitspeed;

        char m_hit;
};

#endif
