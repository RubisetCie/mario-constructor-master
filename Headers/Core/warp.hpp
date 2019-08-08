/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef WARP_INCLUDED
#define WARP_INCLUDED

#include <SFML/Graphics.hpp>

typedef struct Warp
{
    sf::Vector2f entrance_position;
    sf::Vector2f exit_position;

    char entrance_dir;
    char exit_dir;
} Warp;

#endif
