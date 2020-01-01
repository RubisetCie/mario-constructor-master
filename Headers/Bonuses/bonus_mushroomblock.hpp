/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKMUSHROOM_INCLUDED
#define BLOCKMUSHROOM_INCLUDED

#include <SFML/Graphics.hpp>
#include "bonus_block.hpp"

class Bonus_MushroomBlock : public Bonus_Block
{
    public :

        Bonus_MushroomBlock(sf::Texture* blockTexture);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();

        void hit();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::VertexArray m_output;

        unsigned char m_outputdir;
};

#endif
