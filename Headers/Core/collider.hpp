/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef COLLIDER_INCLUDED
#define COLLIDER_INCLUDED

#include <SFML/System.hpp>
#include "placeable.hpp"

enum ColliderType {C_IMMOBILE, C_PLATFORM, C_STOMPABLE, C_UNSTOMPABLE, C_CENTIPEDE};

/* Direction :
    - 0 : Solid
    - 1 : Only top
    - 2 : Only bottom
*/

typedef struct Collider
{
    sf::FloatRect rect;
    sf::Vector2f speed;
    int dir;
    enum ColliderType type;
    bool destroyed;
    Placeable* object;
} Collider;

#endif
