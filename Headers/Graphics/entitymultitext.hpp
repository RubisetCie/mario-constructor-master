/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef ENTITYMULTITEXT_INCLUDED
#define ENTITYMULTITEXT_INCLUDED

#include <SFML/Graphics.hpp>
#include "entity.hpp"

class Entity_MultiText : public Entity
{
    public :

        Entity_MultiText(sf::Texture* texture, sf::IntRect textureRect, float x, float y, float originx, float originy, unsigned int type, unsigned short layer = 1, float angle = 0);

        sf::Vector2f getPosition() const;
};

#endif
