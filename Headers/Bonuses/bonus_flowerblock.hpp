/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKFLOWER_INCLUDED
#define BLOCKFLOWER_INCLUDED

#include <SFML/Graphics.hpp>
#include "bonus_block.hpp"

class Bonus_FlowerBlock : public Bonus_Block
{
    public :

        Bonus_FlowerBlock(sf::Texture* blockTexture);

        void setPosition(sf::Vector2f pos);

        void update();
        void secureUpdate();

        void hit();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::VertexArray m_output;

        unsigned int m_frame;
        unsigned char m_outputdir;

        bool m_mushroom;
};

#endif
