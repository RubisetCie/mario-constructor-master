/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKLUI_INCLUDED
#define BLOCKLUI_INCLUDED

#include <SFML/Graphics.hpp>
#include "bonus_block.hpp"

class Bonus_LuiBlock : public Bonus_Block
{
    public :

        Bonus_LuiBlock(sf::Texture* blockTexture);

        void setPosition(sf::Vector2f pos);

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
