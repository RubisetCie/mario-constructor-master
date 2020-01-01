/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKSTAR_INCLUDED
#define BLOCKSTAR_INCLUDED

#include <SFML/Graphics.hpp>
#include "bonus_block.hpp"

class Bonus_StarBlock : public Bonus_Block
{
    public :

        Bonus_StarBlock(sf::Texture* blockTexture);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();

        void hit();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::VertexArray m_output;

        unsigned char m_outputdir, m_timer;
        unsigned int m_frame;
};

#endif
