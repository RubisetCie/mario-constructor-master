/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKPOISONINV_INCLUDED
#define BLOCKPOISONINV_INCLUDED

#include <SFML/Graphics.hpp>
#include "bonus_blockinv.hpp"

class Bonus_PoisonBlockInv : public Bonus_BlockInv
{
    public :

        Bonus_PoisonBlockInv(sf::Texture* blockTexture, Collider* collision);

        void setPosition(sf::Vector2f pos);

        void update();
        void secureUpdate();

        void hit();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::VertexArray m_output;

        unsigned char m_id, m_outputdir;
};

#endif
