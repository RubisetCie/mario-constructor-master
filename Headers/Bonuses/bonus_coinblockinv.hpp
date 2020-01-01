/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKCOININV_INCLUDED
#define BLOCKCOININV_INCLUDED

#include <SFML/Graphics.hpp>
#include "bonus_blockinv.hpp"

class Bonus_CoinBlockInv : public Bonus_BlockInv
{
    public :

        Bonus_CoinBlockInv(sf::Texture* blockTexture, Collider* collision, char once);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();

        void hit();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
