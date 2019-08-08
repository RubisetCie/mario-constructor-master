/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPLANTGREYT_INCLUDED
#define SPRITEPLANTGREYT_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_plantt.hpp"

class Sprite_PlantGreyT : public Sprite_PlantT
{
    public :

        Sprite_PlantGreyT(sf::Texture* texture, Collider* collision);

        void update();
        void secureUpdate();
        void afterUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
