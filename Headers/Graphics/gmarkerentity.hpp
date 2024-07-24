/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKER_ENTITY_INCLUDED
#define MARKER_ENTITY_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>

#include "../gglobals.hpp"

typedef struct MarkerData
{
    char levelURL[MAX_PATH];

    float x;
    float y;

    bool isLevel;
} MarkerData;

#endif
