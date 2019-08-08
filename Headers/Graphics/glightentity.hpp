/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef LIGHT_ENTITY_INCLUDED
#define LIGHT_ENTITY_INCLUDED

#include <SFML/Graphics.hpp>

typedef struct LightData
{
    float x;
    float y;

    float moveablex;
    float moveabley;

    bool litTextures;
    bool litEyecandies;

    sf::Color lightColor;
} LightData;

#endif
