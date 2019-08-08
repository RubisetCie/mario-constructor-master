/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPLANTGREYL_INCLUDED
#define SPRITEPLANTGREYL_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_plantl.hpp"

class Sprite_PlantGreyL : public Sprite_PlantL
{
    public :

        Sprite_PlantGreyL(sf::Texture* texture, Collider* collision);

        void update();
        void secureUpdate();
        void afterUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
