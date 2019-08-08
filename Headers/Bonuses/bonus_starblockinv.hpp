/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKSTARINV_INCLUDED
#define BLOCKSTARINV_INCLUDED

#include <SFML/Graphics.hpp>
#include "bonus_blockinv.hpp"

class Bonus_StarBlockInv : public Bonus_BlockInv
{
    public :

        Bonus_StarBlockInv(sf::Texture* blockTexture, Collider* collision, char once);

        void setPosition(sf::Vector2f pos);

        void update();
        void secureUpdate();

        void hit();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::VertexArray m_output;

        unsigned char m_id, m_outputdir, m_timer;
        unsigned int m_frame;
};

#endif
