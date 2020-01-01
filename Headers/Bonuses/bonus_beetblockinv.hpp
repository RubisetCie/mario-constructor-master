/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKBEETINV_INCLUDED
#define BLOCKBEETINV_INCLUDED

#include <SFML/Graphics.hpp>
#include "bonus_blockinv.hpp"

class Bonus_BeetBlockInv : public Bonus_BlockInv
{
    public :

        Bonus_BeetBlockInv(sf::Texture* blockTexture, Collider* collision, char once);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();

        void hit();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::VertexArray m_output;

        unsigned char m_outputdir;

        bool m_mushroom;
};

#endif
