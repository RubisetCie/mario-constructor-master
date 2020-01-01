/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKFLOWERINV_INCLUDED
#define BLOCKFLOWERINV_INCLUDED

#include <SFML/Graphics.hpp>
#include "bonus_blockinv.hpp"

class Bonus_FlowerBlockInv : public Bonus_BlockInv
{
    public :

        Bonus_FlowerBlockInv(sf::Texture* blockTexture, Collider* collision, char once);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();

        void hit();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::VertexArray m_output;

        unsigned int m_frame;
        unsigned char m_id, m_outputdir;

        bool m_mushroom;
};

#endif
