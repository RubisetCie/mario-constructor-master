/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPLANTGREYR_INCLUDED
#define SPRITEPLANTGREYR_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_plantr.hpp"

class Sprite_PlantGreyR : public Sprite_PlantR
{
    public :

        Sprite_PlantGreyR(sf::Texture* texture, Collider* collision);

        void update();
        void secureUpdate();
        void afterUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
