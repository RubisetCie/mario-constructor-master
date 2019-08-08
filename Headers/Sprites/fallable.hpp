/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef FALLABLE_INCLUDED
#define FALLABLE_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/pawn.hpp"

class Fallable : public Pawn
{
    public :

        Fallable();

        void update();

    protected :

        sf::Sprite m_sprite;

        float m_movedistance;

        bool m_onfloor;
};

#endif
