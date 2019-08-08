/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPLANTGREENT_INCLUDED
#define SPRITEPLANTGREENT_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_plantt.hpp"

class Sprite_PlantGreenT : public Sprite_PlantT
{
    public :

        Sprite_PlantGreenT(sf::Texture* texture, Collider* collision);

        void update();
        void secureUpdate();
        void afterUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
