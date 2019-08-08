/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef ENTITYROTATION_INCLUDED
#define ENTITYROTATION_INCLUDED

#include <SFML/Graphics.hpp>
#include "entity.hpp"

class Entity_Rotation : public Entity
{
    public :

        Entity_Rotation(sf::Texture* texture, float x, float y, float angle, float originx, float originy, unsigned int type);

        sf::Vector2f getPosition() const;
};

#endif
