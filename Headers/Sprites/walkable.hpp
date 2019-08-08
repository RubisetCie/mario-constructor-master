/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef WALKABLE_INCLUDED
#define WALKABLE_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/pawn.hpp"
#include "gotspeed.hpp"

class Walkable : public Pawn, public GotSpeed
{
    public :

        Walkable();

        void update();

        bool m_active;

    protected :

        sf::Sprite m_sprite;

        bool m_onfloor;
};

#endif
