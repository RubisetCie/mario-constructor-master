/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef ENTITYJ_INCLUDED
#define ENTITYJ_INCLUDED

#include <SFML/Graphics.hpp>
#include "entity.hpp"

class Entityj : public Entity
{
    public :

        Entityj(sf::Texture* texture, sf::Texture* textText, float x, float y, float originx, float originy, unsigned int type, float angle = 0, bool ispodoboo = false);

    protected :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_spriteb;
};

#endif
