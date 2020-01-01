/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKCOIN_INCLUDED
#define BLOCKCOIN_INCLUDED

#include <SFML/Graphics.hpp>
#include "bonus_block.hpp"

class Bonus_CoinBlock : public Bonus_Block
{
    public :

        Bonus_CoinBlock(sf::Texture* blockTexture);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();

        void hit();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
