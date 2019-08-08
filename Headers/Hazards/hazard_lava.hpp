/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDLAVA_INCLUDED
#define HAZARDLAVA_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Hazard_Lava : public Placeable
{
    public :

        void update();
        void secureUpdate();
        void afterUpdate();

        ID getID() const;

    protected :

        sf::Sprite m_sprite;
};

#endif
