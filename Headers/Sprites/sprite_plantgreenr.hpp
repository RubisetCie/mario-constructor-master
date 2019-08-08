/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPLANTGREENR_INCLUDED
#define SPRITEPLANTGREENR_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_plantr.hpp"

class Sprite_PlantGreenR : public Sprite_PlantR
{
    public :

        Sprite_PlantGreenR(sf::Texture* texture, Collider* collision);

        void update();
        void secureUpdate();
        void afterUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
