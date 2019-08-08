/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPLANTGREYB_INCLUDED
#define SPRITEPLANTGREYB_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_plantb.hpp"

class Sprite_PlantGreyB : public Sprite_PlantB
{
    public :

        Sprite_PlantGreyB(sf::Texture* texture, Collider* collision);

        void update();
        void secureUpdate();
        void afterUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
