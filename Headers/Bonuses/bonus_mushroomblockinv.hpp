/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKMUSHROOMINV_INCLUDED
#define BLOCKMUSHROOMINV_INCLUDED

#include <SFML/Graphics.hpp>
#include "bonus_blockinv.hpp"

class Bonus_MushroomBlockInv : public Bonus_BlockInv
{
    public :

        Bonus_MushroomBlockInv(sf::Texture* blockTexture, Collider* collision, char once);

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
