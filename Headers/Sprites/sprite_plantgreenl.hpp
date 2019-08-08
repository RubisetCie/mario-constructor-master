/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPLANTGREENL_INCLUDED
#define SPRITEPLANTGREENL_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_plantl.hpp"

class Sprite_PlantGreenL : public Sprite_PlantL
{
    public :

        Sprite_PlantGreenL(sf::Texture* texture, Collider* collision);

        void update();
        void secureUpdate();
        void afterUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
