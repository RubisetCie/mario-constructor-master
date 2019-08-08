/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BRICKCOIN_INCLUDED
#define BRICKCOIN_INCLUDED

#include <SFML/Graphics.hpp>
#include "bonus_block.hpp"

class Bonus_CoinBrick : public Placeable
{
    public :

        Bonus_CoinBrick(sf::Texture* blockTexture, unsigned int coins);

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

        unsigned int m_coins;

        char m_hit;

        float m_startheight;
        float m_hitspeed;
};

#endif
