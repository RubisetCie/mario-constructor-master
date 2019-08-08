/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPLANTGREENB_INCLUDED
#define SPRITEPLANTGREENB_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_plantb.hpp"

class Sprite_PlantGreenB : public Sprite_PlantB
{
    public :

        Sprite_PlantGreenB(sf::Texture* texture, Collider* collision);

        void update();
        void secureUpdate();
        void afterUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
